// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "../Widget.h"

/** What should happen if widget is too big? */
enum class ETextRenderMode : Uint8
{
	None,						// Default value
	Solid,						// Render text only
	Shaded,						// Render text with background
	Blended,					// Render
};

/** How should we adjust size of this widget */
enum class ETextWidgetSizeType : Uint8
{
	/** Do nothing to widget size */
	None,
	/** check text size and set widget size to be exact size to fit */
	ClipWidgetSizeToText,
	/** Check Widget size and align size of font to fit vertically */
	ResizeFontToFit
};

class FFontStatics
{
public:
	static int32 GetFontSizeFromPixels(const int32 SizeInPixels);
	static int32 GetPixelsFromFontSize(const int32 FontSize);

};

class ENGINE_API FTextWidget : public FWidget
{
public:
	FTextWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);
	~FTextWidget() override;
	
	/** Begin FWidget */
	void Render() override;
	void OnClippingMethodChanged(EClipping NewClipping) override;
	/** End FWidget */

	/** Begin IWidgetPositionInterface */
	void UpdateWidgetLocation() override;
	void UpdateWidgetSize(const bool bWasSentFromRebuild) override;
	void UpdateAnchor(const bool bIsFromRebuild) override;
	/** End IWidgetPositionInterface */

public:
	/** Use for advanced text with parameters like InText='"Test button " << 1' */
#define SET_TEXT_ADV(InText) SetText(TEXT_ADV(InText))

	/**
	 * Use for simple string InText='"Some text ..."'
	 * You can use SetTextM("Test button " << 1 << SomeParameter)
	 * or std::format("{1} to {0}", "a", "b") for parameterized versions.
	 */
	void SetText(const std::string& InText);

	/** How should we change size of widget? */
	void SetTextWidgetSizeType(ETextWidgetSizeType InTextWidgetSizeType);

	NO_DISCARD std::string GetDesiredText() const;
	NO_DISCARD std::string GetRenderedText() const;

	void SetTextRenderMode(ETextRenderMode NewTextRenderMode);
	NO_DISCARD ETextRenderMode GetTextRenderMode() const;

protected:
	void RebuildWidget() override;

	/** Redraw text and auto calc size */
	virtual void OnTextChanged();

	/** Call to adjust size depending on settings */
	void AutoAdjustSize();

	/** Auto adjusts size for @RenderedText */
	void AutoAdjustSizeToText(const bool bLimitToParentSize = false);

	/** Auto adjust size @returns if font size has been changed */
	bool AutoAdjustFontToFit();

	/** Helper for AutoAdjustSizeToText */
	virtual void AutoAdjustTextSize(const FVector2D<int32>& InMaxSize);

	/** SDL Wrapper */
	int32 CalculateDefaultSizeForRenderText(FVector2D<int32>& InOutSize) const;

	/** Makes new texture for text */
	void RedrawText();

protected:
	std::string DesiredText;
	std::string RenderedText;

	int32 TextSize;
	int32 RenderedTextSize;

	/** How should we change size of widget? */
	ETextWidgetSizeType TextWidgetSizeType;

	FAssetsManager* AssetsManager;
	FFontAsset* FontAsset;
	
	FColorRGBA TextRenderColor;
	FColorRGBA TextBackgroundRenderColor;

	SDL_FRect* SDLRect;
	SDL_Texture* TextTexture;

	FVector2D<int32> LastTextTextureSize;

	ETextRenderMode CurrentTextRenderMode;
	ETextRenderMode DesiredTextRenderMode;
	
};
