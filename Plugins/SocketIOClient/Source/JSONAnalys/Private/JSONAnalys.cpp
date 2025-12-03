#include "JSONAnalys.h"
#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, JSONAnalys)

bool UJSONAnalys::ParseJSONKeys(const FString& JSONString, const TArray<FString>& Keys, TArray<FJSONKeyValue>& OutKeyValues)
{
	OutKeyValues.Empty();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);

	// 尝试解析JSON
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ParseJSONKeys: 无法解析JSON字符串"));
		return false;
	}

	// 遍历传入的Key数组
	for (const FString& Key : Keys)
	{
		FJSONKeyValue KV;
		KV.Key = Key;

		if (JsonObject->HasField(Key))
		{
			// 将值转换为字符串
			TSharedPtr<FJsonValue> Value = JsonObject->TryGetField(Key);
			if (Value.IsValid())
			{
				switch (Value->Type)
				{
				case EJson::String:
					KV.Value = Value->AsString();
					break;
				case EJson::Number:
					KV.Value = FString::SanitizeFloat(Value->AsNumber());
					break;
				case EJson::Boolean:
					KV.Value = Value->AsBool() ? TEXT("true") : TEXT("false");
					break;
				case EJson::Null:
					KV.Value = TEXT("null");
					break;
				case EJson::Array:
				case EJson::Object:
				default:
					KV.Value = TEXT("[Complex Type]");
					break;
				}
			}
			else
			{
				KV.Value = TEXT("");
			}
		}
		else
		{
			KV.Value = TEXT("");
		}

		OutKeyValues.Add(KV);
	}

	return true;
}
