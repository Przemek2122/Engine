// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "CoreMinimal.h"

struct ENGINE_API FSpark
{
	FSpark();
	FSpark(const FVector2D<int>& InLocation, const float InSpeed, const int InRotationDegree);
	
	FVector2D<float> Location;
	float Speed;
	int Angle;
};

/**
 * Widget that creates sparks under mouse cursor
 */
class ENGINE_API FMouseSparkWidget : public FWidget
{
public:
	FMouseSparkWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder = WIDGET_DEFINES_DEFAULT_ORDER);

	/** Begin FWidget interface */
	void Init() override;
	void Tick() override;
	void Render() override;
	/** End FWidget interface */

protected:
	CDeque<FSpark> Sparks;
	CArray<SDL_FPoint> Points;
	
	FVector2D<float> MinMaxSpeedRange;
	FVector2D<int> MinMaxAngleRange;
	
	int MaxNumOfPoints;
	int PointPerTick;
	
};
