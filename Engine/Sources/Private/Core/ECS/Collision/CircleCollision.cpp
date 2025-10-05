// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/Collision/CircleCollision.h"

void FCircle::UpdateLocation(const FVector2D<int>& InLocation)
{
	Location = InLocation;
}

void FCircle::UpdateRadius(const int InCircleRadius)
{
	CircleRadius = InCircleRadius;
}

FCircleCollision::FCircleCollision(UCollisionComponent* InCollisionComponent, const FVector2D<int> InLocation, const int InCircleRadius)
	: FCollisionBase(InCollisionComponent)
	, CircleCollisionData(InLocation, InCircleRadius)
{
	CollisionType = ECollisionType::Circle;
}

int FCircleCollision::GetBaseExtentRadius()
{
	return CircleCollisionData.GetRadius();
}

const FCircle& FCircleCollision::GetCircleData() const
{
	return CircleCollisionData;
}

FCircle& FCircleCollision::GetCircleDataForEdit()
{
	return CircleCollisionData;
}
