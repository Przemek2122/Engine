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

class FTextWidget : public FWidget
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

	_NODISCARD std::string GetDesiredText() const;
	_NODISCARD std::string GetRenderedText() const;

	void SetTextRenderMode(ETextRenderMode NewTextRenderMode);
	_NODISCARD ETextRenderMode GetTextRenderMode() const;

protected:
	void RebuildWidget() override;

	/** Redraw text and auto calc size */
	virtual void OnTextChanged();

	/** Auto adjusts size for @RenderedText */
	void AutoAdjustSize(const bool bLimitToParentSize = false);

	/** Helper for AutoAdjustSize */
	virtual void AutoAdjustTextSize(const FVector2D<int>& InMaxSize);

	/** SDL Wrapper */
	int CalculateDefaultSizeForRenderText(FVector2D<int>& InOutSize) const;

	/** Makes new texture for text */
	void RedrawText();

protected:
	std::string DesiredText;
	std::string RenderedText;

	int TextSize;

	FAssetsManager* AssetsManager;
	FFontAsset* FontAsset;
	
	FColorRGBA TextRenderColor;
	FColorRGBA TextBackgroundRenderColor;

	SDL_FRect* SDLRect;
	SDL_Texture* TextTexture;

	FVector2D<int> LastTextTextureSize;

	ETextRenderMode CurrentTextRenderMode;
	ETextRenderMode DesiredTextRenderMode;

protected:
	/** if this property is set to true it will cut text to fit inside of parent */
	bool bAutoCutTextToFitInsideOfParent;
	
};
