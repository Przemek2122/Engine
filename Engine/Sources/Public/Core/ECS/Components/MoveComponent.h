// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "ECS/Component.h"

class UParentComponent;
class UArrowComponent;

enum class EMovementDirection
{
	Forward,
	Backwards,
	Right,
	Left,
};

enum class EMovementMethod
{
	Default,		// Default 'ai unit' like movement
	Linear,			// Movement in one direction
};

/**
 * Component for handling transform of the entity
 */
class UMoveComponent : public UComponent
{
public:
	UMoveComponent(IComponentManagerInterface* InComponentManagerInterface);
	virtual ~UMoveComponent() override = default;

	/** Begin UComponent */
	void BeginPlay() override;
	void Tick(const float DeltaTime) override;
	void Render() override;
	/** End UComponent */

	/** How far owner has to be to stop when approaching TargetLocation */
	void SetStoppingDistance(const float InStopDistance);
	float GetStoppingDistance() const { return StopDistance; }

	/** Reset stop distance to default auto calculated value */
	void ResetStoppingDistance();

	/** Set where unit should go. */
	void SetTargetMoveLocation(const FVector2D<int> NewTargetLocation);

	/** Cancel moving to TargetLocation */
	void AbortMovement();

	void SetShouldRotateInstant(const bool bInShouldRotateInstant);

	virtual bool IsMoving() const;

	virtual void SetMovementMethod(EMovementMethod NewMovementMethod);

	void SetLinearSpeedPerSecond(const float NewSpeed);
	void SetAngularSpeedPerSecond(const float NewSpeed);

	FDelegate<void> OnStoppedMovement;

protected:
	/** Moves internal vector. Does not move owner on it's own */
	FVector2D<float> GetVectorForMoveInDirection(const EMovementDirection MovementDirection) const;

	void UpdateRotationToTarget(float DeltaTime);
	void UpdateLocationToTarget(float DeltaTime);

	void UpdateLocationLinear(float DeltaTime);

	bool IsInMapBounds(const FVector2D<int>& Location) const;

	/** Called when unit moves out of map bounds */
	virtual void OnRequestedLocationOutOfBounds();

	virtual void OnMovementMethodChanged(const EMovementMethod InPrevious, const EMovementMethod InCurrent);

protected:
	/** Location desired by unit. This is the place where we want to move to. */
	FVector2D<int> TargetLocation;

	/** If true, component owner is moving */
	bool bIsMoving;

	/** if true we will not make smooth rotation but instant rotation */
	bool bShouldRotateInstant;

	/** Distance to stop when moving */
	float StopDistance;

	float LinearSpeedPerSecond;
	float AngularSpeedPerSecond;

	FVector2D<float> PreciseLocation;
	float PreciseRotation;

	UParentComponent* RootTransformComponent;

	FVector2D<int> CurrentLocation;
	FVector2D<int> CalculatedTargetLocation;

	bool bHasCustomStopDistance;

	FMap* CurrentMap;

#if _DEBUG
	UArrowComponent* ArrowComponent = nullptr;
	bool bShowForwardArrow = true;
#endif

private:
	EMovementMethod CurrentMovementMethod;

};