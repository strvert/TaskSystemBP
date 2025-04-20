#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "TSBEngineSubsystem.generated.h"

UCLASS()
class TASKSYSTEMBP_API UTSBEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

#if WITH_EDITOR
	static bool IsPaused();

	UE::Tasks::FTaskEvent WaitForUnpauseTask();

private:
	UE::Tasks::FTaskEvent GetUnpausedEvent() const;
	TSharedPtr<UE::Tasks::FTaskEvent> UnpausedEvent;

	void ResetEvent();	
	static void SleepForUnpause();

	std::atomic<bool> bIsWaitingForUnpause = false;
#endif
};
