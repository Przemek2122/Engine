// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "BaseCollision.h"

struct FCircle
{
public:
	FCircle(const FVector2D<int>& InLocation, const int InCircleRadius)
		: Location(InLocation)
		, CircleRadius(InCircleRadius)
	{
	}

	const FVector2D<int>& GetLocation() const { return Location; }
	int GetRadius() const { return CircleRadius; }

	void UpdateLocation(const FVector2D<int>& InLocation);
	void UpdateRadius(const int InCircleRadius);

protected:
	FVector2D<int> Location;

	int CircleRadius;
};

/**
 * Basic BoundingCircles collision
 */
class FCircleCollision : public FCollisionBase
{
public:
	FCircleCollision(UCollisionComponent* InCollisionComponent, const FVector2D<int> InLocation, const int InCircleRadius);

	int GetBaseExtentRadius() override;

	const FCircle& GetCircleData() const;
	FCircle& GetCircleDataForEdit();

protected:
	FCircle CircleCollisionData;

};
