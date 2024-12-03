// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Core/ECS/Components/Collision/SquareCollisionComponent.h"

#include "ECS/Collision/CollisionManager.h"
#include "ECS/Collision/SquareCollision.h"

USquareCollisionComponent::USquareCollisionComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UCollisionComponent(InComponentManagerInterface)
	, SquareCollision(nullptr)
{
	SquareCollision = new FSquareCollision(this, FTransform2DLocation::Zero(), FTransform2DSize::Zero());
}

USquareCollisionComponent::~USquareCollisionComponent()
{
	delete SquareCollision;
}

void USquareCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SquareCollision != nullptr)
	{
		FRectangleWithDiagonal& SquareDataForEdit = SquareCollision->GetSquareDataForEdit();
		SquareDataForEdit.SetLocationTopLeftCorner(GetAbsoluteLocation());
		SquareDataForEdit.SetSize(GetSize());
	}

	AddCollision(SquareCollision);
}

void USquareCollisionComponent::Render()
{
	Super::Render();

#if _DEBUG
	// Draw collision
	if (SquareCollision != nullptr && CollisionManagerCached != nullptr && CollisionManagerCached->IsDebugEnabled())
	{
		FRenderer* Renderer = GetOwnerWindow()->GetRenderer();

		const FRectangleWithDiagonal& SquareData = SquareCollision->GetSquareData();

		Renderer->DrawRectangleOutline(SquareData.GetPositionTopLeft(), SquareData.GetSize(), GetCollisionDebugColor());
	}
#endif
}

void USquareCollisionComponent::OnLocationChanged()
{
	Super::OnLocationChanged();

	if (SquareCollision != nullptr)
	{
		// Update location
		FRectangleWithDiagonal& SquareDataForEdit = SquareCollision->GetSquareDataForEdit();
		SquareDataForEdit.SetLocationTopLeftCorner(GetAbsoluteLocation());
	}
}

void USquareCollisionComponent::OnRotationChanged()
{
	Super::OnRotationChanged();

	// @TODO Rotation of square collision
}

void USquareCollisionComponent::OnSizeChanged()
{
	Super::OnSizeChanged();

	if (SquareCollision != nullptr)
	{
		FRectangleWithDiagonal& SquareDataForEdit = SquareCollision->GetSquareDataForEdit();
		SquareDataForEdit.SetSize(GetSize());
	}
}
