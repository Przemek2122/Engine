// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ColorRGBA.h"
#include "Vector2D.h"

static Uint8 ColorRangeMinDefault = 0;
static Uint8 ColorRangeMaxDefault = 255;

FColorRGBA::FColorRGBA()
	: R(0)
	, G(0)
	, B(0)
	, A(255)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

FColorRGBA::FColorRGBA(const Uint8 In)
	: R(In)
	, G(In)
	, B(In)
	, A(255)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

FColorRGBA::FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB)
	: R(InR)
	, G(InG)
	, B(InB)
	, A(255)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

FColorRGBA::FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB, const Uint8 InA)
	: R(InR)
	, G(InG)
	, B(InB)
	, A(InA)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

FColorRGBA::FColorRGBA(const Uint8 InR, const Uint8 InG, const Uint8 InB, const Uint8 InA, const Uint8 InColorRangeMin, const Uint8 InColorRangeMax) 
	: R(InR)
	, G(InG)
	, B(InB)
	, A(InA)
	, ColorRangeMin(InColorRangeMin)
	, ColorRangeMax(InColorRangeMax)
{
}

FColorRGBA::FColorRGBA(FVector2D<int>& OldVector)
	: R(OldVector.X)
	, G(OldVector.Y)
	, B(0)
	, A(0)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

FColorRGBA::FColorRGBA(FVector2D<float>& OldVector)
	: R(static_cast<Uint8>(round(OldVector.X)))
	, G(static_cast<Uint8>(round(OldVector.Y)))
	, B(0)
	, A(0)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{ 
}

FColorRGBA::FColorRGBA(FVector2D<double>& OldVector)
	: R(static_cast<Uint8>(roundl(OldVector.X)))
	, G(static_cast<Uint8>(roundl(OldVector.Y)))
	, B(0)
	, A(0)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{ 
}

FColorRGBA::FColorRGBA(const FColorRGBA& OtherColor)
	: R(OtherColor.R)
	, G(OtherColor.G)
	, B(OtherColor.B)
	, A(OtherColor.A)
	, ColorRangeMin(ColorRangeMinDefault)
	, ColorRangeMax(ColorRangeMaxDefault)
{
}

void FColorRGBA::SetColorRange(const Uint8 InColorRangeMin, const Uint8 InColorRangeMax)
{
	ColorRangeMin = InColorRangeMin;
	ColorRangeMax = InColorRangeMax;
}

void FColorRGBA::NormalizeValues()
{
	R = std::clamp(R, ColorRangeMin, ColorRangeMax);
	G = std::clamp(G, ColorRangeMin, ColorRangeMax);
	B = std::clamp(B, ColorRangeMin, ColorRangeMax);
	A = std::clamp(A, ColorRangeMin, ColorRangeMax);
}

FColorRGBA& FColorRGBA::operator=(const FColorRGBA& OldColor)
{
	R = OldColor.R;
	G = OldColor.G;
	B = OldColor.B;
	A = OldColor.A;
		
	return *this;
}

FColorRGBA::operator SDL_Color() const
{
	SDL_Color Rect;

	Rect.r = R;
	Rect.g = G;
	Rect.b = B;
	Rect.a = A;

	return Rect;
}

bool FColorRGBA::operator==(const FColorRGBA& OldColor) const
{
	return (R == OldColor.R && G == OldColor.G && B == OldColor.B && A == OldColor.A);
}

FColorRGBA::operator std::ostream&() const
{
	return std::ostringstream().flush() << "(" + std::to_string(R) + ", " + std::to_string(R) + ")";
}

FColorRGBA::operator std::string() const 
{ 
	return "(" + std::to_string(R) + ", " + std::to_string(R) + ")"; 
}

FColorRGBA FColorRGBA::ColorWhite()
{
	static const FColorRGBA COLOR_WHITE_STATIC = FColorRGBA(255, 255, 255);

	return COLOR_WHITE_STATIC;
}

FColorRGBA FColorRGBA::ColorBlack()
{
	static const FColorRGBA COLOR_BLACk_STATIC = FColorRGBA(0, 0, 0);

	return COLOR_BLACk_STATIC;
}

FColorRGBA FColorRGBA::ColorLightGray()
{
	static const FColorRGBA COLOR_LIGHT_GRAY_STATIC = FColorRGBA(100, 100, 100);

	return COLOR_LIGHT_GRAY_STATIC;
}

FColorRGBA FColorRGBA::ColorGray()
{
	static const FColorRGBA COLOR_GRAY_STATIC = FColorRGBA(70, 70, 70);

	return COLOR_GRAY_STATIC;
}

FColorRGBA FColorRGBA::ColorDarkGray()
{
	static const FColorRGBA COLOR_DARK_GRAY_STATIC = FColorRGBA(40, 40, 40);

	return COLOR_DARK_GRAY_STATIC;
}

FColorRGBA FColorRGBA::ColorDodgerBlue()
{
	static const FColorRGBA COLOR_DODGER_BLUE_STATIC = FColorRGBA(30, 144, 255);

	return COLOR_DODGER_BLUE_STATIC;
}

FColorRGBA FColorRGBA::ColorLightGreen()
{
	static const FColorRGBA COLOR_LIGHT_GREEN_STATIC = FColorRGBA(30, 255 ,144);

	return COLOR_LIGHT_GREEN_STATIC;
}

FColorRGBA FColorRGBA::ColorGreen()
{
	static const FColorRGBA COLOR_GREEN_STATIC = FColorRGBA(144, 255, 30);

	return COLOR_GREEN_STATIC;
}

FColorRGBA FColorRGBA::ColorPurple()
{
	static const FColorRGBA COLOR_PURPLE_STATIC = FColorRGBA(144, 30, 255);

	return COLOR_PURPLE_STATIC;
}

FColorRGBA FColorRGBA::ColorPink()
{
	static const FColorRGBA COLOR_PINK_STATIC = FColorRGBA(255, 30, 144);

	return COLOR_PINK_STATIC;
}

FColorRGBA FColorRGBA::ColorYellow()
{
	static const FColorRGBA COLOR_YELLOW_STATIC = FColorRGBA(255, 255, 102);

	return COLOR_YELLOW_STATIC;
}

FColorRGBA FColorRGBA::ColorOrange()
{
	static const FColorRGBA COLOR_ORANGE_STATIC = FColorRGBA(255, 144, 30);

	return COLOR_ORANGE_STATIC;
}

FColorRGBA FColorRGBA::ColorRed()
{
	static const FColorRGBA COLOR_RED_STATIC = FColorRGBA(204, 0, 0);

	return COLOR_RED_STATIC;
}
