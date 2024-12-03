#pragma once

#include "CoreMinimal.h"

/** Structure for generating parent widget using children */
struct FWidgetGeometry
{
	/** Widget size */
	FVector2D<int32> Size;
};

/** Base class for padding and margin */
struct FBoxSpacing
{
protected:
	/** Will set given padding to all 4 sides */
	FBoxSpacing(const int32 Margin)
		: Left(Margin)
		, Right(Margin)
		, Top(Margin)
		, Bottom(Margin)
	{
	}

	FBoxSpacing(const int32 InLeft, const int32 InRight, const int32 InTop, const int32 InBottom)
		: Left(InLeft)
		, Right(InRight)
		, Top(InTop)
		, Bottom(InBottom)
	{
	}

public:
	/** Sets left and right by value with (InHorizontal / 2); */
	void SetHorizontal(const int32 InHorizontal)
	{
		const int32 Half = InHorizontal / 2;

		Left = Half;
		Right = Half;
	}
	/** Sets left and right by value with (InVertical / 2); */
	void SetVertical(const int32 InVertical)
	{
		const int32 Half = InVertical / 2;

		Top = Half;
		Bottom = Half;
	}

	FVector2D<int32> Get() const { return { GetHorizontal(), GetVertical() }; }

	INLINE_DEBUGABLE int32 GetHorizontal() const { return (Left + Right); }
	INLINE_DEBUGABLE int32 GetVertical() const { return (Bottom + Top); }

	INLINE_DEBUGABLE int32 GetLeft() const { return Left; }
	INLINE_DEBUGABLE int32 GetRight() const { return Right; }
	INLINE_DEBUGABLE int32 GetTop() const { return Top; }
	INLINE_DEBUGABLE int32 GetBottom() const { return Bottom; }

protected:
	int32 Left;
	int32 Right;
	int32 Top;
	int32 Bottom;
	
};

/** Margin is for child widget to set size for parent */
struct FWidgetMargin : FBoxSpacing
{
	FWidgetMargin(const int32 Margin = 0)
		: FBoxSpacing(Margin)
	{
	}

	FWidgetMargin(const int32 InLeft, const int32 InRight, const int32 InTop, const int32 InBottom)
		: FBoxSpacing(InLeft, InRight, InTop, InBottom)
	{
	}
};

/** Margin is for internal widget size */
struct FWidgetPadding : FBoxSpacing
{
	FWidgetPadding(const int32 Margin = 0)
		: FBoxSpacing(Margin)
	{
	}

	FWidgetPadding(const int32 InLeft, const int32 InRight, const int32 InTop, const int32 InBottom)
		: FBoxSpacing(InLeft, InRight, InTop, InBottom)
	{
	}
};

/** Internal child widget padding - Calculated by margin of parent */
struct FWidgetPaddingInternal : FBoxSpacing
{
	FWidgetPaddingInternal(const int32 Margin = 0)
		: FBoxSpacing(Margin)
	{
	}

	FWidgetPaddingInternal(const int32 InLeft, const int32 InRight, const int32 InTop, const int32 InBottom)
		: FBoxSpacing(InLeft, InRight, InTop, InBottom)
	{
	}
};
