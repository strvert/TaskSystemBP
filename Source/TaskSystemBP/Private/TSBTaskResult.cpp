#include "TSBTaskResult.h"

#include "Blueprint/BlueprintExceptionInfo.h"

#define LOCTEXT_NAMESPACE "TaskSystemBP"

DEFINE_FUNCTION(UTSBTaskResultLibrary::execMakeTaskResult)
{
	// Read wildcard Value input.
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	const void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	if (!ValueProp || !ValuePtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AbortExecution,
			LOCTEXT("InstancedStruct_MakeInvalidValueWarning", "Invalid value passed to MakeTaskResult")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);

		P_NATIVE_BEGIN;
			static_cast<FTSBTaskResult*>(RESULT_PARAM)->ResultValue.Reset();
		P_NATIVE_END;
	}
	else
	{
		P_NATIVE_BEGIN;
			static_cast<FTSBTaskResult*>(RESULT_PARAM)->ResultValue.InitializeAs(
				ValueProp->Struct,
				static_cast<const uint8*>(ValuePtr));
		P_NATIVE_END;
	}
}

#undef LOCTEXT_NAMESPACE
