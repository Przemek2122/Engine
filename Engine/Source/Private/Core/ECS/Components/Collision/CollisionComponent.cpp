// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/Components/Collision/CollisionComponent.h"

#include "ECS/Collision/CollisionManager.h"
#include "Renderer/Map/Map.h"
#include "Renderer/Map/MapManager.h"

UCollisionComponent::UCollisionComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UComponent(InComponentManagerInterface)
	, CollisionManagerCached(nullptr)
	, bCollisionsEnabled(false)
	, bCollisionsEnabledInitial(true)
{
}

void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	CollisionManagerCached = GetCollisionManager();

	SetCollisionsEnabled(bCollisionsEnabledInitial);
}

void UCollisionComponent::SetCollisionsEnabled(const bool bNewInEnabled)
{
	bCollisionsEnabled = bNewInEnabled;
}

void UCollisionComponent::AddCollision(FCollisionBase* CollisionObject)
{
	if (CollisionManagerCached != nullptr)
	{
		CollisionObjectsArray.Push(CollisionObject);

		CollisionManagerCached->RegisterCollision(CollisionObject);
	}
	else
	{
		LOG_ERROR("Missing CollisionManager. Collision will not work.");
	}
}

void UCollisionComponent::RemoveCollision(FCollisionBase* CollisionObject)
{
	if (CollisionManagerCached != nullptr)
	{
		CollisionObjectsArray.Remove(CollisionObject);

		CollisionManagerCached->UnRegisterCollision(CollisionObject);
	}
	else
	{
		LOG_ERROR("Missing CollisionManager. Collision will not work.");
	}
}

void UCollisionComponent::OnLocationChanged()
{
	Super::OnLocationChanged();

	if (CollisionManagerCached != nullptr)
	{
		// Send notification about changed collision to CollisionManager
		for (FCollisionBase* CollisionObjects : CollisionObjectsArray)
		{
			CollisionManagerCached->OnCollisionObjectMoved(CollisionObjects);
		}
	}
}

const CArray<FCollisionBase*>& UCollisionComponent::GetCollisionObjectsArray() const
{
	return CollisionObjectsArray;
}

void UCollisionComponent::OnCollisionBegin(UCollisionComponent* OtherCollision)
{
	OnCollisionEnter.Execute(OtherCollision);
}

void UCollisionComponent::OnCollisionEnd(UCollisionComponent* OtherCollision)
{
	OnCollisionExit.Execute(OtherCollision);
}

FCollisionManager* UCollisionComponent::GetCollisionManager() const
{
	FCollisionManager* FoundCollisionManager = nullptr;

	FMapManager* MapManager = GetOwnerWindow()->GetMapManager();
	if (MapManager != nullptr)
	{
		FMap* CurrentMap = MapManager->GetCurrentMap();
		if (CurrentMap != nullptr)
		{
			FoundCollisionManager = CurrentMap->GetSubSystemByClass<FCollisionManager>();
		}
	}

	return FoundCollisionManager;
}

FVector2D<int> UCollisionComponent::GetLocationForCollision() const
{
	return GetAbsoluteLocation();
}

#if _DEBUG
FColorRGBA UCollisionComponent::GetCollisionDebugColor()
{
	static FColorRGBA CollisionDebugColor = FColorRGBA::ColorLightGreen();

	return CollisionDebugColor;
}
#endif