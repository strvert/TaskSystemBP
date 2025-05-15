#include "TSBFunctionLibrary.h"

#include "TSBEngineSubsystem.h"
#include "TSBLogChannels.h"
#include "TSBPipe.h"
#include "TSBTask.h"
#include "TSBTaskInput.h"
#include "TSBTaskObject.h"

#if WITH_EDITOR
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#endif

using namespace UE::Tasks;
using namespace TaskSystemBP;

#define LOCTEXT_NAMESPACE "TaskSystemBP"

namespace TaskSystemBP
{
	template <typename TaskBodyType, typename PrerequisitesCollectionType>
	TTask<TInvokeResult_T<TaskBodyType>> LaunchTaskConditional(
		const FTSBPipe& Pipe,
		const TCHAR* InDebugName,
		TaskBodyType&& TaskBody,
		PrerequisitesCollectionType&& Prerequisites,
		ETaskPriority Priority = ETaskPriority::Normal,
		EExtendedTaskPriority ExtendedPriority = EExtendedTaskPriority::None
	)
	{
		if (Pipe.Pipe.IsValid())
		{
			return Pipe.Pipe->Launch(
				InDebugName,
				Forward<TaskBodyType>(TaskBody),
				Forward<PrerequisitesCollectionType>(Prerequisites),
				Priority,
				ExtendedPriority
			);
		}
		return Launch(
			InDebugName,
			Forward<TaskBodyType>(TaskBody),
			Forward<PrerequisitesCollectionType>(Prerequisites),
			Priority,
			ExtendedPriority
		);
	}
}

FTSBTaskHandle UTSBFunctionLibrary::LaunchTaskClass(UObject* WorldContextObject,
                                                    const TSubclassOf<UTSBTaskObject>& TaskClass,
                                                    const TArray<FTSBTaskHandle>& Prerequisites,
                                                    const FTSBTaskData& TaskInput,
                                                    const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
                                                    const FTSBPipe& Pipe,
                                                    const ETSBThreadingPolicy InThreadingPolicy)
{
	UTSBTaskObject* CDO = TaskClass->GetDefaultObject<UTSBTaskObject>();
	if (!IsValid(CDO))
	{
		return FTSBTaskHandle{};
	}

	const ETSBInstancingPolicy& InstancingPolicy = CDO->InstancingPolicy;
	if (InstancingPolicy == ETSBInstancingPolicy::NoInstance)
	{
		return LaunchTaskObject(CDO, Prerequisites, TaskInput, NamedPrerequisites, Pipe, InThreadingPolicy);
	}
	if (InstancingPolicy == ETSBInstancingPolicy::InstantiatePerExecution)
	{
		UTSBTaskObject* TaskObject = NewObject<UTSBTaskObject>(WorldContextObject, TaskClass);
		return LaunchTaskObject(TaskObject, Prerequisites, TaskInput, NamedPrerequisites, Pipe, InThreadingPolicy);
	}

	return FTSBTaskHandle{};
}

FTSBTaskHandle UTSBFunctionLibrary::LaunchTaskObject(UTSBTaskObject* TaskObject,
                                                     const TArray<FTSBTaskHandle>& Prerequisites,
                                                     const FTSBTaskData& TaskInput,
                                                     const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
                                                     const FTSBPipe& Pipe,
                                                     const ETSBThreadingPolicy InThreadingPolicy)
{
	if (!IsValid(TaskObject))
	{
		return FTSBTaskHandle{};
	}

	FTSBTaskInput Input;
	Input.NamedPrerequisites = NamedPrerequisites;
	Input.CustomData = TaskInput;

	const FString DebugName = *TaskObject->GetName();

	auto InternalTask = [TaskObject, InThreadingPolicy, Pipe, Input, DebugName]
	{
		if (!IsValid(TaskObject))
		{
			UE_LOG(LogTaskSystemBP, Warning,
			       TEXT("UTSBFunctionLibrary::LaunchTaskObject: TaskObject is not valid. DebugName: %s"), *DebugName);
			return;
		}
#if WITH_EDITOR
		if (UTSBEngineSubsystem::IsPaused())
		{
			UTSBEngineSubsystem* Subsystem = GEngine->GetEngineSubsystem<UTSBEngineSubsystem>();
			AddNested(LaunchTaskConditional(
				Pipe, *TaskObject->GetName(), [TaskObject, Input, DebugName]
				{
					if (!IsValid(TaskObject))
					{
						UE_LOG(LogTaskSystemBP, Warning,
						       TEXT("UTSBFunctionLibrary::LaunchTaskObject: TaskObject is not valid. DebugName: %s"),
						       *DebugName);
						return;
					}
					TaskObject->ExecuteTask(Input);
				}, Subsystem->WaitForUnpauseTask(), ETaskPriority::Normal, ToTaskPriority(InThreadingPolicy)));
			return;
		}
#endif
		TaskObject->ExecuteTask(Input);
	};

	TArray<Private::FTaskHandle> PrereqTaskHandles;
	PrereqTaskHandles.Append(HandleArrayToTaskArray(Prerequisites));
	PrereqTaskHandles.Append(HandleMapToTaskArray(NamedPrerequisites));

	const FTask Task = LaunchTaskConditional(
		Pipe, *TaskObject->GetName(), MoveTemp(InternalTask), MoveTemp(PrereqTaskHandles),
		ETaskPriority::Normal, ToTaskPriority(InThreadingPolicy));

	const auto ReturnTask = Launch(UE_SOURCE_LOCATION, [TaskObject]
	{
		if (IsValid(TaskObject))
		{
			return TaskObject->GetTaskResult();
		}
		return FTSBTaskData{};
	}, Task, ETaskPriority::Normal, EExtendedTaskPriority::Inline);

	return FTSBTaskHandle{ReturnTask};
}

FTSBTaskHandle UTSBFunctionLibrary::LaunchTaskEventWithResult(const FTSBTaskWithResult& TaskEvent,
                                                              const TArray<FTSBTaskHandle>& Prerequisites,
                                                              const FTSBTaskData& TaskInput,
                                                              const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
                                                              const FTSBPipe& Pipe,
                                                              const ETSBThreadingPolicy InThreadingPolicy)
{
	if (!TaskEvent.IsBound())
	{
		return FTSBTaskHandle{};
	}

	TSharedPtr<FTSBTaskData> ResultHolder = MakeShared<FTSBTaskData>();

	FTSBTaskInput Input;
	Input.NamedPrerequisites = NamedPrerequisites;
	Input.CustomData = TaskInput;

	const FString DebugName = *TaskEvent.GetFunctionName().ToString();

	auto InternalTask = [TaskEvent, ResultHolder, InThreadingPolicy, Pipe, Input, DebugName]() mutable
	{
#if WITH_EDITOR
		if (UTSBEngineSubsystem::IsPaused())
		{
			UTSBEngineSubsystem* Subsystem = GEngine->GetEngineSubsystem<UTSBEngineSubsystem>();
			auto LazyInternalTask = [TaskEvent, ResultHolder, Input, DebugName]
			{
				if (TaskEvent.IsBound())
				{
					*ResultHolder = TaskEvent.Execute(Input);
				}
				else
				{
					UE_LOG(LogTaskSystemBP, Warning,
					       TEXT("UTSBFunctionLibrary::LaunchTaskEventWithResult: TaskEvent is not bound. DebugName: %s"
					       ),
					       *DebugName);
				}
			};
			AddNested(LaunchTaskConditional(
				Pipe, *TaskEvent.GetFunctionName().ToString(), MoveTemp(LazyInternalTask),
				Subsystem->WaitForUnpauseTask(), ETaskPriority::Normal,
				ToTaskPriority(InThreadingPolicy)));
			return;
		}
#endif
		if (!TaskEvent.IsBound())
		{
			UE_LOG(LogTaskSystemBP, Warning,
			       TEXT("UTSBFunctionLibrary::LaunchTaskEventWithResult: TaskEvent is not bound. DebugName: %s"),
			       *DebugName);
			return;
		}
		*ResultHolder = TaskEvent.Execute(Input);
	};

	TArray<Private::FTaskHandle> PrereqTaskHandles;
	PrereqTaskHandles.Append(HandleArrayToTaskArray(Prerequisites));
	PrereqTaskHandles.Append(HandleMapToTaskArray(NamedPrerequisites));

	const FTask MainTask = LaunchTaskConditional(
		Pipe, *TaskEvent.GetFunctionName().ToString(), MoveTemp(InternalTask),
		MoveTemp(PrereqTaskHandles), ETaskPriority::Normal, ToTaskPriority(InThreadingPolicy));

	const auto ReturnTask = Launch(UE_SOURCE_LOCATION, [ResultHolder]
	{
		if (ResultHolder.IsValid())
		{
			return *ResultHolder;
		}
		return FTSBTaskData{};
	}, MainTask, ETaskPriority::Normal, EExtendedTaskPriority::Inline);

	return FTSBTaskHandle{ReturnTask};
}


FTSBTaskHandle UTSBFunctionLibrary::LaunchTaskEvent(const FTSBTask& TaskEvent,
                                                    const TArray<FTSBTaskHandle>& Prerequisites,
                                                    const FTSBTaskData& TaskInput,
                                                    const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
                                                    const FTSBPipe& Pipe,
                                                    const ETSBThreadingPolicy InThreadingPolicy)
{
	if (!TaskEvent.IsBound())
	{
		return FTSBTaskHandle{};
	}

	FTSBTaskInput Input;
	Input.NamedPrerequisites = NamedPrerequisites;
	Input.CustomData = TaskInput;

	const FString DebugName = *TaskEvent.GetFunctionName().ToString();

	auto InternalTask = [TaskEvent, InThreadingPolicy, Pipe, Input, DebugName]()
	{
#if WITH_EDITOR
		if (UTSBEngineSubsystem::IsPaused())
		{
			UTSBEngineSubsystem* Subsystem = GEngine->GetEngineSubsystem<UTSBEngineSubsystem>();
			auto LazyInternalTask = [TaskEvent, Input, DebugName]
			{
				if (!TaskEvent.IsBound())
				{
					UE_LOG(LogTaskSystemBP, Warning,
					       TEXT("UTSBFunctionLibrary::LaunchTaskEvent: TaskEvent is not bound. DebugName: %s"),
					       *DebugName);
					return;
				}
				TaskEvent.Execute(Input);
			};

			AddNested(LaunchTaskConditional(
				Pipe, UE_SOURCE_LOCATION, MoveTemp(LazyInternalTask),
				Subsystem->WaitForUnpauseTask(), ETaskPriority::Normal,
				ToTaskPriority(InThreadingPolicy)));
		}
		else
#endif
		{
			if (!TaskEvent.IsBound() || !IsValid(TaskEvent.GetUObject()))
			{
				UE_LOG(LogTaskSystemBP, Warning,
				       TEXT("UTSBFunctionLibrary::LaunchTaskEvent: TaskEvent is not bound. DebugName: %s"),
				       *DebugName);
				return;
			}
			TaskEvent.Execute(Input);
		}
	};

	TArray<Private::FTaskHandle> PrereqTaskHandles;
	PrereqTaskHandles.Append(HandleArrayToTaskArray(Prerequisites));
	PrereqTaskHandles.Append(HandleMapToTaskArray(NamedPrerequisites));

	const FTask Task = LaunchTaskConditional(
		Pipe, *DebugName, MoveTemp(InternalTask),
		MoveTemp(PrereqTaskHandles), ETaskPriority::Normal, ToTaskPriority(InThreadingPolicy));

	return FTSBTaskHandle{Task};
}

void UTSBFunctionLibrary::AddNestedTask(const FTSBTaskHandle& ChildTask)
{
	AddNested(*ChildTask.GetHandle());
}

void UTSBFunctionLibrary::AddPrerequisite(FTSBTaskHandle& Event, const FTSBTaskHandle& Prerequisite)
{
	const TSharedPtr<Private::FTaskHandle> EventHandle = Event.GetHandle();

	if (const TSharedPtr<Private::FTaskHandle> PrerequisiteHandle = Prerequisite.GetHandle();
		Event.GetTaskType() == ETSBTaskType::Event && EventHandle.IsValid() && PrerequisiteHandle.IsValid())
	{
		static_cast<FTaskEvent*>(EventHandle.Get())->AddPrerequisites(*PrerequisiteHandle.Get());
	}
}

void UTSBFunctionLibrary::Trigger(FTSBTaskHandle& Event)
{
	if (const TSharedPtr<Private::FTaskHandle> EventHandle = Event.GetHandle();
		Event.GetTaskType() == ETSBTaskType::Event && EventHandle.IsValid())
	{
		static_cast<FTaskEvent*>(EventHandle.Get())->Trigger();
	}
}

FTSBTaskHandle UTSBFunctionLibrary::CombineHandles(const TArray<FTSBTaskHandle>& Handles)
{
	return FTSBTaskHandle{
		Launch(UE_SOURCE_LOCATION, []
		{
		}, HandleArrayToTaskArray(Handles), ETaskPriority::Normal, EExtendedTaskPriority::Inline)
	};
}

void UTSBFunctionLibrary::BindCompletion(const FTSBTaskHandle& Task, const FTSBOnTaskCompleted& OnTaskCompleted)
{
	if (!Task.GetHandle().IsValid())
	{
		UE_LOG(LogTaskSystemBP, Warning, TEXT("UTSBFunctionLibrary::BindCompletion: Task is invalid"));
		return;
	}

	Launch(UE_SOURCE_LOCATION, [OnTaskCompleted, Task]
	{
		if (OnTaskCompleted.IsBound())
		{
			OnTaskCompleted.Execute(Task);
		}
	}, *Task.GetHandle(), ETaskPriority::Normal, EExtendedTaskPriority::GameThreadNormalPri);
}

bool UTSBFunctionLibrary::GetTaskResult(const FTSBTaskHandle& Task, FTSBTaskData& OutResult)
{
	if (!Task.GetHandle().IsValid())
	{
		UE_LOG(LogTaskSystemBP, Warning, TEXT("UTSBFunctionLibrary::GetTaskResult: Task is invalid"));
		return false;
	}

	if (Task.GetTaskType() != ETSBTaskType::TSBResultTask)
	{
		UE_LOG(LogTaskSystemBP, Warning,
		       TEXT("UTSBFunctionLibrary::GetTaskResult: Task is not having a result."));
		return false;
	}

	const TSharedRef<TTask<FTSBTaskData>> TaskRef = StaticCastSharedRef<TTask<FTSBTaskData>>(
		Task.GetHandle().ToSharedRef());
	if (!TaskRef->IsCompleted())
	{
		UE_LOG(LogTaskSystemBP, Warning, TEXT("UTSBFunctionLibrary::GetTaskResult: Task is not completed"));
		return false;
	}

	OutResult = TaskRef->GetResult();
	return true;
}

// FTSBTaskHandle UTSBFunctionLibrary::Any(const TArray<FTSBTaskHandle>& Tasks)
// {
// 	return FTSBTaskHandle{UE::Tasks::Any(Tasks)};
// }

TArray<FTSBTaskHandle> UTSBFunctionLibrary::Conv_HandleToHandleArray(const FTSBTaskHandle& InHandle)
{
	return TArray{InHandle};
}

FTSBTaskHandle UTSBFunctionLibrary::MakeTaskEvent(const FString& InDebugName)
{
	return FTSBTaskHandle{FTaskEvent{*InDebugName}};
}

FTSBPipe UTSBFunctionLibrary::MakePipe(const FString& InDebugName)
{
	return FTSBPipe{*InDebugName};
}

FTSBTaskHandle UTSBFunctionLibrary::MakeCompletedTask()
{
	return FTSBTaskHandle{
		Launch(UE_SOURCE_LOCATION, []
		{
		}, ETaskPriority::Normal, EExtendedTaskPriority::Inline)
	};
}

// FTSBCancellationToken UTSBFunctionLibrary::MakeCancellationToken()
// {
// 	return FTSBCancellationToken::MakeCancellationToken();
// }

#undef LOCTEXT_NAMESPACE
