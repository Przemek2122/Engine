// Created by Przemys³aw Wiewióra 2025

#pragma once

#include "CoreMinimal.h"

/**
 * Math library
 */
namespace FMath
{
	constexpr float AngleMin = 0.f;
	constexpr float AngleMax = 360.f;

	double Cos(const double AngleRadians);

	double Sin(const double AngleRadians);

	int FloorToInt(const float Value);
	int FloorToInt(const double Value);

	int RoundToInt(const float Value);
	int RoundToInt(const double Value);

	int CeilToInt(const float Value);
	int CeilToInt(const double Value);

	/** NOT THREAD SAFE. @return random float value in given range */
	float RandRange(const float Min, const float Max);
	int RandRange(const int Min, const int Max);

	double RadiansToDegrees(double Value);
	double RadiansToDegreesDouble(double Value);

	double DegreesToRadians(double Value);
	double DegreesToRadiansDouble(double Value);

	template<typename TType>
	TType Clamp(const TType Value, const TType Min,  const TType Max)
	{
		if (Value < Min)
		{
			return Min;
		}
		else if (Value > Max)
		{
			return Max;
		}
		else
		{
			return Value;
		}
	}

	template<typename TType>
	float ClampAngle(const TType Value)
	{
		float FinalValue = Value;

		if (FinalValue < AngleMin)
		{
			FinalValue += AngleMax;
		}
		else if (FinalValue > AngleMax)
		{
			FinalValue -= AngleMax;
		}

		return FinalValue;
	}

	/** @returns bigger value of two given */
	template<typename TType>
	TType Max(const TType A, const TType B)
	{
		return (A > B) ? A : B;
	}

	/** @returns lower value of two given */
	template<typename TType>
	TType Min(const TType A, const TType B)
	{
		return (A < B) ? A : B;
	}

	/** @returns absolute value (non negative) */
	template<typename TType>
	TType Abs(const TType Value)
	{
		return (Value < 0) ? -Value : Value;
	}

	/** Square root */
	template<typename TType>
	TType Sqrt(const TType Value)
	{
		return static_cast<TType>(sqrt(Value));
	}

	/** Returns power of given value */
	template<typename TType>
	TType Power(const TType Value)
	{
		return (Value * Value);
	}

	template<typename TType = float>
	bool IsNearlyEqual(TType A, TType B, float Tolerance = 0.0001f)
	{
		return (Abs(A - B) < Tolerance);
	}

};
