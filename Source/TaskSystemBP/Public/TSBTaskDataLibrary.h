#pragma once

#include "CoreMinimal.h"
#include "TSBFunctionLibrary.h"
#include "StructUtils/InstancedStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TSBTaskData.h"
#include "TSBTaskDataLibrary.generated.h"

struct FTSBTaskHandle;

namespace TaskSystemBP
{
	template <typename ResultStructType, typename ValueType = decltype(ResultStructType::ResultValue)>
	FTSBTaskData MakeTaskHandle(const ValueType& InValue)
	{
		FTSBTaskData Result;
		Result.ResultValue = FInstancedStruct::Make<ResultStructType>(InValue);
		return Result;
	}
	
	template <typename ResultStructType, typename ValueType = decltype(ResultStructType::ResultValue)>
	ValueType ExtractValue(const FTSBTaskData& InTaskHandle)
	{
		if (auto* Result = InTaskHandle.ResultValue.GetPtr<ResultStructType>())
		{
			return Result->ResultValue;
		}
		return ValueType();
	}

	template <typename ResultStructType, typename ValueType = decltype(ResultStructType::ResultValue)>
	ValueType ExtractValueFromHandle(const FTSBTaskHandle& InTaskHandle)
	{
		FTSBTaskData TaskHandle;
		if (!UTSBFunctionLibrary::GetTaskResult(InTaskHandle, TaskHandle))
		{
			return ValueType();
		}
		return ExtractValue<ResultStructType, ValueType>(TaskHandle);
	}
}

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskData_Object
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UObject> ResultValue;

	FTSBTaskData_Object() = default;
	FTSBTaskData_Object(UObject* InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskData_String
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResultValue;

	FTSBTaskData_String() = default;
	FTSBTaskData_String(const FString& InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskData_Int
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ResultValue;

	FTSBTaskData_Int() : ResultValue(0) {}
	FTSBTaskData_Int(const int32 InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskData_Float
{
	GENERATED_BODY()

	UPROPERTY()
	float ResultValue;

	FTSBTaskData_Float() : ResultValue(0.0f) {}
	FTSBTaskData_Float(const float InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskData_Bool
{
	GENERATED_BODY()

	UPROPERTY()
	bool ResultValue;

	FTSBTaskData_Bool() : ResultValue(false) {}
	FTSBTaskData_Bool(const bool InValue) : ResultValue(InValue) {}
};

UENUM()
enum class ETSBTaskDataStatus : uint8
{
	Valid,
	NotValid,
};

UCLASS()
class TASKSYSTEMBP_API UTSBTaskDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle")
	static FORCEINLINE FTSBTaskData FromObject(UObject* InValue)
	{
		return TaskSystemBP::MakeTaskHandle<FTSBTaskData_Object>(InValue);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle")
	static FORCEINLINE UObject* TaskDataToObject(const FTSBTaskData& InTaskHandle)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskData_Object>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle")
	static FORCEINLINE UObject* TaskHandleToObject(const FTSBTaskHandle& InTaskHandle)
	{
		return TaskSystemBP::ExtractValueFromHandle<FTSBTaskData_Object>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskData Conv_StringToTaskData(const FString& InValue)
	{
		return TaskSystemBP::MakeTaskHandle<FTSBTaskData_String>(InValue);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FString Conv_TaskDataToString(const FTSBTaskData& InTaskHandle)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskData_String>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FString Conv_TaskHandleToString(const FTSBTaskHandle& InTaskHandle)
	{
		return TaskSystemBP::ExtractValueFromHandle<FTSBTaskData_String>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskData Conv_FloatToTaskData(const float& InValue)
	{
		return TaskSystemBP::MakeTaskHandle<FTSBTaskData_Float>(InValue);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE float Conv_TaskDataToFloat(const FTSBTaskData& InTaskHandle)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskData_Float>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE float Conv_TaskHandleToFloat(const FTSBTaskHandle& InTaskHandle)
	{
		return TaskSystemBP::ExtractValueFromHandle<FTSBTaskData_Float>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskData Conv_IntToTaskData(const int32& InValue)
	{
		return TaskSystemBP::MakeTaskHandle<FTSBTaskData_Int>(InValue);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE int32 Conv_TaskDataToInt(const FTSBTaskData& InTaskHandle)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskData_Int>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE int32 Conv_TaskHandleToInt(const FTSBTaskHandle& InTaskHandle)
	{
		return TaskSystemBP::ExtractValueFromHandle<FTSBTaskData_Int>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskData Conv_BoolToTaskData(const bool& InValue)
	{
		return TaskSystemBP::MakeTaskHandle<FTSBTaskData_Bool>(InValue);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE bool Conv_TaskDataToBool(const FTSBTaskData& InTaskHandle)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskData_Bool>(InTaskHandle);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskHandle", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE bool Conv_TaskHandleToBool(const FTSBTaskHandle& InTaskHandle)
	{
		return TaskSystemBP::ExtractValueFromHandle<FTSBTaskData_Bool>(InTaskHandle);
	}
	
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "TaskSystemBP|TaskHandle",
		meta = (CustomStructureParam = "InValue"))
	static FORCEINLINE FTSBTaskData MakeTaskStructResult(const int32& InValue);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "TaskSystemBP|TaskHandle",
		meta = (ExpandEnumAsExecs = "ExecResult", CustomStructureParam = "OutValue"))
	static FORCEINLINE void GetTaskStructResult(ETSBTaskDataStatus& ExecResult, UPARAM(ref) FTSBTaskHandle& TaskHandle, int32& OutValue);

private:
	DECLARE_FUNCTION(execMakeTaskStructResult);
	DECLARE_FUNCTION(execGetTaskStructResult);
};
