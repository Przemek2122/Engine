// Created by https://www.linkedin.com/in/przemek2122/ 2025

#pragma once

#include "CoreMinimal.h"

/**
 * Anything that uses more complex types should be separated from Math lib
 */
namespace FMath
{
	template<typename TValue = float>
	float Distance2D(FVector2D<TValue> VectorA, FVector2D<TValue> VectorB)
	{
		// Just a^2 + b^2 = c^2
		const int DistanceX = (VectorB.X - VectorA.X) * (VectorB.X - VectorA.X);
		const int DistanceY = (VectorB.Y - VectorA.Y) * (VectorB.Y - VectorA.Y);

		// Then just square root
		return static_cast<float>(sqrt(DistanceX + DistanceY));
	}

	FVector2D<int> GetRectCenter(const SDL_Rect Rect);
	FVector2D<int> GetLocationCenter(const FVector2D<int>& InLocation, const FVector2D<int>& InSize);

	/** @Returns radians angle to look at @To */
	template <typename TType>
	float FindLookAtRotationInRadians(const FVector2D<TType>& From, const FVector2D<TType>& LookAtTarget)
	{
		// Calculate the direction vector from A to B 
		FVector2D<TType> Direction = LookAtTarget - From;

		// Normalize the direction vector 
		FVector2D<float> NormalizedDirection = Direction.Normalize();

		// Calculate the angle in radians 
		float AngleRadians = std::atan2(NormalizedDirection.Y, NormalizedDirection.X);

		return AngleRadians;
	}

	/** @Returns radians angle to look at LookAtTarget. Performs automatic conversion to degrees */
	template <typename TType>
	float FindLookAtRotationInDegrees(const FVector2D<TType> From, const FVector2D<TType> LookAtTarget)
	{
		// @TODO HACK
		static constexpr float RotationOffset = 90.f;

		return FMath::ClampAngle(RadiansToDegrees(FindLookAtRotationInRadians(From, LookAtTarget)) + RotationOffset);
	}

	/* Returns radians */
	double Get2DAngleOfPointRadians(FVector2D<> A, FVector2D<> B);

	/* Expects radians not degree */
	void RotatePointAroundPoint(const FVector2D<int>& Pivot, const float& Angle, FVector2D<int>& Point);

	template<typename TType = float>
	TType MapRange(TType Value, const FVector2D<TType>& InRange, const FVector2D<TType>& OutRange)
	{
		return (Value - InRange.X) * (OutRange.Y - OutRange.X) / (InRange.Y - InRange.X) + OutRange.X;
	}

};
