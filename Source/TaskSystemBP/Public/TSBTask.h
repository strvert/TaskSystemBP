#pragma once

#include "CoreMinimal.h"
#include "TSBTaskData.h"
#include "UObject/Object.h"
#include "TSBTask.generated.h"

struct FInstancedStruct;

enum class ETSBTaskType : uint8
{
	None,
	Task,
	Event,
	TSBResultTask,
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

	explicit FTSBTaskHandle(const UE::Tasks::FTask& InTask)
		: Handle(MakeShared<UE::Tasks::FTask>(InTask)), TaskType(ETSBTaskType::Task)
	{
	}

	explicit FTSBTaskHandle(const UE::Tasks::TTask<FTSBTaskData>& InTask)
		: Handle(MakeShared<UE::Tasks::TTask<FTSBTaskData>>(InTask)), TaskType(ETSBTaskType::TSBResultTask)
	{
	}

	explicit FTSBTaskHandle(const UE::Tasks::FTaskEvent& InEvent)
		: Handle(MakeShared<UE::Tasks::FTaskEvent>(InEvent)), TaskType(ETSBTaskType::Event)
	{
	}

	TSharedPtr<UE::Tasks::Private::FTaskHandle> Handle;
	ETSBTaskType TaskType;
};

namespace TaskSystemBP
{
	TASKSYSTEMBP_API TArray<UE::Tasks::Private::FTaskHandle> HandleArrayToTaskArray(const TArray<FTSBTaskHandle>& Handles);
	
	template<typename Key>
	TArray<UE::Tasks::Private::FTaskHandle> HandleMapToTaskArray(const TMap<Key, FTSBTaskHandle>& Handles)
	{
		TArray<UE::Tasks::Private::FTaskHandle> Result;
		Result.Reserve(Handles.Num());
		for (const auto& Pair : Handles)
		{
			Result.Add(*Pair.Value.Handle);
		}
		return Result;
	}
}
