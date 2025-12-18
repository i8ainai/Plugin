// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "udpRecvTest.generated.h"

/**
 *
 */
UCLASS()
class PLUGIN_API UudpRecvTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "UDP",meta = (CustomStructureParam = "OutStruct"))
	static bool analyzeBytes(const TArray<uint8>& Bytes,UPARAM(ref) int32& OutStruct
	);
	DECLARE_FUNCTION(execAnalyzeBytes);
};
