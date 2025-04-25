#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TSBTaskResult.generated.h"

USTRUCT(BlueprintType)
struct TASKSYSTEMBP_API FTSBTaskResult
{
	GENERATED_BODY()

	UPROPERTY()
	FInstancedStruct ResultValue;
};

namespace TaskSystemBP
{
	template <typename ResultStructType, typename ValueType = decltype(ResultStructType::ResultValue)>
	FTSBTaskResult MakeTaskResult(const ValueType& InValue)
	{
		FTSBTaskResult Result;
		Result.ResultValue = FInstancedStruct::Make<ResultStructType>(InValue);
		return Result;
	}

	template <typename ResultStructType, typename ValueType = decltype(ResultStructType::ResultValue)>
	ValueType ExtractValue(const FTSBTaskResult& InTaskResult)
	{
		if (auto* Result = InTaskResult.ResultValue.GetPtr<ResultStructType>())
		{
			return Result->ResultValue;
		}
		return ValueType();
	}
}

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_String
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResultValue;

	FTSBTaskResult_String() = default; // デフォルトコンストラクタ
	FTSBTaskResult_String(const FString& InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Int
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ResultValue;

	FTSBTaskResult_Int() : ResultValue(0) {} // デフォルトコンストラクタ
	FTSBTaskResult_Int(const int32 InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Float
{
	GENERATED_BODY()

	UPROPERTY()
	float ResultValue;

	FTSBTaskResult_Float() : ResultValue(0.0f) {} // デフォルトコンストラクタ
	FTSBTaskResult_Float(const float InValue) : ResultValue(InValue) {}
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Bool
{
	GENERATED_BODY()

	UPROPERTY()
	bool ResultValue;

	FTSBTaskResult_Bool() : ResultValue(false) {} // デフォルトコンストラクタ
	FTSBTaskResult_Bool(const bool InValue) : ResultValue(InValue) {}
};

UCLASS()
class TASKSYSTEMBP_API UTSBTaskResultLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "TaskSystemBP|TaskResult",
		meta = (CustomStructureParam = "InValue"))
	static FORCEINLINE FTSBTaskResult MakeTaskResult(const int32& InValue);

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult")
	static FORCEINLINE FInstancedStruct GetTaskResultValue(const FTSBTaskResult& InTaskResult)
	{
		return InTaskResult.ResultValue;
	}


	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskResult Conv_StringToTaskResult(const FString& InTaskResult)
	{
		return TaskSystemBP::MakeTaskResult<FTSBTaskResult_String>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FString Conv_TaskResultToString(const FTSBTaskResult& InTaskResult)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskResult_String>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskResult Conv_FloatToTaskResult(const float& InTaskResult)
	{
		return TaskSystemBP::MakeTaskResult<FTSBTaskResult_Float>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE float Conv_TaskResultToFloat(const FTSBTaskResult& InTaskResult)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskResult_Float>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskResult Conv_IntToTaskResult(const int32& InTaskResult)
	{
		return TaskSystemBP::MakeTaskResult<FTSBTaskResult_Int>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE int32 Conv_TaskResultToInt(const FTSBTaskResult& InTaskResult)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskResult_Int>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE FTSBTaskResult Conv_BoolToTaskResult(const bool& InTaskResult)
	{
		return TaskSystemBP::MakeTaskResult<FTSBTaskResult_Bool>(InTaskResult);
	}

	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult", meta = (CompactNodeTitle = "->", BlueprintAutocast))
	static FORCEINLINE bool Conv_TaskResultToBool(const FTSBTaskResult& InTaskResult)
	{
		return TaskSystemBP::ExtractValue<FTSBTaskResult_Bool>(InTaskResult);
	}

private:
	DECLARE_FUNCTION(execMakeTaskResult);
};
