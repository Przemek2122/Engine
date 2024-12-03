// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Components/MoveComponent.h"
#include "ECS/Components/Debug/ArrowComponent.h"
#include "ECS/Components/ParentComponent.h"
#include "Renderer/Map/Map.h"

UMoveComponent::UMoveComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UComponent(InComponentManagerInterface)
	, bIsMoving(false)
	, bShouldRotateInstant(false)
	, StopDistance(0.f)
	, LinearSpeedPerSecond(40.f)
	, AngularSpeedPerSecond(180.f)
	, bHasCustomStopDistance(false)
	, CurrentMap(nullptr)
	, CurrentMovementMethod(EMovementMethod::Default)
{
#if _DEBUG
	if (bShowForwardArrow)
	{
		ArrowComponent = CreateComponent<UArrowComponent>("ForwardArrowComponent");
	}
#endif
}

void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	EEntity* EntityOwner = GetEntity();
	if (EntityOwner != nullptr)
	{
		RootTransformComponent = dynamic_cast<UParentComponent*>(EntityOwner->GetRootComponent());
		if (RootTransformComponent != nullptr)
		{
			PreciseLocation = RootTransformComponent->GetAbsoluteLocation();
		}

		CurrentMap = EntityOwner->GetCurrentMap();
	}
}

void UMoveComponent::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving())
	{
		switch (CurrentMovementMethod)
		{
			case EMovementMethod::Default:
			{
				if (RootTransformComponent != nullptr)
				{
					CurrentLocation = RootTransformComponent->GetAbsoluteLocation();
					CalculatedTargetLocation = TargetLocation;

					UpdateRotationToTarget(DeltaTime);

					UpdateLocationToTarget(DeltaTime);
				}

				break;
			}
			case EMovementMethod::Linear:
			{
				if (RootTransformComponent != nullptr)
				{
					UpdateLocationLinear(DeltaTime);
				}

				break;
			}
		}
	}
}

void UMoveComponent::Render()
{
	UComponent::Render();

#if _DEBUG
	// Draw line
	FRenderer* Renderer = GetOwnerWindow()->GetRenderer();

	Renderer->DrawLine(CurrentLocation, CalculatedTargetLocation);
#endif
}

void UMoveComponent::SetStoppingDistance(const float InStopDistance)
{
	StopDistance = InStopDistance;

	bHasCustomStopDistance = true;
}

void UMoveComponent::ResetStoppingDistance()
{
	bHasCustomStopDistance = false;
}

void UMoveComponent::SetTargetMoveLocation(const FVector2D<int> NewTargetLocation)
{
	FMap* CurrentMap = GetEntity()->GetCurrentMap();
	if (CurrentMap->IsInBounds(NewTargetLocation))
	{
		PreciseLocation = RootTransformComponent->GetAbsoluteLocation();
		PreciseRotation = static_cast<float>(RootTransformComponent->GetAbsoluteRotation());

		TargetLocation = NewTargetLocation;

		bIsMoving = true;

		if (!bHasCustomStopDistance)
		{
			StopDistance = 0.f;
		}
	}
	else
	{
		OnRequestedLocationOutOfBounds();
	}
}

void UMoveComponent::AbortMovement()
{
	TargetLocation = FVector2D<int>();

	bIsMoving = false;

	OnStoppedMovement.Execute();
}

void UMoveComponent::SetShouldRotateInstant(const bool bInShouldRotateInstant)
{
	bShouldRotateInstant = bInShouldRotateInstant;
}

bool UMoveComponent::IsMoving() const
{
	return bIsMoving;
}

void UMoveComponent::SetMovementMethod(const EMovementMethod NewMovementMethod)
{
	OnMovementMethodChanged(CurrentMovementMethod, NewMovementMethod);

	CurrentMovementMethod = NewMovementMethod;
}

void UMoveComponent::SetLinearSpeedPerSecond(const float NewSpeed)
{
	LinearSpeedPerSecond = NewSpeed;
}

void UMoveComponent::SetAngularSpeedPerSecond(const float NewSpeed)
{
	AngularSpeedPerSecond = NewSpeed;
}

FVector2D<float> UMoveComponent::GetVectorForMoveInDirection(const EMovementDirection MovementDirection) const
{
	if (RootTransformComponent != nullptr)
	{
		FVector2D<float> CurrentVector;

		switch (MovementDirection)
		{
			case EMovementDirection::Forward:
			{
				CurrentVector = RootTransformComponent->GetForwardVector();

				break;
			}
			case EMovementDirection::Backwards:
			{
				CurrentVector = -RootTransformComponent->GetForwardVector();

				break;
			}
			case EMovementDirection::Right:
			{
				CurrentVector = RootTransformComponent->GetRightVector();

				break;
			}
			case EMovementDirection::Left:
			{
				CurrentVector = -RootTransformComponent->GetRightVector();

				break;
			}
		}

		return CurrentVector;
	}

	return { };
}

void UMoveComponent::UpdateRotationToTarget(const float DeltaTime)
{
	// This property is used to never encounter 250 and 4 from next rotation, so we have 360 + 250 and 360 + 4
	static constexpr float FullRotation = 360.f;

	const float TargetRotation = FMath::FindLookAtRotationInDegrees(CurrentLocation, CalculatedTargetLocation);
	const float RotationChangeInCurrentTick = AngularSpeedPerSecond * DeltaTime;

	if (bShouldRotateInstant)
	{
		// Instant rotation to target
		PreciseRotation = TargetRotation;
	}
	else
	{
		if (FMath::IsNearlyEqual(FullRotation + PreciseRotation, FullRotation + TargetRotation, RotationChangeInCurrentTick))
		{
			PreciseRotation = TargetRotation;
		}
		else
		{
			if (FMath::IsNearlyEqual(FullRotation + PreciseRotation + 90.f, FullRotation + TargetRotation, 90.f))
			{
				PreciseRotation += RotationChangeInCurrentTick;
			}
			else
			{
				PreciseRotation -= RotationChangeInCurrentTick;
			}
		}
	}

	PreciseRotation = FMath::ClampAngle(PreciseRotation);

	RootTransformComponent->SetRotation(static_cast<int>(PreciseRotation));
}

void UMoveComponent::UpdateLocationToTarget(const float DeltaTime)
{
	const float CurrentDistance = static_cast<float>(CurrentLocation.DistanceTo(TargetLocation));

	if (CurrentDistance < StopDistance)
	{
		// We are close enough. Stop.
		AbortMovement();
	}
	else
	{
		// Get direction vector
		const FVector2D<float> CurrentMoveVector = GetVectorForMoveInDirection(EMovementDirection::Forward);

		// Distance to move
		float DistanceToMoveInThisTick = LinearSpeedPerSecond * DeltaTime;

		FVector2D<> NewLocation;

		const int DistanceBetweenPoints = RootTransformComponent->GetAbsoluteLocation().DistanceTo(TargetLocation);
		if (DistanceBetweenPoints < FMath::RoundToInt(DistanceToMoveInThisTick))
		{
			NewLocation = TargetLocation;

			// We are close enough. Stop.
			AbortMovement();
		}
		else
		{
			const FVector2D<float> LocationToMove = CurrentMoveVector * DistanceToMoveInThisTick;
			PreciseLocation += LocationToMove;

			NewLocation = PreciseLocation;
		}

		if (IsInMapBounds(NewLocation))
		{
			RootTransformComponent->SetLocation(NewLocation);
		}
	}
}

void UMoveComponent::UpdateLocationLinear(const float DeltaTime)
{
	// GetDirection
	FVector2D<float> CurrentMoveVector = GetVectorForMoveInDirection(EMovementDirection::Forward);

	// Units to move
	float DistanceToMoveInThisTick = LinearSpeedPerSecond * DeltaTime;

	// Calculated distance to move in 2D
	FVector2D<> CalculatedMoveDistance = CurrentMoveVector * DistanceToMoveInThisTick;

	if (IsInMapBounds(PreciseLocation + CalculatedMoveDistance))
	{
		// Save precise float location
		PreciseLocation += CalculatedMoveDistance;

		// Apply location
		RootTransformComponent->SetLocation(PreciseLocation);
	}
	else
	{
		AbortMovement();
	}
}

bool UMoveComponent::IsInMapBounds(const FVector2D<int>& Location) const
{
	bool bIsInBounds;

	if (CurrentMap != nullptr)
	{
		bIsInBounds = CurrentMap->IsInBounds(Location);
	}
	else
	{
		bIsInBounds = false;
	}

	return bIsInBounds;
}

void UMoveComponent::OnRequestedLocationOutOfBounds()
{
	AbortMovement();
}

void UMoveComponent::OnMovementMethodChanged(const EMovementMethod InPrevious, const EMovementMethod InCurrent)
{
	if (InCurrent == EMovementMethod::Linear)
	{
		bIsMoving = true;
	}
}
