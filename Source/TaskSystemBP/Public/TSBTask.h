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

	virtual ~FTSBTaskHandle()
	{
		if (Handle.IsValid())
		{
			if (TaskType == ETSBTaskType::Task)
			{
			}
			if (TaskType == ETSBTaskType::Event)
			{
				// クラッシュ回避実装。 Event は Trigger されていない状態で破棄されると check で落ちる。
				// Trigger されていない場合は、 Trigger してから破棄する。
				// ただし、構造体のデストラクタの破棄は常に Event の解放ではないので、 SharedReference が残り 1 の場合だけ Trigger する。
				//
				// TODO: Event や Pipe などのプリミティブは構造体内で直接管理するのではなく、より上位のライフサイクルで管理し、
				// 所有オブジェクトの破棄後にキャンセル処理などを適切に実行できるようにするべきである。
				// Trigger が破棄によるものかは、依存先のタスクからは判定できず、不適切なタスク実行に繋がる可能性がある。
				if (const TSharedPtr<UE::Tasks::FTaskEvent> Event = StaticCastSharedPtr<UE::Tasks::FTaskEvent>(Handle);
					Event.IsValid() && !Event->IsCompleted() && Event.GetSharedReferenceCount() == 1)
				{
					Event->Trigger();
				}
			}
			if (TaskType == ETSBTaskType::TSBResultTask)
			{
			}
		}
	}

	TSharedPtr<UE::Tasks::Private::FTaskHandle> GetHandle() const
	{
		return Handle;
	}

	ETSBTaskType GetTaskType() const
	{
		return TaskType;
	}

	// bool IsCancelled() const
	// {
	// 	return bIsCancelled;
	// }

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

private:
	TSharedPtr<UE::Tasks::Private::FTaskHandle> Handle;
	ETSBTaskType TaskType;
};

namespace TaskSystemBP
{
	TASKSYSTEMBP_API TArray<UE::Tasks::Private::FTaskHandle> HandleArrayToTaskArray(
		const TArray<FTSBTaskHandle>& Handles);

	template <typename Key>
	TArray<UE::Tasks::Private::FTaskHandle> HandleMapToTaskArray(const TMap<Key, FTSBTaskHandle>& Handles)
	{
		TArray<UE::Tasks::Private::FTaskHandle> Result;
		Result.Reserve(Handles.Num());
		for (const auto& Pair : Handles)
		{
			Result.Add(*Pair.Value.GetHandle());
		}
		return Result;
	}
}
