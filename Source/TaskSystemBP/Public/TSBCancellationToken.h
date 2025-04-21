#pragma once
#include "CoreMinimal.h"
#include "TSBCancellationToken.generated.h"

USTRUCT(BlueprintType)
struct FTSBCancellationToken
{
	GENERATED_BODY()

	explicit FTSBCancellationToken() : Token(nullptr)
	{
	}

	static FTSBCancellationToken MakeCancellationToken()
	{
		FTSBCancellationToken NewToken;
		NewToken.Token = MakeShared<UE::Tasks::FCancellationToken>();
		return NewToken;
	}

	TSharedPtr<UE::Tasks::FCancellationToken> Token;
};
