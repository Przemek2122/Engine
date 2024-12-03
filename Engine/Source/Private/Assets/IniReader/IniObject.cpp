#include "CoreEngine.h"
#include "Assets/IniReader/IniObject.h"

#include "Assets/Assets/IniAsset.h"
#include "Assets/IniReader/IniManager.h"
#include "Misc/StringHelpers.h"

FIniField::FIniField(std::string InName, std::string InValue)
	: Name(std::move(InName))
	, Value(std::move(InValue))
{
}

std::string FIniField::GetName() const
{
	return Name;
}

std::string FIniField::GetValueAsString() const
{
	return Value;
}

int FIniField::GetValueAsInt() const
{
	return SDL_atoi(Value.c_str());
}

bool FIniField::GetValueAsBool() const
{
	return FStringHelpers::ToBoolValue(Value);
}

bool FIniField::IsValid() const
{
	return (!Name.empty() && !Value.empty());
}

FIniObject::FIniObject(FIniManager* InIniManager, std::string InIniPath, std::string InIniName)
	: IniManager(InIniManager)
	, IniPath(std::move(InIniPath))
	, IniName(std::move(InIniName))
{
	// IniManager must not be nullptr
	ENSURE_VALID(InIniManager != nullptr);
}

FIniObject::~FIniObject()
{
	FieldsMap.Clear();
}

void FIniObject::Initialize()
{
	FAssetsManager* AssetsManager = IniManager->GetAssetsManager();
	IniAssetSharedPtr = AssetsManager->CreateAssetFromRelativePath<FIniAsset>(IniName, IniPath);
}

bool FIniObject::DoesIniExist() const
{
	return (IniAssetSharedPtr != nullptr && IniAssetSharedPtr->DoesFileExist());
}

void FIniObject::LoadIni()
{
	// Load disk data
	IniAssetSharedPtr->Load(IniManager->GetIniParser());

	const CArray<FParserLine>& Lines = IniAssetSharedPtr->GetLines();
	for (const FParserLine& Line : Lines)
	{
		std::string IniKey, IniValue;

		for (const FParserText& Text : Line.Texts)
		{
			if (Text.Type == EParserTextType::Word)
			{
				if (IniKey.empty())
				{
					IniKey = Text.Text;
				}
				else if (Text.Text != "=" && IniValue.empty())
				{
					IniValue = Text.Text;
				}
			}
		}

		FIniField IniField(IniKey, IniValue);
		if (IniField.IsValid())
		{
			FieldsMap.Emplace(IniKey, std::make_shared<FIniField>(IniField));
		}
	}

	// Unloaded after loading ini
	IniAssetSharedPtr->UnLoad();
}

void FIniObject::SaveIni()
{


	// Save data to disk
	IniAssetSharedPtr->Save(IniManager->GetIniParser());
}

bool FIniObject::ContainsFieldByName(const std::string& FieldName) const
{
	return FieldsMap.ContainsKey(FieldName);
}

bool FIniObject::ContainsFieldByValue(const std::string& FieldValue) const
{
	const FFunctorLambda<bool, std::string, std::shared_ptr<FIniField>> SearchFunction = [&](std::string Key, std::shared_ptr<FIniField> Field)
	{
		if (FieldValue == Field->GetValueAsString())
		{
			return true;
		}

		return false;
	};

	const bool bHasValue = FieldsMap.ContainsByPredicate(SearchFunction);

	return bHasValue;
}

FIniField FIniObject::FindFieldByName(const std::string& FieldName)
{
	FIniField Value;

	for (const std::pair<const std::string, std::shared_ptr<FIniField>>& Field : FieldsMap)
	{
		if (Field.first == FieldName)
		{
			Value = *Field.second;

			break;
		}
	}

	return Value;
}

FIniField FIniObject::FindFieldByValue(const std::string& FieldValue)
{
	FIniField Value;

	for (const std::pair<const std::string, std::shared_ptr<FIniField>>& Field : FieldsMap)
	{
		if (Field.second->GetValueAsString() == FieldValue)
		{
			Value = *Field.second;

			break;
		}
	}

	return Value;
}

void FIniObject::AddOrUpdateField(const FIniField& IniField)
{
	const std::string FieldName = IniField.GetName();
	if (!FieldName.empty())
	{
		if (ContainsFieldByName(FieldName))
		{
			FieldsMap[FieldName] = std::make_shared<FIniField>(IniField);
		}
		else
		{
			FieldsMap.Emplace(FieldName, std::make_shared<FIniField>(IniField));
		}
	}
}
