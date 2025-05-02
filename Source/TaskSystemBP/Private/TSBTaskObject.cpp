#include "TSBTaskObject.h"

void UTSBTaskObject::SetTaskResult(const FTSBTaskData& InTaskResult)
{
	TaskResult = InTaskResult;
}

void UTSBTaskObject::ExecuteTask()
{
	K2_ExecuteTask();
}

FTSBTaskData UTSBTaskObject::GetTaskResult()
{
	return TaskResult;
}

#if WITH_EDITOR
bool UTSBTaskObject::ImplementsGetWorld() const
{
	return true;
}
#endif
