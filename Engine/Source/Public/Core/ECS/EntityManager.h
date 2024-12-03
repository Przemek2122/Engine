// Created by Przemys³aw Wiewióra 2020-2023

#pragma once

#include "Components/ParentComponent.h"
#include "ECS/Entity.h"

class FMap;

class FEntityManager
{
	friend FMap;

protected:
	FEntityManager(FWindow* InOwnerWindow);
	virtual ~FEntityManager();

public:
	template <typename TEntityClass>
	void RegisterNewEntity(TEntityClass* NewEntity)
	{
		Entities.Push(NewEntity);

		NewEntity->BeginPlay();

		OnEntityCreated(NewEntity);
	}

	template<typename TEntityClass, typename... TInParams>
	TEntityClass* CreateEntity(TInParams... InParams)
	{
		TEntityClass* NewEntity = new TEntityClass(this, InParams ...);

		RegisterNewEntity<TEntityClass>(NewEntity);

		return NewEntity;
	}

	template<typename TEntityClass, typename... TInParams>
	TEntityClass* CreateEntityAt(const FVector2D<int> Location, TInParams... InParams)
	{
		TEntityClass* NewEntity = CreateEntity<TEntityClass>();

		UParentComponent* TransformComponent = dynamic_cast<UParentComponent*>(NewEntity->GetRootComponent());
		if (TransformComponent != nullptr)
		{
			TransformComponent->SetLocation(Location);
		}

		return NewEntity;
	}

	/** Create multiple entities. @returns array with newly created entities */
	template<typename TEntityClass, typename... TInParams>
	CArray<TEntityClass*> CreateMultipleEntities(const int32_t NumberOfEntitiesToCreate, TInParams... InParams)
	{
		CArray<TEntityClass*> EntitiesCreated;

		for (int32_t i = 0; i < NumberOfEntitiesToCreate; i++)
		{
			EntitiesCreated.Push(CreateEntity<TEntityClass>());
		}

		return std::move(EntitiesCreated);
	}

	bool DestroyEntity(const EEntity* Entity);

	template<typename TEntityClass>
	TEntityClass* GetEntityByType() const
	{
		for (EEntity* Entity : Entities)
		{
			if (TEntityClass* EntitySearch = dynamic_cast<TEntityClass*>(Entity))
			{
				return EntitySearch;
			}
		}

		return nullptr;
	}

	FWindow* GetOwnerWindow() const { return OwnerWindow; }

	virtual void Tick(float DeltaTime);
	virtual void Render();

	FDelegate<> OnEntityManagerDestroyed;

protected:
	virtual void OnEntityCreated(EEntity* Entity);
	virtual void OnEntityPreDestroyed(EEntity* Entity);

private:
	/** Array with entites */
	CArray<EEntity*> Entities;

	/** Owner window */
	FWindow* OwnerWindow;

};
