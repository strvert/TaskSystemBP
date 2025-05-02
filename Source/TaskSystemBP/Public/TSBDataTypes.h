#pragma once

#include "CoreMinimal.h"
#include "TSBTaskData.h"
#include "TSBDataTypes.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FTSBTaskData, FTSBTaskWithResult, const FTSBTaskInput&, TaskInput);
DECLARE_DYNAMIC_DELEGATE_OneParam(FTSBTask, const FTSBTaskInput&, TaskInput);

UENUM()
enum class ETSBInstancingPolicy : uint8
{
	NoInstance, // No instance, always use the class default.
	InstantiatePerExecution, // Create a new instance for each execution.
};

UENUM()
enum class ETSBThreadingPolicy : uint8
{
	GameThreadNormalPriority, // Execute on the game thread with normal priority.
	GameThreadHighPriority, // Execute on the game thread with high priority.
	WorkerThread, // Execute on a worker thread.
};

namespace TaskSystemBP
{
	inline UE::Tasks::EExtendedTaskPriority ToTaskPriority(const ETSBThreadingPolicy ThreadingPolicy)
	{
		switch (ThreadingPolicy)
		{
		case ETSBThreadingPolicy::GameThreadNormalPriority:
			return UE::Tasks::EExtendedTaskPriority::GameThreadNormalPri;
		case ETSBThreadingPolicy::GameThreadHighPriority:
			return UE::Tasks::EExtendedTaskPriority::GameThreadHiPri;
		case ETSBThreadingPolicy::WorkerThread:
			return UE::Tasks::EExtendedTaskPriority::None;
		}
		return UE::Tasks::EExtendedTaskPriority::None;
	}
}
