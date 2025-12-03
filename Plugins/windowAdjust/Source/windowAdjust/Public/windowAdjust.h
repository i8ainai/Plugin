#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "WindowAdjust.generated.h"

UCLASS()
/*WINDOWADJUST_API:模块导出宏，用于控制类或函数在 不同编译单元之间的可见性
* UBlueprintFunctionLibrary 是 UE 提供的一个特殊基类，专门用来定义可以被蓝图直接调用的“静态函数库”。
继承自 UBlueprintFunctionLibrary 的类可以：
向蓝图系统暴露静态函数（static）；
无需实例化对象即可调用（类似 C++ 的工具类）；
可以在任何蓝图中直接使用这些函数节点
*/
class WINDOWADJUST_API UWindowAdjust : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // 修改主窗口标题
    UFUNCTION(BlueprintCallable, Category = "WindowAdjust")
    static void SetWindowTitle(const FString& NewTitle);

    // 修改主窗口尺寸（像素）
    UFUNCTION(BlueprintCallable, Category = "WindowAdjust")
    static void SetWindowSize(int32 Width, int32 Height);

    // 修改主窗口位置（左上角坐标）
    UFUNCTION(BlueprintCallable, Category = "WindowAdjust")
    static void SetWindowPosition(int32 X, int32 Y);

};
