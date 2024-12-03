// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "BaseCollision.h"

/**
 * Base rectangle class
 */
struct FRectangle
{
	FRectangle(const FVector2D<int> InPosition = FVector2D(), const FVector2D<int> InSize = FVector2D())
		: PositionTopLeft(InPosition), Size(InSize)
	{
		UpdatePositionBottomRight();
	}
	virtual ~FRectangle() = default;

	virtual void SetLocationTopLeftCorner(const FVector2D<int>& InPositionTopLeft);
	virtual void SetSize(const FVector2D<int>& InSize);

	const FVector2D<int>& GetPositionTopLeft() const;
	const FVector2D<int>& GetPositionBottomRight() const;
	const FVector2D<int>& GetSize() const;

protected:
	void UpdatePositionBottomRight();

	FVector2D<int> PositionTopLeft;			// Top-left corner of the rectangle 
	FVector2D<int> PositionBottomRight;		// Top-left corner of the rectangle 
	FVector2D<int> Size;					// Width (X) and Height (Y)
};

struct FRectangleWithDiagonal : public FRectangle
{
public:
	FRectangleWithDiagonal(const FVector2D<int>& InLocation, const FVector2D<int>& InSize)
		: FRectangle(InLocation, InSize)
	{
		UpdateDiagonalSize();
	}

	void SetSize(const FVector2D<int>& InSize) override;

	/** Calculated diagonal of square */
	int GetDiagonalSize() const;

	void UpdateDiagonalSize();

protected:
	/** Calculated diagonal of square  */
	int DiagonalSize;
};

/**
 * Basic AABB Collision
 */
class FSquareCollision : public FCollisionBase
{
public:
	FSquareCollision(UCollisionComponent* InCollisionComponent, const FVector2D<int> InLocation, const FVector2D<int> InSize);

	int GetBaseExtentRadius() override;

	const FRectangleWithDiagonal& GetSquareData() const;
	FRectangleWithDiagonal& GetSquareDataForEdit();

	int GetDiagonalSize() const;

protected:
	FRectangleWithDiagonal SquareData;

};
