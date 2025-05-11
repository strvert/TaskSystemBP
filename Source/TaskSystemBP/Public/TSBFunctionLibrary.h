#pragma once

#include "CoreMinimal.h"
#include "TSBCancellationToken.h"
#include "TSBDataTypes.h"
#include "TSBPipe.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TSBFunctionLibrary.generated.h"

struct FTSBPipe;
struct FTSBTaskBase;
class UTSBTaskObject;


DECLARE_DYNAMIC_DELEGATE_OneParam(FTSBOnTaskCompleted, const FTSBTaskBase&, TaskHandle);

UCLASS()
class TASKSYSTEMBP_API UTSBFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites,TaskClass", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			WorldContext = "WorldContextObject", BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskBase LaunchTaskClass(
		UObject* WorldContextObject,
		const TSubclassOf<UTSBTaskObject>& TaskClass,
		const TArray<FTSBTaskBase>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskBase>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		const ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskBase LaunchTaskObject(
		UTSBTaskObject* TaskObject,
		const TArray<FTSBTaskBase>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskBase>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		const ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskBase LaunchTaskEventWithResult(
		const FTSBTaskWithResult& TaskEvent,
		const TArray<FTSBTaskBase>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskBase>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy"))
	static FTSBTaskBase LaunchTaskEvent(
		const FTSBTask& TaskEvent, const TArray<FTSBTaskBase>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskBase>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void AddNestedTask(const FTSBTaskBase& ChildTask);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void AddPrerequisite(UPARAM(ref) FTSBTaskBase& Event, const FTSBTaskBase& Prerequisite);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void Trigger(UPARAM(ref) FTSBTaskBase& Event);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static FTSBTaskBase CombineHandles(const TArray<FTSBTaskBase>& Handles);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void BindCompletion(const FTSBTaskBase& Task, const FTSBOnTaskCompleted& OnTaskCompleted);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static bool GetTaskResult(const FTSBTaskBase& Task, FTSBTaskData& OutResult);

	// UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	// static FTSBTaskHandle Any(const TArray<FTSBTaskHandle>& Tasks);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem", meta = (AutoCreateRefTerm = "InDebugName"))
	static FTSBTaskBase MakeTaskEvent(const FString& InDebugName);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem", meta = (AutoCreateRefTerm = "InDebugName"))
	static FTSBPipe MakePipe(const FString& InDebugName);

	// UFUNCTION(BlueprintPure, Category = "TaskSystem")
	// static FTSBCancellationToken MakeCancellationToken();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", BlueprintAutocast), Category = "TaskSystem")
	static TArray<FTSBTaskBase> Conv_HandleToHandleArray(const FTSBTaskBase& InHandle);
};
