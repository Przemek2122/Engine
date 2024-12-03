// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/BaseComponent.h"

UBaseComponent::UBaseComponent(IComponentManagerInterface* InComponentManagerInterface)
	: IComponentManagerInterface(InComponentManagerInterface, InComponentManagerInterface->GetOwnerWindow())
	, bIsComponentActive(true)
{
}

void UBaseComponent::BeginPlay()
{
	ActivateComponent();
}

void UBaseComponent::EndPlay()
{
	DeactivateComponent();
}

void UBaseComponent::Tick(const float DeltaTime)
{
	// Tick sub-components
	TickComponents(DeltaTime);
}

void UBaseComponent::Render()
{
	// Render sub-components
	RenderComponents();
}

void UBaseComponent::ActivateComponent()
{
	bIsComponentActive = true;
}

void UBaseComponent::DeactivateComponent()
{
	bIsComponentActive = false;
}

bool UBaseComponent::IsComponentActive() const
{
	return bIsComponentActive;
}

EEntity* UBaseComponent::GetEntity() const
{
	EEntity* Owner = nullptr;

	IComponentManagerInterface* TopOwner = GetOwnerTop();
	if (TopOwner != nullptr)
	{
		Owner = dynamic_cast<EEntity*>(TopOwner);
	}

	return Owner;
}

UParentComponent* UBaseComponent::GetRootComponentOfEntity() const
{
	UParentComponent* RetComponent = nullptr;

	EEntity* Entity = GetEntity();
	if (Entity != nullptr)
	{
		UParentComponent* RootComponent = Entity->GetRootComponent();
		if (RootComponent != nullptr)
		{
			RetComponent = RootComponent;
		}
		else
		{
			LOG_ERROR("Mising RootComponent.");
		}
	}
	else
	{
		LOG_ERROR("Mising Entity. Make sure owner is correct.");
	}

	return RetComponent;
}
