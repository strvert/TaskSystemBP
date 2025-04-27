#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TSBPipe.generated.h"

USTRUCT(BlueprintType, meta = (HasNativeMake))
struct TASKSYSTEMBP_API FTSBPipe
{
	GENERATED_BODY()

	explicit FTSBPipe() : Pipe(nullptr)
	{
	}

	explicit FTSBPipe(const FString& InDebugName): Pipe(MakeShared<UE::Tasks::FPipe>(*InDebugName))
	{
	}

	TSharedPtr<UE::Tasks::FPipe> Pipe;
};
