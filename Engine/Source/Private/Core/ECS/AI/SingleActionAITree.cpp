// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "ECS/AI/SingleActionAITree.h"

#include "ECS/AI/AIActionBase.h"

void FSingleActionAITree::ChooseAction()
{
	const bool bIsAnyActionRunning = IsAnyActionRunning();
	if (!bIsAnyActionRunning)
	{
		switch (SingleActionChooseMethod)
		{
			case ESingleActionChooseMethod::Random:
			{
				if (!AllAIActionsArray.IsEmpty())
				{
					std::shared_ptr<FAIActionBase> ActionPtr = AllAIActionsArray.GetRandomValue();
					FAIActionBase* Action = ActionPtr.get();

					if (Action->HasAutomaticStart())
					{
						StartAction(Action);

						break;
					}
				}

				break;
			}
			case ESingleActionChooseMethod::FirstAvailable:
			{
				for (std::shared_ptr<FAIActionBase>& ActionPtr : AllAIActionsArray)
				{
					FAIActionBase* Action = ActionPtr.get();

					if (Action->HasAutomaticStart())
					{
						StartAction(Action);

						break;
					}
				}

				break;
			}
		}
	}
}