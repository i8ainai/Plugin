#include "readConfig.h"
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, readConfig)

FMyConfigData UMyConfigLibrary::GetMyConfig()
{
    FMyConfigData Result;

    FString ConfigPath = GetConfigFilePath();

    FString ConfigDir = FPaths::GetPath(ConfigPath);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*ConfigDir))
    {
        PlatformFile.CreateDirectoryTree(*ConfigDir);
    }

    if (!PlatformFile.FileExists(*ConfigPath))
    {
        CreateDefaultConfigFile(ConfigPath);
    }

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *ConfigPath))
    {
        return Result;
    }

    ParseINIContent(FileContent, Result);

    UE_LOG(LogTemp, Log, TEXT("Config Loaded: Client %s:%d [%d,%d,%d], Server %s:%d"),
        *Result.ClientIP, Result.ClientPort,
        Result.ClientPortA, Result.ClientPortB, Result.ClientPortC,
        *Result.ServerIP, Result.ServerPort);

    return Result;
}

bool UMyConfigLibrary::SaveMyConfig(const FMyConfigData& ConfigData)
{
    FString ConfigPath = GetConfigFilePath();

    FString ConfigDir = FPaths::GetPath(ConfigPath);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*ConfigDir))
    {
        PlatformFile.CreateDirectoryTree(*ConfigDir);
    }

    FString Content = GenerateINIContent(ConfigData);

    return FFileHelper::SaveStringToFile(Content, *ConfigPath);
}

FString UMyConfigLibrary::GetConfigFilePath()
{
    return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Config/MyConfig.ini"));
}

void UMyConfigLibrary::ParseINIContent(const FString& Content, FMyConfigData& OutConfigData)
{
    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines);

    FString CurrentSection;

    for (const FString& Line : Lines)
    {
        FString TrimmedLine = Line.TrimStartAndEnd();

        if (TrimmedLine.IsEmpty() ||
            TrimmedLine.StartsWith(TEXT("#")) ||
            TrimmedLine.StartsWith(TEXT(";")))
        {
            continue;
        }

        if (TrimmedLine.StartsWith(TEXT("[")))
        {
            int32 EndBracket = TrimmedLine.Find(TEXT("]"));
            if (EndBracket > 1)
            {
                CurrentSection = TrimmedLine.Mid(1, EndBracket - 1).TrimStartAndEnd();
            }
            continue;
        }

        FString Key, Value;
        if (!TrimmedLine.Split(TEXT("="), &Key, &Value)) continue;

        Key = Key.TrimStartAndEnd();
        Value = Value.TrimStartAndEnd();

        if (CurrentSection.Equals(TEXT("Client"), ESearchCase::IgnoreCase))
        {
            if (Key.Equals(TEXT("IP"))) OutConfigData.ClientIP = Value;
            else if (Key.Equals(TEXT("Port"))) OutConfigData.ClientPort = FCString::Atoi(*Value);
            else if (Key.Equals(TEXT("PortA"))) OutConfigData.ClientPortA = FCString::Atoi(*Value);
            else if (Key.Equals(TEXT("PortB"))) OutConfigData.ClientPortB = FCString::Atoi(*Value);
            else if (Key.Equals(TEXT("PortC"))) OutConfigData.ClientPortC = FCString::Atoi(*Value);
        }
        else if (CurrentSection.Equals(TEXT("Server"), ESearchCase::IgnoreCase))
        {
            if (Key.Equals(TEXT("IP"))) OutConfigData.ServerIP = Value;
            else if (Key.Equals(TEXT("Port"))) OutConfigData.ServerPort = FCString::Atoi(*Value);
        }
    }
}

FString UMyConfigLibrary::GenerateINIContent(const FMyConfigData& ConfigData)
{
    FString Content;

    Content += TEXT("[Client]\n");
    Content += FString::Printf(TEXT("IP=%s\n"), *ConfigData.ClientIP);
    Content += FString::Printf(TEXT("Port=%d\n"), ConfigData.ClientPort);
    Content += FString::Printf(TEXT("PortA=%d\n"), ConfigData.ClientPortA);
    Content += FString::Printf(TEXT("PortB=%d\n"), ConfigData.ClientPortB);
    Content += FString::Printf(TEXT("PortC=%d\n"), ConfigData.ClientPortC);
    Content += TEXT("\n");

    Content += TEXT("[Server]\n");
    Content += FString::Printf(TEXT("IP=%s\n"), *ConfigData.ServerIP);
    Content += FString::Printf(TEXT("Port=%d\n"), ConfigData.ServerPort);
    Content += TEXT("\n");

    Content += TEXT("# Auto-generated config file\n");

    return Content;
}

bool UMyConfigLibrary::CreateDefaultConfigFile(const FString& ConfigPath)
{
    FMyConfigData DefaultConfig;
    FString Content = GenerateINIContent(DefaultConfig);
    return FFileHelper::SaveStringToFile(Content, *ConfigPath);
}