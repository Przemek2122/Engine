// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/AI/AITree.h"

#include "ECS/AI/AIActionBase.h"

FAITree::FAITree(EEntity* InOwnerEntity)
	: OwnerEntity(InOwnerEntity)
	, bIsTreeEnabled(true)
{
}

FAITree::~FAITree()
{
	EndAllActiveActions();
}

void FAITree::RemoveAction(const FAIActionBase* AiAction)
{
	ContainerInt Index;

	const bool bIsIndexFound = AllAIActionsArray.FindByLambda([&](const std::shared_ptr<FAIActionBase>& Item)
	{
		bool bIsItemEqual = false;

		if (Item.get() == AiAction)
		{
			bIsItemEqual = true;
		}

		return bIsItemEqual;
	}, Index);

	if (bIsIndexFound)
	{
		AllAIActionsArray.RemoveAt(Index);
	}
}

void FAITree::TickInternal()
{
	if (bIsTreeEnabled)
	{
		for (std::shared_ptr<FAIActionBase>& AIActionPtr : AllAIActionsArray)
		{
			AIActionPtr->Tick();
		}

		ChooseAction();

		Tick();
	}
}

void FAITree::Tick()
{
}

EEntity* FAITree::GetOwnerEntity() const
{
	return OwnerEntity;
}

void FAITree::SetIsTreeEnabled(const bool bInEnable)
{
	bIsTreeEnabled = bInEnable;

	if (!bInEnable)
	{
		EndAllActiveActions();
	}
}

bool FAITree::ActivateAction(FAIActionBase* InAction)
{
	bool bIsActionActivated = false;

	if (InAction != nullptr && !InAction->IsActionRunning() && InAction->IsActionReady())
	{
		StartAction(InAction);

		bIsActionActivated = true;
	}

	return bIsActionActivated;
}

bool FAITree::StopAction(FAIActionBase* InAction, const bool bForceStopAction)
{
	bool bWasStoppingActionSuccess = false;

	if (InAction != nullptr && InAction->IsActionRunning())
	{
		bWasStoppingActionSuccess = true;

		if (bForceStopAction || InAction->ShouldFinishAction())
		{
			InAction->End();
		}
	}

	return bWasStoppingActionSuccess;
}

bool FAITree::IsAnyActionRunning() const
{
	bool bIsAnyActionRunning = false;

	for (const std::shared_ptr<FAIActionBase>& ActionPtr : AllAIActionsArray)
	{
		if (ActionPtr->IsActionRunning())
		{
			bIsAnyActionRunning = true;

			break;
		}
	}

	return bIsAnyActionRunning;
}

void FAITree::ChooseAction()
{
	for (std::shared_ptr<FAIActionBase>& ActionPtr : AllAIActionsArray)
	{
		if (ActionPtr->IsActionRunning())
		{
			if (ActionPtr->ShouldFinishAction())
			{
				EndAction(ActionPtr.get());
			}
		}
		else if (ActionPtr->HasAutomaticStart() && ActionPtr->IsActionReady())
		{
			StartAction(ActionPtr.get());
		}
	}
}

void FAITree::OnActionActivated(FAIActionBase* InAction)
{
}

void FAITree::OnActionDeactivated(FAIActionBase* InAction)
{
}

void FAITree::StartAction(FAIActionBase* InAction)
{
	InAction->Start();

	OnActionActivated(InAction);
}

void FAITree::EndAction(FAIActionBase* InAction)
{
	InAction->End();

	OnActionDeactivated(InAction);
}

void FAITree::EndAllActiveActions()
{
	for (const std::shared_ptr<FAIActionBase>& AIActionBase : AllAIActionsArray)
	{
		if (AIActionBase->IsActionRunning())
		{
			StopAction(AIActionBase.get(), true);
		}
	}
}
