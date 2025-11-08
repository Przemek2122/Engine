// Created by https://www.linkedin.com/in/przemek2122/ 2025

#include "CoreEngine.h"
#include "Misc/VectorMath.h"

FVector2D<int> FMath::GetRectCenter(const SDL_Rect Rect)
{
	return {
		Rect.x + (Rect.w / 2),
		Rect.y + (Rect.h / 2)
	};
}

FVector2D<int> FMath::GetLocationCenter(const FVector2D<int>& InLocation, const FVector2D<int>& InSize)
{
	return InLocation - (InSize / 2);
}

double FMath::Get2DAngleOfPointRadians(const FVector2D<> A, const FVector2D<> B)
{
	return atan2(A.Y - B.Y, A.X - B.X);
}

void FMath::RotatePointAroundPoint(const FVector2D<int>& Pivot, const float& Angle, FVector2D<int>& Point)
{
	const float Sinus = FMath::Sin(Angle);
	const float Cosinus = FMath::Cos(Angle);

	// translate point back to origin:
	Point.X -= Pivot.X;
	Point.Y -= Pivot.Y;

	// rotate point
	const float NewX = static_cast<float>(Point.X) * Cosinus - static_cast<float>(Point.Y) * Sinus;
	const float NewY = static_cast<float>(Point.X) * Sinus + static_cast<float>(Point.Y) * Cosinus;

	// translate point back:
	Point.X = RoundToInt(NewX + static_cast<float>(Pivot.X));
	Point.Y = RoundToInt(NewY + static_cast<float>(Pivot.Y));
}

