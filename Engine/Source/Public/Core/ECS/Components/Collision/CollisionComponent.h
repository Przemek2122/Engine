// Created by Przemys³aw Wiewióra 2020-2024

#pragma once

#include "ECS/BaseComponent.h"
#include "ECS/Component.h"

class FCollisionManager;
class FCollisionBase;

/**
 * Component for collision management
 */
class UCollisionComponent : public UComponent
{
public:
	UCollisionComponent(IComponentManagerInterface* InComponentManagerInterface);

	void BeginPlay() override;

	/** Allows to enable or disable collision of this component */
	virtual void SetCollisionsEnabled(const bool bNewInEnabled);

	bool IsCollisionEnabled() const { return bCollisionsEnabled; }

	void AddCollision(FCollisionBase* CollisionObject);
	void RemoveCollision(FCollisionBase* CollisionObject);

	/** Notify manager about changed collision */
	void OnLocationChanged() override;

	const CArray<FCollisionBase*>& GetCollisionObjectsArray() const;

	void OnCollisionBegin(UCollisionComponent* OtherCollision);
	void OnCollisionEnd(UCollisionComponent* OtherCollision);

	FDelegate<void, UCollisionComponent*> OnCollisionEnter;
	FDelegate<void, UCollisionComponent*> OnCollisionExit;
	
protected:
	/** @returns collision subsystem. Might be expensive. Try avoid use */
	FCollisionManager* GetCollisionManager() const;

	virtual FVector2D<int> GetLocationForCollision() const;

#if _DEBUG
	static FColorRGBA GetCollisionDebugColor();
#endif

	/** Collision manager cached reference */
	FCollisionManager* CollisionManagerCached;

private:
	/** Array with collision objects */
	CArray<FCollisionBase*> CollisionObjectsArray;

	/**
	 * This bool will be initially disabled.
	 * Then depending on @bCheckCollisionsAfterInit it will be set to true or false
	 * User can also disable or enable collisions
	 */
	bool bCollisionsEnabled;

	/** See bool above */
	bool bCollisionsEnabledInitial;
};
