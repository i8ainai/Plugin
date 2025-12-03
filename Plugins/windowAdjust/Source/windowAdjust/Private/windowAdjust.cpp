#include "WindowAdjust.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"

struct FWindowConfig
{
    int32 Width = 1280;
    int32 Height = 720;
    int32 PosX = 100;
    int32 PosY = 100;
    bool bStartMinimized = false;
    FString Title = TEXT("Default Window");
};

static FString GetConfigPath()
{
    return FPaths::Combine(FPaths::ProjectDir(), TEXT("WindowConfig.json"));
}

static void SaveDefaultConfig(const FString& ConfigPath, const FWindowConfig& Config)
{
    //TSharedPtr智能指针，用来自动管理对象的生命周期，防止内存泄漏。
    //MakeShareable将裸指针包装成 TSharedPtr。等价于 MakeShared<FJsonObject>()
    //FJsonObject：一个字典 / 映射结构，用于在内存中表示 JSON 对象（键->值
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetNumberField(TEXT("Width"), Config.Width);
    JsonObject->SetNumberField(TEXT("Height"), Config.Height);
    JsonObject->SetNumberField(TEXT("PosX"), Config.PosX);
    JsonObject->SetNumberField(TEXT("PosY"), Config.PosY);
    JsonObject->SetBoolField(TEXT("StartMinimized"), Config.bStartMinimized);
    JsonObject->SetStringField(TEXT("Title"), Config.Title);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FFileHelper::SaveStringToFile(OutputString, *ConfigPath);
}

static FWindowConfig LoadWindowConfig()
{
    FWindowConfig Config;
    FString ConfigPath = GetConfigPath();

    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *ConfigPath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            Config.Width = JsonObject->GetIntegerField(TEXT("Width"));
            Config.Height = JsonObject->GetIntegerField(TEXT("Height"));
            Config.PosX = JsonObject->GetIntegerField(TEXT("PosX"));
            Config.PosY = JsonObject->GetIntegerField(TEXT("PosY"));
            Config.bStartMinimized = JsonObject->GetBoolField(TEXT("StartMinimized"));
            if (JsonObject->HasField(TEXT("Title")))
                Config.Title = JsonObject->GetStringField(TEXT("Title"));
        }
    }
    else
    {
        SaveDefaultConfig(ConfigPath, Config);
    }

    return Config;
}

class FWindowAdjustModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
#if PLATFORM_WINDOWS
        FCoreDelegates::OnFEngineLoopInitComplete.AddLambda([]()//注册lambda，在引擎初始化时修改窗口参数
            {
                if (GEngine && GEngine->GameViewport)
                {
                    FWindowConfig Config = LoadWindowConfig();

                    TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
                    if (Window.IsValid() && Window->GetNativeWindow().IsValid())
                    {
                        HWND Hwnd = (HWND)Window->GetNativeWindow()->GetOSWindowHandle();
                        if (Hwnd)
                        {
                            Window->SetWindowMode(EWindowMode::Windowed);

                        #if !WITH_EDITOR 
                            LONG Style = GetWindowLong(Hwnd, GWL_STYLE);
                            Style &= ~(WS_CAPTION | WS_THICKFRAME);
                            SetWindowLong(Hwnd, GWL_STYLE, Style);
                            SetWindowPos(Hwnd, nullptr, Config.PosX, Config.PosY, Config.Width, Config.Height, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
                        #endif

                            if (Config.bStartMinimized)
                            {
                                ::ShowWindow(Hwnd, SW_MINIMIZE);
                            }
                            else
                            {
                                ::ShowWindow(Hwnd, SW_RESTORE);
                            }
                            SetWindowTextW(Hwnd, *Config.Title);
                        }
                    }
                }
            });
#endif
    }

    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FWindowAdjustModule, WindowAdjust)

void UWindowAdjust::SetWindowSize(int32 Width, int32 Height)
{
#if PLATFORM_WINDOWS
    if (GEngine && GEngine->GameViewport)
    {
        TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
        if (Window.IsValid() && Window->GetNativeWindow().IsValid())
        {
            Window->SetWindowMode(EWindowMode::Windowed);
            Window->Resize(FVector2D(Width, Height));

            HWND Hwnd = (HWND)Window->GetNativeWindow()->GetOSWindowHandle();
            if (Hwnd)
            {
                ::SetWindowPos(Hwnd, nullptr, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER);
            }
        }
    }
#endif
}

void UWindowAdjust::SetWindowPosition(int32 X, int32 Y)
{
#if PLATFORM_WINDOWS
    if (GEngine && GEngine->GameViewport)
    {
        TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
        if (Window.IsValid() && Window->GetNativeWindow().IsValid())
        {
            HWND Hwnd = (HWND)Window->GetNativeWindow()->GetOSWindowHandle();
            if (Hwnd)
            {
                ::SetWindowPos(Hwnd, nullptr, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
        }
    }
#endif
}

void UWindowAdjust::SetWindowTitle(const FString& NewTitle)
{
#if PLATFORM_WINDOWS
#if WITH_EDITOR
    if (GEngine && GEngine->GameViewport)
    {
        TSharedPtr<SWindow> Window = GEngine->GameViewport->GetWindow();
        if (Window.IsValid() && Window->GetNativeWindow().IsValid())
        {
            HWND Hwnd = (HWND)Window->GetNativeWindow()->GetOSWindowHandle();
            if (Hwnd)
            {
                SetWindowTextW(Hwnd, *NewTitle);
            }
        }
    }
#endif
#endif
}
