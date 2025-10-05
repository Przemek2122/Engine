// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/TeamEntity.h"
#include "ECS/Components/TeamComponent.h"

ETeamEntity::ETeamEntity(FEntityManager* InEntityManager, const FUserId& InOwnerUserId, const int InTeam)
	: EEntity(InEntityManager)
{
	TeamComponent = CreateComponent<UTeamComponent>("TeamComponent", InOwnerUserId, InTeam);
}

UTeamComponent* ETeamEntity::GetTeamComponent() const
{
	return TeamComponent;
}
