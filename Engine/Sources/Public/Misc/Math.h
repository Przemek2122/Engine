// Created by https://www.linkedin.com/in/przemek2122/ 2025

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

	double RadiansToDegrees(double Value);
	double RadiansToDegreesDouble(double Value);

	double DegreesToRadians(double Value);
	double DegreesToRadiansDouble(double Value);

	/** @returns random float value in given range */
	template<typename TType>
	TType RandRange(const TType Min, const TType Max)
	{
		static thread_local std::mt19937 generator(std::random_device{}());

		if constexpr (std::is_integral_v<TType>)
		{
			std::uniform_int_distribution<TType> distribution(Min, Max);
			return distribution(generator);
		}
		else
		{
			std::uniform_real_distribution<TType> distribution(Min, Max);
			return distribution(generator);
		}
	}

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
