// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/Collision/SquareCollision.h"

void FRectangle::SetLocationTopLeftCorner(const FVector2D<int>& InPositionTopLeft)
{
	PositionTopLeft = InPositionTopLeft;

	UpdatePositionBottomRight();
}

void FRectangle::SetSize(const FVector2D<int>& InSize)
{
	Size = InSize;
}

const FVector2D<int>& FRectangle::GetPositionTopLeft() const
{
	return PositionTopLeft;
}

const FVector2D<int>& FRectangle::GetPositionBottomRight() const
{
	return PositionBottomRight;
}

const FVector2D<int>& FRectangle::GetSize() const
{
	return Size;
}

void FRectangle::UpdatePositionBottomRight()
{
	PositionBottomRight = PositionTopLeft + Size;
}

void FRectangleWithDiagonal::SetSize(const FVector2D<int>& InSize)
{
	Super::SetSize(InSize);

	UpdateDiagonalSize();
}

int FRectangleWithDiagonal::GetDiagonalSize() const
{
	return DiagonalSize;
}

void FRectangleWithDiagonal::UpdateDiagonalSize()
{
	DiagonalSize = FMath::Sqrt(FMath::Power(Size.X) + FMath::Power(Size.Y));
}

FSquareCollision::FSquareCollision(UCollisionComponent* InCollisionComponent, const FVector2D<int> InLocation, const FVector2D<int> InSize)
	: FCollisionBase(InCollisionComponent)
	, SquareData(InLocation, InSize)
{
	CollisionType = ECollisionType::Square;
}

int FSquareCollision::GetBaseExtentRadius()
{
	return (SquareData.GetDiagonalSize() / 2);
}

const FRectangleWithDiagonal& FSquareCollision::GetSquareData() const
{
	return SquareData;
}

FRectangleWithDiagonal& FSquareCollision::GetSquareDataForEdit()
{
	return SquareData;
}

int FSquareCollision::GetDiagonalSize() const
{
	return SquareData.GetDiagonalSize();
}
