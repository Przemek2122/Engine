// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/EntityManager.h"

FEntityManager::FEntityManager(FWindow* InOwnerWindow)
	: OwnerWindow(InOwnerWindow)
{
}

FEntityManager::~FEntityManager()
{
	OnEntityManagerDestroyed.Execute();

	for (EEntity* Entity : Entities)
	{
		Entity->EndPlay();

		delete Entity;
	}

	Entities.Clear();
}

bool FEntityManager::DestroyEntity(const EEntity* Entity)
{
	bool bWasFound = false;

	for (ContainerInt i = 0; i < Entities.Size(); i++)
	{
		EEntity* CurrentEntity = Entities[i];
		if (CurrentEntity == Entity)
		{
			OnEntityPreDestroyed(CurrentEntity);

			CurrentEntity->EndPlay();

			Entities.RemoveAt(i);

			delete CurrentEntity;

			bWasFound = true;

			break;
		}
	}

	return bWasFound;
}

void FEntityManager::Tick(const float DeltaTime)
{
	for (EEntity* Entity : Entities)
	{
		Entity->ReceiveTick(DeltaTime);
	}
}

void FEntityManager::Render()
{
	for (EEntity* Entity : Entities)
	{
		Entity->ReceiveRender();
	}
}

void FEntityManager::OnEntityCreated(EEntity* Entity)
{
}

void FEntityManager::OnEntityPreDestroyed(EEntity* Entity)
{
}
