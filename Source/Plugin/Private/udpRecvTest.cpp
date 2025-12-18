// Fill out your copyright notice in the Description page of Project Settings.


#include "udpRecvTest.h"

bool UudpRecvTest::analyzeBytes(const TArray<uint8>& Bytes, UPARAM(ref)int32& OutStruct)
{
	return false;
}

DEFINE_FUNCTION(UudpRecvTest::execAnalyzeBytes)
{
	P_GET_TARRAY(uint8, Bytes);


	Stack.StepCompiledIn<FStructProperty>(nullptr);
	void* StructPtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);


	P_FINISH;


	if (!StructProp || !StructPtr)
	{
		*(bool*)RESULT_PARAM = false;
		return;
	}


	const int32 StructSize = StructProp->Struct->GetStructureSize();


	if (Bytes.Num() < StructSize)
	{
		*(bool*)RESULT_PARAM = false;
		return;
	}


	FMemory::Memcpy(StructPtr, Bytes.GetData(), StructSize);


	*(bool*)RESULT_PARAM = true;
}

