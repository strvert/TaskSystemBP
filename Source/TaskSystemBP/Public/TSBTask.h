#pragma once

#include "CoreMinimal.h"
#include "TSBTaskResult.h"
#include "UObject/Object.h"
#include "TSBTask.generated.h"

struct FInstancedStruct;

enum class ETSBTaskType : uint8
{
	None,
	Task,
	Event,
};

USTRUCT(BlueprintType, meta = (HasNativeMake))
struct TASKSYSTEMBP_API FTSBTaskHandle
{
	GENERATED_BODY()

	virtual ~FTSBTaskHandle() = default;
	virtual TSharedPtr<UE::Tasks::Private::FTaskHandle> GetTaskHandle() const PURE_VIRTUAL(, return nullptr;);

	FTSBTaskHandle() : Handle(nullptr), TaskType(ETSBTaskType::None)
	{
	}

	explicit FTSBTaskHandle(const UE::Tasks::TTask<FTSBTaskResult>& InTask)
		: Handle(MakeShared<UE::Tasks::TTask<FTSBTaskResult>>(InTask)), TaskType(ETSBTaskType::Task)
	{
	}

	explicit FTSBTaskHandle(const UE::Tasks::FTaskEvent& InEvent)
		: Handle(MakeShared<UE::Tasks::FTaskEvent>(InEvent)), TaskType(ETSBTaskType::Event)
	{
	}

	TSharedPtr<UE::Tasks::Private::FTaskHandle> Handle;
	ETSBTaskType TaskType;
};

// USTRUCT(BlueprintType)
// struct TASKSYSTEMBP_API FTSBTaskHandle : public FTSBTaskBase
// {
// 	GENERATED_BODY()
//
// 	explicit FTSBTaskHandle()
// 		: Task(MakeShared<UE::Tasks::TTask<FTSBTaskResult>>())
// 	{
// 	}
//
// 	explicit FTSBTaskHandle(const UE::Tasks::TTask<FTSBTaskResult>& InTask)
// 		: Task(MakeShared<UE::Tasks::TTask<FTSBTaskResult>>(InTask))
// 	{
// 	}
//
// 	virtual TSharedPtr<UE::Tasks::Private::FTaskHandle> GetTaskHandle() const override
// 	{
// 		return Task;
// 	}
//
// 	TSharedPtr<UE::Tasks::TTask<FTSBTaskResult>> Task;
// };
//
namespace TaskSystemBP
{
	TASKSYSTEMBP_API TArray<UE::Tasks::Private::FTaskHandle> ToTaskArray(const TArray<FTSBTaskHandle>& Handles);
}

//
// USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/TaskSystemBP.TSBFunctionLibrary.MakeTaskEvent"))
// struct TASKSYSTEMBP_API FTSBTaskEvent : public FTSBTaskBase
// {
// 	GENERATED_BODY()
//
// 	explicit FTSBTaskEvent()
// 		: Event(nullptr)
// 	{
// 	}
//
// 	explicit FTSBTaskEvent(const TCHAR* InDebugName) : Event(MakeShared<UE::Tasks::FTaskEvent>(InDebugName))
// 	{
// 	}
//
// 	virtual TSharedPtr<UE::Tasks::Private::FTaskHandle> GetTaskHandle() const override
// 	{
// 		return Event;
// 	}
//
// 	TSharedPtr<UE::Tasks::FTaskEvent> Event;
// };
