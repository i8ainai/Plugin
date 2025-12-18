// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UDPStruct.generated.h"

USTRUCT(BlueprintType)
struct FTest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float C;
};

USTRUCT(BlueprintType)
struct FTest2
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 A;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float C;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	FTest D;
};