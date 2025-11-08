// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "BaseCollision.h"

struct ENGINE_API FCircle
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
class ENGINE_API FCircleCollision : public FCollisionBase
{
public:
	FCircleCollision(UCollisionComponent* InCollisionComponent, const FVector2D<int> InLocation, const int InCircleRadius);

	int GetBaseExtentRadius() override;

	const FCircle& GetCircleData() const;
	FCircle& GetCircleDataForEdit();

protected:
	FCircle CircleCollisionData;

};
