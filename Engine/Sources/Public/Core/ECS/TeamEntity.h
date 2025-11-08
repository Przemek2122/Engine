// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "Core/ECS/Entity.h"
#include "Engine/Logic/BaseController.h"

class UTeamComponent;
class FWindowAdvanced;
class FMap;
class FGameModeManager;
class FEntityManager;

/**
 * Entity with team component
 */
class ENGINE_API ETeamEntity : public EEntity
{
public:
	ETeamEntity(FEntityManager* InEntityManager, const FUserId& InOwnerUserId, const int InTeam);

	UTeamComponent* GetTeamComponent() const;

private:
	UTeamComponent* TeamComponent;

};
