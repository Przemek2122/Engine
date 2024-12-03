// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Components/ParentComponent.h"
#include "Renderer/Map/Map.h"
#include "Renderer/Map/MapManager.h"

UParentComponent::UParentComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UBaseComponent(InComponentManagerInterface)
{
}

void UParentComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto ComponentPair : ComponentsMap)
	{
		ComponentPair.second->BeginPlay();
	}
}

void UParentComponent::OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent)
{
	Super::OnComponentCreated(ComponentName, NewComponent);

	FTransform2DInterface* TransformComponent = dynamic_cast<FTransform2DInterface*>(NewComponent);
	if (TransformComponent != nullptr)
	{
		AddUpdatedComponent(TransformComponent);

		// Update location of new component
		TransformComponent->OnParentLocationChanged(GetAbsoluteLocation());

		// Update rotation of new component
		TransformComponent->OnParentRotationChanged(GetAbsoluteRotation());
	}
}

void UParentComponent::OnComponentDestroy(const std::string& ComponentName, UBaseComponent* OldComponent)
{
	Super::OnComponentDestroy(ComponentName, OldComponent);

	FTransform2DInterface* TransformComponent = dynamic_cast<FTransform2DInterface*>(OldComponent);
	if (TransformComponent != nullptr)
	{
		RemoveUpdatedComponent(TransformComponent);
	}
}

FVector2D<float> UParentComponent::GetForwardVector() const
{
	static const FVector2D<float> ForwardVector = { 0, -1 };
	constexpr float VectorRotation = 360;

	FVector2D<int> CurrentForwardVector = ForwardVector * VectorRotation;

	const int CurrentRotation = GetAbsoluteRotation();
	const float CurrentRotationRadian = FMath::DegreesToRadians(static_cast<float>(CurrentRotation));

	FMath::RotatePointAroundPoint({ 0, 0 }, CurrentRotationRadian, CurrentForwardVector);

	return FVector2D<float>(CurrentForwardVector) / VectorRotation;
}

FVector2D<float> UParentComponent::GetRightVector() const
{
	static const FVector2D<float> RightVector = { 1, 0 };

	FVector2D<int> CurrentRightVector = RightVector;

	FMath::RotatePointAroundPoint({ 0, 0 }, static_cast<float>(GetAbsoluteRotation()), CurrentRightVector);

	return RightVector;
}

FVector2D<int32> UParentComponent::GetLocationCenter() const
{
	return (GetAbsoluteLocation() + (GetSize() / 2));
}
