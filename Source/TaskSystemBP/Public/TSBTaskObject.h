#pragma once

#include "CoreMinimal.h"
#include "TSBDataTypes.h"
#include "TSBTaskData.h"
#include "UObject/Object.h"
#include "TSBTaskObject.generated.h"

UCLASS(Abstract, Blueprintable)
class TASKSYSTEMBP_API UTSBTaskObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Task System", DisplayName = "Execute Task")
	void K2_ExecuteTask();

	UFUNCTION(BlueprintCallable, Category = "Task System")
	void SetTaskResult(const FTSBTaskData& InTaskResult);

	virtual void ExecuteTask();
	
	FTSBTaskData GetTaskResult();

	UPROPERTY(EditAnywhere, Category = "Task System")
	ETSBInstancingPolicy InstancingPolicy;

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override;
#endif

private:
	UPROPERTY()
	FTSBTaskData TaskResult;
};
