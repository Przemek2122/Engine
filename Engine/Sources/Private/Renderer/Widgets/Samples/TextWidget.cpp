// Created by Przemys�aw Wiewi�ra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/TextWidget.h"
#include "Assets/TypesForAssets/Font.h"
#include "Assets/Assets/FontAsset.h"

static const char* DefaultText = "Default text"; 
static const char* DefaultFont = "OpenSans";

int32 FFontStatics::GetFontSizeFromPixels(const int32 SizeInPixels)
{
	static float FontMultiplier = 0.75f;

	return static_cast<float>(SizeInPixels) * FontMultiplier;
}

int32 FFontStatics::GetPixelsFromFontSize(const int32 FontSize)
{
	static float FontMultiplier = 1.33f;

	return static_cast<float>(FontSize) * FontMultiplier;
}

FTextWidget::FTextWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, DesiredText(DefaultText)
	, RenderedText(DefaultText)
	, TextSize(16)
	, RenderedTextSize(0)
	, TextWidgetSizeType(ETextWidgetSizeType::ResizeFontToFit)
	, AssetsManager(FGlobalDefines::GEngine->GetAssetsManager())
	, FontAsset(nullptr)
	, TextRenderColor(255)
	, TextBackgroundRenderColor({ 255, 0, 0})
	, SDLRect(new SDL_FRect)
	, TextTexture(nullptr)
	, LastTextTextureSize({ 0, 0 })
	, CurrentTextRenderMode(ETextRenderMode::None)
	, DesiredTextRenderMode(ETextRenderMode::Blended)
{
	SetClippingMethod(EClipping::None);

    FontAsset = AssetsManager->GetAsset<FFontAsset>(DefaultFont);

#if _DEBUG
	// Unable to find font asset.
	ENSURE_VALID(FontAsset != nullptr);
#endif

	SetWidgetMargin(FWidgetMargin(10, 10, 8, 8 ));
}

FTextWidget::~FTextWidget()
{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
	SDL_DestroyTexture(TextTexture);
#endif

	delete SDLRect;
}

void FTextWidget::Render()
{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
	SDL_RenderTexture(GetRenderer()->GetSDLRenderer(), TextTexture, nullptr, SDLRect);
#endif

	FWidget::Render();
}

void FTextWidget::OnClippingMethodChanged(const EClipping NewClipping)
{
	FWidget::OnClippingMethodChanged(NewClipping);

	// We need to redraw because it might need to cut part of text
	OnTextChanged();
}

void FTextWidget::UpdateWidgetLocation()
{
	FWidget::UpdateWidgetLocation();

	const FVector2D<int32>& LocationCache = GetWidgetLocation(EWidgetOrientation::Absolute);

	SDLRect->x = LocationCache.X;
	SDLRect->y = LocationCache.Y;
}

void FTextWidget::UpdateWidgetSize(const bool bWasSentFromRebuild)
{
	FWidget::UpdateWidgetSize(bWasSentFromRebuild);

	const FVector2D<int32>& SizeCache = GetWidgetSize();

	SDLRect->w = SizeCache.X;
	SDLRect->h = SizeCache.Y;
}

void FTextWidget::UpdateAnchor(const bool bIsFromRebuild)
{
	FWidget::UpdateAnchor(bIsFromRebuild);

	const FVector2D<int32>& LocationCache = GetWidgetLocation(EWidgetOrientation::Absolute);
	const FVector2D<int32>& SizeCache = GetWidgetSize();

	SDLRect->x = LocationCache.X;
	SDLRect->y = LocationCache.Y;
	SDLRect->w = SizeCache.X;
	SDLRect->h = SizeCache.Y;
}

void FTextWidget::SetText(const std::string& InText)
{
	DesiredText = InText;

	OnTextChanged();
}

void FTextWidget::SetTextWidgetSizeType(const ETextWidgetSizeType InTextWidgetSizeType)
{
	TextWidgetSizeType = InTextWidgetSizeType;
}

std::string FTextWidget::GetDesiredText() const
{
	return DesiredText;
}

std::string FTextWidget::GetRenderedText() const
{
	return RenderedText;
}

void FTextWidget::RebuildWidget()
{
	FWidget::RebuildWidget();

	if (FontAsset != nullptr)
	{
		// Update text
		RedrawText();
	}
}

void FTextWidget::OnTextChanged()
{
	RequestWidgetRebuild();
}

void FTextWidget::AutoAdjustSize()
{
	switch (TextWidgetSizeType)
	{
		case ETextWidgetSizeType::None:
		{
			break;
		}
		case ETextWidgetSizeType::ClipWidgetSizeToText:
		{
			AutoAdjustSizeToText(GetClippingMethod() == EClipping::Cut);

			break;
		}
		case ETextWidgetSizeType::ResizeFontToFit:
		{
			// Find font size
			AutoAdjustFontToFit();

			break;
		}
	}
}

void FTextWidget::AutoAdjustSizeToText(const bool bLimitToParentSize)
{
	FVector2D<int32> NewWidgetSize;

	if (bLimitToParentSize)
	{
		AutoAdjustTextSize(GetParent()->GetWidgetManagerSize());
	}
	
	CalculateDefaultSizeForRenderText(NewWidgetSize);
	
	SetWidgetSize(NewWidgetSize);
}

bool FTextWidget::AutoAdjustFontToFit()
{
	FVector2D<int32> ParentSize = GetParent()->GetWidgetManagerSize();
	const int32 NewTextSize = FFontStatics::GetFontSizeFromPixels(ParentSize.Y);

	bool bWasChanged = false;

	if (NewTextSize != 0 && NewTextSize != TextSize)
	{
		TextSize = NewTextSize;

		bWasChanged = true;
	}

	return bWasChanged;
}

void FTextWidget::AutoAdjustTextSize(const FVector2D<int32>& InMaxSize)
{
	auto StringSize = RenderedText.length();

	FVector2D<int32> OutSize(0);

	CalculateDefaultSizeForRenderText(OutSize);
	
	while(OutSize.X > InMaxSize.X)
	{
		CalculateDefaultSizeForRenderText(OutSize);
		
		if (OutSize.X > InMaxSize.X)
		{
			RenderedText = RenderedText.substr(0, --StringSize);
		}
		else
		{
			break;
		}
	}
}

int32 FTextWidget::CalculateDefaultSizeForRenderText(FVector2D<int32>& InOutSize) const
{
	bool bIsRendering = false;

	TTF_Font* Font = FontAsset->GetFont(TextSize)->GetFont();
	if (Font != nullptr)
	{
#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
		bIsRendering = TTF_GetStringSize(Font, RenderedText.c_str(), 0, &InOutSize.X, &InOutSize.Y);
#endif
	}
	
	if (!bIsRendering)
	{
		LOG_ERROR("Text could not be rendered. TTF: '" << SDL_GetError() << "' SDL: '" << SDL_GetError() << "'");
	}

	return bIsRendering;
}

void FTextWidget::SetTextRenderMode(const ETextRenderMode NewTextRenderMode)
{
	DesiredTextRenderMode = NewTextRenderMode;

	// @TODO We could run single rebuild instead of rebuilding whole hierarchy as this change should not change size
	RequestWidgetRebuild();
}

ETextRenderMode FTextWidget::GetTextRenderMode() const
{
	return DesiredTextRenderMode;
}

void FTextWidget::RedrawText()
{
	const bool bIsInputDataCorrect = (FontAsset != nullptr && !DesiredText.empty());
	const bool bIsAnyDataChanged = (DesiredText != RenderedText || CurrentTextRenderMode != DesiredTextRenderMode || RenderedTextSize != TextSize);
	if (bIsInputDataCorrect && bIsAnyDataChanged)
	{
		RenderedText = DesiredText;

		AutoAdjustSize();

		SDL_Surface* SdlSurface = nullptr;
		TTF_Font* Font = FontAsset->GetFont(TextSize)->GetFont();
		if (Font != nullptr)
		{
			RenderedTextSize = TextSize;

#if defined(ENGINE_USING_VIDEO) && ENGINE_USING_VIDEO
			switch (DesiredTextRenderMode)
			{
				case ETextRenderMode::Solid:
				{
					SdlSurface = TTF_RenderText_Solid(Font, RenderedText.c_str(), 0, TextRenderColor);

					break;
				}
				case ETextRenderMode::Blended:
				{
					SdlSurface = TTF_RenderText_Blended(Font, RenderedText.c_str(), 0, TextRenderColor);

					break;
				}
				case ETextRenderMode::Shaded:
				{
					SdlSurface = TTF_RenderText_Shaded(Font, RenderedText.c_str(), 0, TextRenderColor, TextBackgroundRenderColor);

					break;
				}
				default: ;
			}
#endif

			CurrentTextRenderMode = DesiredTextRenderMode;
		}

		if (SdlSurface != nullptr)
		{
			SDL_LockSurface(SdlSurface); // Lock surface for safe pixel access

			FVector2D<int32> WidgetSize = GetWidgetSize();

			// If we have texture and X or Y size has changed and we need texture of different size
			if (TextTexture == nullptr || (WidgetSize.X != LastTextTextureSize.X || WidgetSize.Y != LastTextTextureSize.Y))
			{
				// Destroy old texture
				SDL_DestroyTexture(TextTexture);

				// Create new texture
				TextTexture = SDL_CreateTextureFromSurface(GetRenderer()->GetSDLRenderer(), SdlSurface);

				FVector2D<float> TempSizeOfTexture;
				SDL_GetTextureSize(TextTexture, &TempSizeOfTexture.X, &TempSizeOfTexture.Y);
				LastTextTextureSize = TempSizeOfTexture;
			}
			else
			{
				// If size not changed update old texture
				const bool bWasUpdateTextureSuccess = SDL_UpdateTexture(TextTexture, nullptr, SdlSurface->pixels, SdlSurface->pitch);

				if (!bWasUpdateTextureSuccess)
				{
					LOG_ERROR("SDL_UpdateTexture error: " << SDL_GetError());
				}
			}

			SDL_UnlockSurface(SdlSurface);
			SDL_DestroySurface(SdlSurface);

			// Update widget size	
			AutoAdjustSizeToText(GetClippingMethod() == EClipping::Cut);
		}
	}
	else
	{
		if (FontAsset == nullptr)
		{
			LOG_ERROR("FontAsset for Text widget is null");
		}

		if (DesiredText.empty())
		{
			LOG_ERROR("Desired text is empty, if you intend to hide widget, use SetVisibility.");
		}
	}
}
