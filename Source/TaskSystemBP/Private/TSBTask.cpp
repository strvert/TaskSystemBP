#include "TSBTask.h"

TArray<UE::Tasks::Private::FTaskHandle> TaskSystemBP::HandleArrayToTaskArray(const TArray<FTSBTaskHandle>& Handles)
{
	TArray<UE::Tasks::Private::FTaskHandle> Tasks;
	Tasks.Reserve(Handles.Num());
	for (const auto& Handle : Handles)
	{
		if (Handle.TaskType != ETSBTaskType::None && Handle.Handle.IsValid())
		{
			Tasks.Add(*Handle.Handle);
		}
	}
	return Tasks;
}
