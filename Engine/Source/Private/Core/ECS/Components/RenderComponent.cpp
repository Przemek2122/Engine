// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Components/RenderComponent.h"

#include "Assets/AssetsManagerHelpers.h"
#include "Assets/Assets/TextureAsset.h"
#include "Assets/Collection/AssetCollectionItem.h"

URenderComponent::URenderComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UComponent(InComponentManagerInterface)
	, TextureAsset(nullptr)
	, CurrentRenderCenterType(ERenderCenterType::RotateAround)
	, CurrentRenderType(ERenderType::Center)
{
}

void URenderComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set default size
	SetImageSize(GetSize());
}

void URenderComponent::EndPlay()
{
	Super::EndPlay();

	if (TextureAsset != nullptr)
	{
		TextureAsset->DecrementNumberOfReferences();
		TextureAsset = nullptr;
	}
}

void URenderComponent::Render()
{
	Super::Render();

	if (TextureAsset != nullptr)
	{
		FVector2D<int> RenderLocation;
 
		switch (CurrentRenderType)
		{
			case ERenderType::Center:
			{
				RenderLocation = GetLocationCenter();

				break;
			}

			case ERenderType::AbsoluteLocation:
			{
				RenderLocation = GetAbsoluteLocation();

				break;
			}
		}

		FVector2D<int> PivotLocationCenter;

		switch (CurrentRenderCenterType)
		{
			case ERenderCenterType::RotateAround:
			{
				PivotLocationCenter = GetSize() / 2;

				break;
			}

			case ERenderCenterType::AtPivotPoint:
			{
				// Do nothing we want to rotate at zero

				break;
			}
		}

		GetOwnerWindow()->GetRenderer()->DrawTextureAdvanced(
			TextureAsset, 
			RenderLocation,
			GetSize(), 
			GetAbsoluteRotation(),
			PivotLocationCenter
		);
	}
}

void URenderComponent::SetImage(const FAssetCollectionItem& AssetCollectionItem)
{
	SetImage(AssetCollectionItem.GetAssetName(), AssetCollectionItem.GetAssetPath());
}

void URenderComponent::SetImage(const std::string& InImageName, const std::string& OptionalPath)
{
	if (!InImageName.empty())
	{
		FAssetsManager* AssetsManager = GEngine->GetAssetsManager();
		if (AssetsManager != nullptr)
		{
			FTextureAsset* TemporaryTexture = FAssetsManagerHelpers::GetOrCreateAsset<FTextureAsset>(AssetsManager, GetOwnerWindow(), InImageName, OptionalPath, EAssetType::AT_TEXTURE);

			if (TemporaryTexture != nullptr)
			{
				SetImage(TemporaryTexture);
			}
			else
			{
				LOG_ERROR("Asset: '" << InImageName << "' not found. If you would like to load it instead use OptionalPath parameter in URenderComponent::SetImage");
			}
		}
	}
	else
	{
		LOG_ERROR("Got empty @InImageName. We will not set an image");
	}
}

void URenderComponent::SetImage(FTextureAsset* InAsset)
{
	if (InAsset != nullptr)
	{
		if (TextureAsset != nullptr)
		{
			TextureAsset->DecrementNumberOfReferences();
			TextureAsset = nullptr; 
		}

		// Set new one
		TextureAsset = InAsset;
		TextureAsset->IncrementNumberOfReferences();

		SetImageSize(TextureAsset->GetSize());
	}
	else
	{
		LOG_ERROR("Texture 'InAsset' is nullptr.");
	}
}

void URenderComponent::SetImageSize(const FVector2D<int>& InSize)
{
	SetSize(InSize);
}

void URenderComponent::SetRenderLocationType(const ERenderType InRenderType)
{
	CurrentRenderType = InRenderType;
}

void URenderComponent::SetRenderCenterType(const ERenderCenterType InRenderCenterType)
{
	CurrentRenderCenterType = InRenderCenterType;
}
