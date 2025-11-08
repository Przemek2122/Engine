// Created by https://www.linkedin.com/in/przemek2122/ 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/ImageWidget.h"

#include "Assets/AssetsManagerHelpers.h"
#include "Assets/Assets/TextureAsset.h"

FImageWidget::FImageWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, bScaleWidgetToImage(false)
	, TextureAsset(nullptr)
{
#if WIDGET_DEBUG_COLORS
	// Image widget does not use colors debug because it has image
	SetDebugWidgetColorsEnabled(false);
#endif
}

void FImageWidget::Init()
{
	FWidget::Init();

	SetWidgetSize({ 40, 40 });
}

void FImageWidget::PreDeInit()
{
	FWidget::PreDeInit();

	if (TextureAsset != nullptr)
	{
		TextureAsset->DecrementNumberOfReferences();
		TextureAsset = nullptr;
	}
}

void FImageWidget::Render()
{
	if (TextureAsset != nullptr)
	{
		GetRenderer()->DrawTexture(TextureAsset, GetWidgetLocation(), GetWidgetSize(), false);
	}

	FWidget::Render();
}

void FImageWidget::SetImage(const std::string& InImageName, const std::string& OptionalPath)
{
	FAssetsManager* AssetsManager = FGlobalDefines::GEngine->GetAssetsManager();
	if (AssetsManager != nullptr)
	{
		FTextureAsset* TemporaryTextureAsset = FAssetsManagerHelpers::GetOrCreateAsset<FTextureAsset>(AssetsManager, GetOwnerWindow(), InImageName, OptionalPath, EAssetType::AT_TEXTURE);

		if (TemporaryTextureAsset != nullptr)
		{
			SetImage(TemporaryTextureAsset);
		}
		else
		{
			LOG_ERROR("Unable to find texture asset: " << InImageName);
		}
	}
}

void FImageWidget::SetImage(const std::shared_ptr<FTextureAsset>& TexturePtr)
{
	if (TexturePtr != nullptr)
	{
		SetImage(TexturePtr.get());

		OptionalTexturePtr = TexturePtr;
	}
}

void FImageWidget::SetImage(FTextureAsset* NewTexture)
{
	if (NewTexture != nullptr)
	{
		if (OptionalTexturePtr != nullptr)
		{
			OptionalTexturePtr = nullptr;
		}

		if (TextureAsset != nullptr)
		{
			TextureAsset->DecrementNumberOfReferences();
			TextureAsset = nullptr;
		}

		// Set new one
		TextureAsset = NewTexture;
		TextureAsset->IncrementNumberOfReferences();

		if (bScaleWidgetToImage)
		{
			ScaleWidgetToTextureSize();
		}
	}
	else
	{
		LOG_ERROR("Texture 'InAsset' is nullptr.");
	}
}

void FImageWidget::SetScaleWidgetToImage(const bool bInScaleWidgetToImage)
{
	bScaleWidgetToImage = bInScaleWidgetToImage;

	if (bScaleWidgetToImage && TextureAsset != nullptr)
	{
		ScaleWidgetToTextureSize();
	}
}

void FImageWidget::ScaleWidgetToTextureSize()
{
	const FVector2D<int> TextureSize = TextureAsset->GetSize();

	SetWidgetSize(TextureSize);
}

void FImageWidget::ClearPreviousImage()
{
	OptionalTexturePtr = nullptr;
}