// Created by Przemys�aw Wiewi�ra 2020

#include "CoreEngine.h"

UComponent::UComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UBaseComponent(InComponentManagerInterface)
{
}

void UComponent::BeginPlay()
{
	UBaseComponent::BeginPlay();

	for (const auto& ComponentPair : ComponentsMap)
	{
		ComponentPair.second->BeginPlay();
	}
}

void UComponent::OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent)
{
	UBaseComponent::OnComponentCreated(ComponentName, NewComponent);

	FTransform2DInterface* Transform2DInterface = dynamic_cast<FTransform2DInterface*>(NewComponent);
	if (Transform2DInterface != nullptr)
	{
		AddUpdatedComponent(Transform2DInterface);

		// Update location of new component
		Transform2DInterface->OnParentLocationChanged(GetAbsoluteLocation());

		// Update rotation of new component
		Transform2DInterface->OnParentRotationChanged(GetAbsoluteRotation());
	}
}

void UComponent::OnComponentDestroy(const std::string& ComponentName, UBaseComponent* OldComponent)
{
	UBaseComponent::OnComponentDestroy(ComponentName, OldComponent);

	FTransform2DInterface* Transform2DInterface = dynamic_cast<FTransform2DInterface*>(OldComponent);
	if (Transform2DInterface != nullptr)
	{
		RemoveUpdatedComponent(Transform2DInterface);
	}
}

FVector2D<int> UComponent::GetLocationCenter() const
{
	return FMath::GetLocationCenter(GetAbsoluteLocation(), GetSize());
}
