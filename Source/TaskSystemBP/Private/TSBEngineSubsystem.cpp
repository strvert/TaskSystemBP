#include "TSBEngineSubsystem.h"

#include "TSBLogChannels.h"

void UTSBEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if WITH_EDITOR
	ResetEvent();
#endif
}

void UTSBEngineSubsystem::Deinitialize()
{
#if WITH_EDITOR	
	if (UnpausedEvent.IsValid())
	{
		UnpausedEvent->Trigger();
	}
	UnpausedEvent.Reset();
#endif
}

#if WITH_EDITOR
bool UTSBEngineSubsystem::IsPaused()
{
	return GEditor && GEditor->PlayWorld && GEditor->PlayWorld->bDebugPauseExecution;
}

UE::Tasks::FTaskEvent UTSBEngineSubsystem::WaitForUnpauseTask()
{
	if (bIsWaitingForUnpause.exchange(true))
	{
		// If we are already waiting for unpause, just return the event
		return *UnpausedEvent;
	}
	
	// If we are not waiting for unpause, reset the event and wait for it
	ResetEvent();
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this]
	{
		while (IsPaused())
		{
			FPlatformProcess::Sleep(0.1);
		}
		FPlatformProcess::Sleep(0.3);
		UnpausedEvent->Trigger();
		bIsWaitingForUnpause = false;
	});
	
	return *UnpausedEvent;
}

UE::Tasks::FTaskEvent UTSBEngineSubsystem::GetUnpausedEvent() const
{
	return *UnpausedEvent;
}

void UTSBEngineSubsystem::ResetEvent()
{
	if (UnpausedEvent.IsValid())
	{
		UnpausedEvent->Trigger();
	}
	UnpausedEvent = MakeShared<UE::Tasks::FTaskEvent>(TEXT("TSB UnpausedEvent"));
}

void UTSBEngineSubsystem::SleepForUnpause()
{
	while (IsPaused())
	{
		FPlatformProcess::Sleep(0.1);
	}
}
#endif
