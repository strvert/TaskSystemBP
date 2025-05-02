#include "TSBTaskDataLibrary.h"

#include "TSBFunctionLibrary.h"
#include "TSBTask.h"
#include "Blueprint/BlueprintExceptionInfo.h"

#define LOCTEXT_NAMESPACE "TaskSystemBP"

DEFINE_FUNCTION(UTSBTaskDataLibrary::execMakeTaskStructData)
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
			static_cast<FTSBTaskData*>(RESULT_PARAM)->DataValue.Reset();
		P_NATIVE_END;
	}
	else
	{
		P_NATIVE_BEGIN;
			static_cast<FTSBTaskData*>(RESULT_PARAM)->DataValue.InitializeAs(
				ValueProp->Struct,
				static_cast<const uint8*>(ValuePtr));
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(UTSBTaskDataLibrary::execGetTaskStructResult)
{
	P_GET_ENUM_REF(ETSBTaskDataStatus, ExecResult);
	P_GET_STRUCT_REF(FTSBTaskHandle, TaskHandle);

	// Read wildcard Value input.
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	ExecResult = ETSBTaskDataStatus::NotValid;

	if (!ValueProp || !ValuePtr)
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AbortExecution,
			LOCTEXT("InstancedStruct_GetInvalidValueWarning",
			        "Failed to resolve the Value for Get Instanced Struct Value")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
			FTSBTaskData TaskResult;
			if (!UTSBFunctionLibrary::GetTaskResult(TaskHandle, TaskResult))
			{
				ExecResult = ETSBTaskDataStatus::NotValid;
				return;
			}

			if (const FInstancedStruct& InstancedStruct = TaskResult.DataValue;
				InstancedStruct.IsValid() && InstancedStruct.GetScriptStruct()->IsChildOf(ValueProp->Struct))
			{
				ValueProp->Struct->CopyScriptStruct(ValuePtr, InstancedStruct.GetMemory());
				ExecResult = ETSBTaskDataStatus::Valid;
			}
			else
			{
				ExecResult = ETSBTaskDataStatus::NotValid;
			}
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(UTSBTaskDataLibrary::execGetTaskStructData)
{
	P_GET_ENUM_REF(ETSBTaskDataStatus, ExecResult);
	P_GET_STRUCT_REF(FTSBTaskData, TaskData);

	// Read wildcard Value input.
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	ExecResult = ETSBTaskDataStatus::NotValid;

	if (!ValueProp || !ValuePtr)
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AbortExecution,
			LOCTEXT("InstancedStruct_GetInvalidValueWarning",
			        "Failed to resolve the Value for Get Instanced Struct Value")
		);

		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
			if (const FInstancedStruct& InstancedStruct = TaskData.DataValue;
				InstancedStruct.IsValid() && InstancedStruct.GetScriptStruct()->IsChildOf(ValueProp->Struct))
			{
				ValueProp->Struct->CopyScriptStruct(ValuePtr, InstancedStruct.GetMemory());
				ExecResult = ETSBTaskDataStatus::Valid;
			}
			else
			{
				ExecResult = ETSBTaskDataStatus::NotValid;
			}
		P_NATIVE_END;
	}
}

#undef LOCTEXT_NAMESPACE
