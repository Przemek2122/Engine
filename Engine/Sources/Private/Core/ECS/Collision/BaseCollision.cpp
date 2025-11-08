// Created by https://www.linkedin.com/in/przemek2122/ 2024

#include "CoreEngine.h"
#include "ECS/Collision/BaseCollision.h"

FCollisionBase::FCollisionBase(UCollisionComponent* InCollisionComponent)
	: CollisionType(ECollisionType::Other)
	, CollisionComponent(InCollisionComponent)
{
}

int FCollisionBase::GetBaseExtentRadius()
{
	return 0;
}

ECollisionType FCollisionBase::GetCollisionType() const
{
	return CollisionType;
}
