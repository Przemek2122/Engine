#pragma once

#include "ECS/BaseComponent.h"
#include "Engine/Logic/BaseController.h"

/**
 * Team Component
 * defines team and user which is owning a unit
 */
class UTeamComponent : public UBaseComponent
{
public:
	UTeamComponent(IComponentManagerInterface* InComponentManagerInterface);
	UTeamComponent(IComponentManagerInterface* InComponentManagerInterface, const FUserId& InOwnerUserId, const int32 InTeam);

	void SetOwnerUserId(const FUserId& NewOwnerUserId) { OwnerUserId = NewOwnerUserId; }
	FUserId GetOwnerUserId() const { return OwnerUserId; }

	void SetCurrentTeam(const int32 NewTeam) { CurrentTeamId = NewTeam; }
	int32 GetCurrentTeam() const { return CurrentTeamId; }

	/** Set UserId and CurrentTeamId from BaseController. */
	void SetDataFromController(const FBaseController* InBaseController);

	/** Set UserId and CurrentTeamId from other team component. */
	void SetDataFromOtherComponent(const UTeamComponent* InTeamComponent);

protected:
	/** Owner id */
	FUserId OwnerUserId;

	/** Team of owning entity */
	int32 CurrentTeamId;

};
