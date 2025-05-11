#include "TSBTask.h"

TArray<UE::Tasks::Private::FTaskHandle> TaskSystemBP::HandleArrayToTaskArray(const TArray<FTSBTaskBase>& Handles)
{
	TArray<UE::Tasks::Private::FTaskHandle> Tasks;
	Tasks.Reserve(Handles.Num());
	for (const auto& Handle : Handles)
	{
		if (Handle.GetTaskType() != ETSBTaskType::None && Handle.GetHandle().IsValid())
		{
			Tasks.Add(*Handle.GetHandle());
		}
	}
	return Tasks;
}
