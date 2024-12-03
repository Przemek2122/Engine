// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include <sstream>

/**
 * Color type
 */
class FColorRGBA
{
public:
	FColorRGBA();
	
	/**
	 * Initialize R, G, B with In,
	 * A(1),
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(const Uint8 In);

	/**
	 * Initialize R, G, B,
	 * A(1),
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB);
	
	/**
	 * Initialize R, G, B, A,
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB, const Uint8 InA);

	/** Full initializer */
	FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB, const Uint8 InA, const Uint8 InColorRangeMin, const Uint8 InColorRangeMax);
	
	/**
	 * Initialize from FVector
	 * R = X, G = Y,
	 * B(0), A(1),
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(FVector2D<int>& OldVector);
	
	/**
	 * Initialize from FVector
	 * R = X, G = Y,
	 * B(0), A(1),
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(FVector2D<float>& OldVector);
	
	/**
	 * Initialize from FVector
	 * R = X, G = Y,
	 * B(0), A(1),
	 * ColorRangeMin(0.f), ColorRangeMax(1.f)
	 */
	FColorRGBA(FVector2D<double>& OldVector);

	/** Initialize this by other object of FColorRGBA */
	FColorRGBA(const FColorRGBA& OtherColor);	

	void SetColorRange(const Uint8 InColorRangeMin = 0, const Uint8 InColorRangeMax = 255);

	/** Clamps values to be between min and max of 'ColorRange'. */
	void NormalizeValues();
	
	/** Comparison */
	FColorRGBA& operator=(const FColorRGBA& OldColor);
	bool operator==(const FColorRGBA& OldColor) const;

	/** Conversion */
	operator SDL_Color() const;
	explicit operator std::ostream&() const;
	explicit operator std::string() const;

public:
	Uint8 R;
	Uint8 G;
	Uint8 B;
	Uint8 A;

protected:
	Uint8 ColorRangeMin;
	Uint8 ColorRangeMax;

public:
	/** Begin of static predefined colors */
	static FColorRGBA ColorWhite();
	static FColorRGBA ColorBlack();
	static FColorRGBA ColorLightGray();
	static FColorRGBA ColorGray();
	static FColorRGBA ColorDarkGray();
	static FColorRGBA ColorDodgerBlue();
	static FColorRGBA ColorLightGreen();
	static FColorRGBA ColorGreen();
	static FColorRGBA ColorPurple();
	static FColorRGBA ColorPink();
	static FColorRGBA ColorYellow();
	static FColorRGBA ColorOrange();
	static FColorRGBA ColorRed();
	/** End of predefined colors */
};
