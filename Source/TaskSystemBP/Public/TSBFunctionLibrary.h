#pragma once

#include "CoreMinimal.h"
#include "TSBCancellationToken.h"
#include "TSBDataTypes.h"
#include "TSBPipe.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TSBFunctionLibrary.generated.h"

struct FTSBPipe;
struct FTSBTaskHandle;
class UTSBTaskObject;


DECLARE_DYNAMIC_DELEGATE_OneParam(FTSBOnTaskCompleted, const FTSBTaskHandle&, TaskHandle);

UCLASS()
class TASKSYSTEMBP_API UTSBFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites,TaskClass", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			WorldContext = "WorldContextObject", BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskHandle LaunchTaskClass(
		UObject* WorldContextObject,
		const TSubclassOf<UTSBTaskObject>& TaskClass,
		const TArray<FTSBTaskHandle>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		const ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskHandle LaunchTaskObject(
		UTSBTaskObject* TaskObject,
		const TArray<FTSBTaskHandle>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		const ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy",
			BaseStruct = "/Script/TaskSystemBP.TSBTaskBase"))
	static FTSBTaskHandle LaunchTaskEventWithResult(
		const FTSBTaskWithResult& TaskEvent,
		const TArray<FTSBTaskHandle>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem",
		meta = (AutoCreateRefTerm = "TaskInput,Pipe,Prerequisites,NamedPrerequisites", AdvancedDisplay =
			"NamedPrerequisites,Pipe,InThreadingPolicy"))
	static FTSBTaskHandle LaunchTaskEvent(
		const FTSBTask& TaskEvent, const TArray<FTSBTaskHandle>& Prerequisites,
		const FTSBTaskData& TaskInput,
		const TMap<FString, FTSBTaskHandle>& NamedPrerequisites,
		const FTSBPipe& Pipe = FTSBPipe(),
		ETSBThreadingPolicy InThreadingPolicy = ETSBThreadingPolicy::GameThreadNormalPriority);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void AddNestedTask(const FTSBTaskHandle& ChildTask);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void AddPrerequisite(UPARAM(ref) FTSBTaskHandle& Event, const FTSBTaskHandle& Prerequisite);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void Trigger(UPARAM(ref) FTSBTaskHandle& Event);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static FTSBTaskHandle CombineHandles(const TArray<FTSBTaskHandle>& Handles);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static void BindCompletion(const FTSBTaskHandle& Task, const FTSBOnTaskCompleted& OnTaskCompleted);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	static bool GetTaskResult(const FTSBTaskHandle& Task, FTSBTaskData& OutResult);

	// UFUNCTION(BlueprintCallable, Category = "TaskSystem")
	// static FTSBTaskHandle Any(const TArray<FTSBTaskHandle>& Tasks);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem", meta = (AutoCreateRefTerm = "InDebugName"))
	static FTSBTaskHandle MakeTaskEvent(const FString& InDebugName);

	UFUNCTION(BlueprintCallable, Category = "TaskSystem", meta = (AutoCreateRefTerm = "InDebugName"))
	static FTSBPipe MakePipe(const FString& InDebugName);

	// UFUNCTION(BlueprintPure, Category = "TaskSystem")
	// static FTSBCancellationToken MakeCancellationToken();

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", BlueprintAutocast), Category = "TaskSystem")
	static TArray<FTSBTaskHandle> Conv_HandleToHandleArray(const FTSBTaskHandle& InHandle);
};
