#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "readConfig.generated.h"

USTRUCT(BlueprintType)
struct FMyConfigData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    FString ClientIP;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 ClientPort;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 ClientPortA;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 ClientPortB;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 ClientPortC;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    FString ServerIP;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 ServerPort;


    FMyConfigData()
    {
        ClientIP = TEXT("192.168.100.101");
        ClientPort = 8151;

        ClientPortA = 8152;
        ClientPortB = 8153;
        ClientPortC = 8154;

        ServerIP = TEXT("192.168.100.160");
        ServerPort = 8052;
    }
};

USTRUCT(BlueprintType)
struct FExtraConfigData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    FString ExtraInfo;

    UPROPERTY(BlueprintReadWrite, Category = "MyConfig")
    int32 SomeValue;

    FExtraConfigData()
    {
        ExtraInfo = TEXT("");
        SomeValue = 0;
    }
};

UCLASS()
class READCONFIG_API UMyConfigLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MyConfig")
    static FMyConfigData GetMyConfig();

    UFUNCTION(BlueprintCallable, Category = "MyConfig")
    static bool SaveMyConfig(const FMyConfigData& ConfigData);

    UFUNCTION(BlueprintCallable, Category = "MyConfig")
    static FString GetConfigFilePath();

private:
    static void ParseINIContent(const FString& Content, FMyConfigData& OutConfigData);
    static FString GenerateINIContent(const FMyConfigData& ConfigData);
    static bool CreateDefaultConfigFile(const FString& ConfigPath);
};
