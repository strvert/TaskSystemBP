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
	template <typename ResultStructType, typename ValueType = typename ResultStructType::ValueType>
	FTSBTaskResult MakeTaskResult(const ValueType& InValue)
	{
		FTSBTaskResult Result;
		Result.ResultValue = FInstancedStruct::Make<ResultStructType>(InValue);
		return Result;
	}

	template <typename ResultStructType, typename ValueType = typename ResultStructType::ValueType>
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

	using ValueType = FString;

	UPROPERTY()
	FString ResultValue;
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Int
{
	GENERATED_BODY()

	using ValueType = int32;

	UPROPERTY()
	int32 ResultValue;
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Float
{
	GENERATED_BODY()

	using ValueType = float;

	UPROPERTY()
	float ResultValue;
};

USTRUCT()
struct TASKSYSTEMBP_API FTSBTaskResult_Bool
{
	GENERATED_BODY()

	using ValueType = bool;

	UPROPERTY()
	bool ResultValue;
};

UCLASS()
class TASKSYSTEMBP_API UTSBTaskResultLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TaskSystemBP|TaskResult")
	static FORCEINLINE FTSBTaskResult MakeTaskResult(const FInstancedStruct& InValue)
	{
		return FTSBTaskResult{InValue};
	}

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
};
