// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

class FTextureAsset;

struct FColorPoint
{
	FColorPoint();
	FColorPoint(const FVector2D<float> InLocation, const FColorRGBA& InColor);

	FVector2D<float> Location;
	FColorRGBA Color;
};

/**
 * Base Renderer class.
 * Has SDL_Renderer.
 * Should only be created from FWindow class. 
 */
class FRenderer
{
friend FWindow;

protected:
	FRenderer(FWindow* InWindow);
	virtual ~FRenderer();
	
	/** Before render - Clear scene */
	virtual void PreRender();

	/** Gather render data */
	virtual void Render();

	/** After render - do render */
	virtual void PostRender();

	virtual void PaintDefaultBackground();

	virtual void Repaint();
	
	virtual void RepaintWindow();

	virtual void MarkNeedsRepaint();

public:
	/** Raw SDL Renderer */
	_NODISCARD INLINE_DEBUGABLE SDL_Renderer* GetSDLRenderer() const { return Renderer; }

	/** Raw SDL Window */
	_NODISCARD INLINE_DEBUGABLE SDL_Window* GetSdlWindow() const;

	/** Raw SDL Window surface */
	_NODISCARD INLINE_DEBUGABLE SDL_Surface* GetSdlWindowSurface() const;

	/** @returns cached window size */
	_NODISCARD INLINE_DEBUGABLE FVector2D<int> GetWindowSize() const;

	void SetWindowSize(const int X, const int Y, const bool bUpdateSDL = true) const;

	void DrawTexture(const FTextureAsset* Texture,	FVector2D<float> Location, const FVector2D<float> Size, const bool bIsLocationRelative = true) const;

	void DrawTexture(SDL_Texture* Texture,			FVector2D<float> Location, const FVector2D<float> Size, const bool bIsLocationRelative = true) const;

	void DrawTextureAdvanced(const FTextureAsset* Texture,	FVector2D<float> Location, const FVector2D<float> Size, const double Rotation,
		const FVector2D<float> CenterOfRotation = FVector2D<float>(), SDL_FlipMode Flip = SDL_FLIP_NONE, const bool bIsLocationRelative = true) const;

	void DrawTextureAdvanced(SDL_Texture* Texture,			FVector2D<float> Location, const FVector2D<float> Size, const double Rotation,
		const FVector2D<float> CenterOfRotation = FVector2D<float>(), SDL_FlipMode Flip = SDL_FLIP_NONE, const bool bIsLocationRelative = true) const;

	static void OverrideTextureColor(SDL_Texture* Texture, const FColorRGBA& Color);
	static void OverrideTextureColorReset(SDL_Texture* Texture);

	/** Draw single point. Relative means to move with map */
	void DrawPointAtRelative(const FColorPoint& ColorPoint) const;

	/** Draw single point. Absolute means never move. Example: UI */
	void DrawPointAtAbsolute(const FColorPoint& ColorPoint) const;

	/** Draw multiple points with same colors. */
	void DrawPointsAt(const CArray<FVector2D<float>>& Points, const FColorRGBA& AllPointsColor, const bool bIsLocationRelative = true) const;

	/** Draw multiple points with same colors. */
	void DrawPointsAt(const CArray<SDL_FPoint>& Points, const FColorRGBA& AllPointsColor, const bool bIsLocationRelative = true) const;

	/** Draw single rectangle */
	void DrawRectangle(FVector2D<float> RectLocation, const FVector2D<float> RectSize, const FColorRGBA& InColor, const bool bIsLocationRelative = true) const;

	/** Draw single rectangle but without interior */
	void DrawRectangleOutline(FVector2D<float> RectLocation, const FVector2D<float> RectSize, const FColorRGBA& InColor, const bool bIsLocationRelative = true) const;
	
	void DrawCircle(FVector2D<int> Location, const int Radius, const bool bIsLocationRelative = true) const;

	void DrawLine(FVector2D<int> From, FVector2D<int> To, const bool bIsLocationRelative = true) const;

	/** Adds RenderOffset to rendered objects */
	FVector2D<float> ConvertLocationToScreenSpace(const FVector2D<float>& InLocation) const;

	/** Updates offset of rendering object. Usually called by map when moved. */
	void SetRenderOffset(const FVector2D<int>& NewRenderOffset);

	/** @returns render offset */
	FVector2D<int> GetRenderOffset() const { return RenderOffset; }

protected:
	/** Owner window pointer */
	FWindow* Window;

	/** Raw SDL Renderer pointer */
	SDL_Renderer* Renderer;

	/** Que of point to draw */
	CDeque<FColorPoint> PointsToDrawDeque;

	bool bNeedsRepaint;

	/**
	 * Render offset. This offset is used to move screen.
	 * @Note this offset only affects rendering.
	 * All other calculations should be done on default location without offset.
	 */
	FVector2D<int> RenderOffset;

};
