#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JSONAnalys.generated.h"

USTRUCT(BlueprintType)
struct FJSONKeyValue
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Key;

    UPROPERTY(BlueprintReadOnly)
    FString Value;
};

UCLASS()
class JSONANALYS_API UJSONAnalys : public UBlueprintFunctionLibrary  // ¸Ä³É JSONANALYS_API
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "JSONAnalys")
    static bool ParseJSONKeys(const FString& JSONString, const TArray<FString>& Keys, TArray<FJSONKeyValue>& OutKeyValues);
};