// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Core/ECS/Components/Collision/CircleCollisionComponent.h"

#include "ECS/Collision/CircleCollision.h"
#include "ECS/Collision/CollisionManager.h"

UCircleCollisionComponent::UCircleCollisionComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UCollisionComponent(InComponentManagerInterface)
	, CircleCollision(nullptr)
{
}

UCircleCollisionComponent::~UCircleCollisionComponent()
{
	delete CircleCollision;
}

void UCircleCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	CircleCollision = new FCircleCollision(this, GetLocationForCollision(), GetCircleRadius());

	AddCollision(CircleCollision);
}

void UCircleCollisionComponent::Render()
{
	Super::Render();

#if _DEBUG
	// Draw collision
	if (CircleCollision != nullptr && CollisionManagerCached != nullptr && CollisionManagerCached->IsDebugEnabled())
	{
		FRenderer* Renderer = GetOwnerWindow()->GetRenderer();
		const FCircle& CircleData = CircleCollision->GetCircleData();

		Renderer->DrawCircle(CircleData.GetLocation(), CircleData.GetRadius());
	}
#endif
}

void UCircleCollisionComponent::OnLocationChanged()
{
	Super::OnLocationChanged();

	if (CircleCollision != nullptr)
	{
		FCircle& CircleCollisionDataForEdit = CircleCollision->GetCircleDataForEdit();
		CircleCollisionDataForEdit.UpdateLocation(GetLocationForCollision());
	}
}

int UCircleCollisionComponent::GetCircleRadius() const
{
	FVector2D<int> Size = GetSize();

	return FMath::Max(Size.X, Size.Y) / 2;
}

FVector2D<int> UCircleCollisionComponent::GetLocationForCollision() const
{
	return GetAbsoluteLocation();
}
