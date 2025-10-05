#include "CoreEngine.h"
#include "ECS/Components/TeamComponent.h"

UTeamComponent::UTeamComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UBaseComponent(InComponentManagerInterface)
	, CurrentTeamId(0)
{
}

UTeamComponent::UTeamComponent(IComponentManagerInterface* InComponentManagerInterface, const FUserId& InOwnerUserId, const int32 InTeam)
	: UBaseComponent(InComponentManagerInterface)
	, OwnerUserId(InOwnerUserId)
	, CurrentTeamId(InTeam)
{
}

void UTeamComponent::SetDataFromController(const FBaseController* InBaseController)
{
	SetOwnerUserId(InBaseController->GetUserId());
	SetCurrentTeam(InBaseController->GetUserTeamId());
}

void UTeamComponent::SetDataFromOtherComponent(const UTeamComponent* InTeamComponent)
{
	SetOwnerUserId(InTeamComponent->GetOwnerUserId());
	SetCurrentTeam(InTeamComponent->GetCurrentTeam());
}
