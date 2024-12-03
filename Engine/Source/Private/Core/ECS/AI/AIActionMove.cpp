// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Core/ECS/AI/AIActionMove.h"

#include "ECS/Components/MoveComponent.h"

FAIActionMove::FAIActionMove(FAITree* InAiTree)
	: FAIActionBase(InAiTree)
	, CurrentMoveComponent(nullptr)
{
}

void FAIActionMove::Initialize()
{
	Super::Initialize();

	// Get entity. Should always be valid.
	EEntity* Entity = GetTree()->GetOwnerEntity();

	// Move component does not have to be present.
	CurrentMoveComponent = Entity->GetComponentByClass<UMoveComponent>();
	if (CurrentMoveComponent == nullptr)
	{
		LOG_WARN("Missing MoveComponent in FAIActionMove, Entity class: " << Entity->GetCppClassNameWithoutClass());
	}
}

bool FAIActionMove::ShouldFinishAction() const
{
	return (CurrentMoveComponent != nullptr && !CurrentMoveComponent->IsMoving());
}

void FAIActionMove::EndAction()
{
	Super::EndAction();

	// Disable if moving due to action running
	if (CurrentMoveComponent != nullptr && (IsActionRunning() && CurrentMoveComponent->IsMoving()))
	{
		CurrentMoveComponent->AbortMovement();
		CurrentMoveComponent = nullptr;
	}
}

void FAIActionMove::StartAction()
{
	Super::StartAction();

	if (CurrentMoveComponent != nullptr)
	{
		CurrentMoveComponent->OnStoppedMovement.BindObject(this, &FAIActionMove::OnStoppedMovement);
	}
}

void FAIActionMove::SetTargetLocation(const FVector2D<int32>& InLocation)
{
	if (CurrentMoveComponent != nullptr)
	{
		CurrentMoveComponent->SetTargetMoveLocation(InLocation);
	}
}

void FAIActionMove::OnStoppedMovement()
{
	if (CurrentMoveComponent != nullptr)
	{
		CurrentMoveComponent->OnStoppedMovement.UnBindObject(this, &FAIActionMove::OnStoppedMovement);
	}

	EndAction();
}
