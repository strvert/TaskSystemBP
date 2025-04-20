#include "TSBTaskObject.h"

#include "TSBTaskResult.h"

FTSBTaskResult UTSBTaskObject::ExecuteTask()
{
	return K2_ExecuteTask();
}

#if WITH_EDITOR
bool UTSBTaskObject::ImplementsGetWorld() const
{
	return true;
}
#endif
