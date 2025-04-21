#pragma once

#include "CoreMinimal.h"
#include "TSBDataTypes.h"
#include "TSBTaskResult.h"
#include "UObject/Object.h"
#include "TSBTaskObject.generated.h"

UCLASS(Abstract, Blueprintable)
class TASKSYSTEMBP_API UTSBTaskObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Task System")
	FTSBTaskResult K2_ExecuteTask();

	virtual FTSBTaskResult ExecuteTask();


	UPROPERTY(EditAnywhere, Category = "Task System")
	ETSBInstancingPolicy InstancingPolicy;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif
};
