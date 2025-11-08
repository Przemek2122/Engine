// Created by https://www.linkedin.com/in/przemek2122/ 2025

#include "CoreEngine.h"
#include "Misc/Math.h"

#define PI (3.14159265358979323846)

double FMath::Cos(const double AngleRadians)
{
	return std::cos(AngleRadians);
}

double FMath::Sin(const double AngleRadians)
{
	return std::sin(AngleRadians);
}

int FMath::FloorToInt(const float Value)
{
	return static_cast<int>(std::floorf(Value));
}

int FMath::FloorToInt(const double Value)
{
	return static_cast<int>(std::floor(Value));
}

int FMath::RoundToInt(const float Value)
{
	return static_cast<int>(std::roundf(Value));
}

int FMath::RoundToInt(const double Value)
{
	return static_cast<int>(std::round(Value));
}

int FMath::CeilToInt(const float Value)
{
	return static_cast<int>(std::ceilf(Value));
}

int FMath::CeilToInt(const double Value)
{
	return static_cast<int>(std::ceil(Value));
}

double FMath::RadiansToDegrees(double Value)
{
	return static_cast<double>(Value * 180 / PI);
}

double FMath::RadiansToDegreesDouble(double Value)
{
	return Value * 180 / PI;
}

double FMath::DegreesToRadians(double Value)
{
	return static_cast<double>(2 * PI * (Value / 360));
}

double FMath::DegreesToRadiansDouble(double Value)
{
	return (2 * PI * (Value / 360));
}
