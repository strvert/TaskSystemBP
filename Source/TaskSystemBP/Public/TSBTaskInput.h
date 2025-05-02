#pragma once
#include "CoreMinimal.h"
#include "TSBTask.h"
#include "TSBTaskInput.generated.h"

USTRUCT(BlueprintType)
struct FTSBTaskInput
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskSystemBP")
	TMap<FString, FTSBTaskHandle> NamedPrerequisites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskSystemBP")
	FTSBTaskData CustomData;
};
