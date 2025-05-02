#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "TSBTaskData.generated.h"

USTRUCT(BlueprintType)
struct TASKSYSTEMBP_API FTSBTaskData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskSystemBP")
	FInstancedStruct TaskData;
};
