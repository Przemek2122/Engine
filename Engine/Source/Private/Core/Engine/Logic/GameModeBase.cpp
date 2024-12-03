// Created by Przemys³aw Wiewióra 2020-2024

#include "CoreEngine.h"
#include "Core/Engine/Logic/GameModeBase.h"
#include "Engine/Logic/GameModeManager.h"
#include "Engine/Logic/BaseController.h"
#include "Renderer/Map/MapManager.h"
#include "Threads/ThreadsManager.h"

FGameModeBase::FGameModeBase(FGameModeManager* InGameModeManager)
	: bIsInProgress(false)
	, OwnerGameModeManager(InGameModeManager)
	, CurrentUserIndex(-1)
	, LocalUserController(nullptr)
{
	CurrentMap = OwnerGameModeManager->GetOwnerWindowAdvanced()->GetMapManager()->GetCurrentMap();
}

void FGameModeBase::Initialize()
{
	SetDefaultControllers();
}

void FGameModeBase::DeInitialize()
{
	// Make sure we do not have any async job in queue
	GEngine->GetThreadsManager()->ResetAllJobs();

	// Also reset Engine queue
	GEngine->GetFunctionsToCallOnStartOfNextTick().UnBindAll();
}

void FGameModeBase::Begin()
{
	if (!bIsInProgress)
	{
		bIsInProgress = true;

		Start();

		OnGameplayStart.Execute();
	}
}

void FGameModeBase::Finish()
{
	if (bIsInProgress)
	{
		bIsInProgress = false;

		End();
	}
}

FPlayerController* FGameModeBase::AddPlayer()
{
	FPlayerController* NewController = CreatePlayerController();

	if (NewController != nullptr)
	{
		LOG_INFO("Player created succesfully.");

		UserStateArray.Push(NewController);

		OnPlayerAdded(NewController);
	}
	else
	{
		LOG_WARN("Player NOT created!");
	}

	return NewController;
}

FAIController* FGameModeBase::AddBot()
{
	FAIController* NewAIController = CreateAIController();

	if (NewAIController != nullptr)
	{
		LOG_INFO("AI created succesfully.");

		UserStateArray.Push(NewAIController);

		OnBotAdded(NewAIController);
	}
	else
	{
		LOG_WARN("AI NOT created!");
	}

	return NewAIController;
}

bool FGameModeBase::RemoveUser(const FUserId& InUserIdToRemove)
{
	bool bWasRemoved = false;

	for (ContainerInt i = 0; i < UserStateArray.Size(); i++)
	{
		FBaseController* BaseController = UserStateArray[i];

		if (BaseController->GetUserId() == InUserIdToRemove)
		{
			UserStateArray.RemoveAt(i);

			bWasRemoved = true;

			break;
		}
	}

	return bWasRemoved;
}

FPlayerController* FGameModeBase::GetPlayerControllerByUserId(const FUserId& InUserId)
{
	FPlayerController* Controller = nullptr;

	for (FBaseController* BaseController : UserStateArray)
	{
		if (BaseController->GetUserId() == InUserId)
		{
			Controller = dynamic_cast<FPlayerController*>(BaseController);
		}
	}

	return Controller;
}

FAIController* FGameModeBase::GetAIControllerByUserId(const FUserId& InUserId)
{
	FAIController* Controller = nullptr;

	for (FBaseController* BaseController : UserStateArray)
	{
		if (BaseController->GetUserId() == InUserId)
		{
			Controller = dynamic_cast<FAIController*>(BaseController);
		}
	}

	return Controller;
}

const CArray<FBaseController*>& FGameModeBase::GetControllerArray() const
{
	return UserStateArray;
}

void FGameModeBase::Start()
{
}

void FGameModeBase::End()
{
}

void FGameModeBase::SetDefaultControllers()
{
	LocalUserController = AddPlayer();
}

FPlayerController* FGameModeBase::CreatePlayerController()
{
	return CreateController<FPlayerController>();
}

FAIController* FGameModeBase::CreateAIController()
{
	return CreateController<FAIController>();
}

void FGameModeBase::OnPlayerAdded(FPlayerController* PlayerController)
{
}

void FGameModeBase::OnBotAdded(FAIController* AIController)
{
}

void FGameModeBase::SetPlayerController(FPlayerController* PlayerController)
{
	LocalUserController = PlayerController;
}

const FPlayerController* FGameModeBase::GetLocalController() const
{
	return LocalUserController;
}

FMap* FGameModeBase::GetCurrentMap() const
{
	return CurrentMap;
}

FGameModeManager* FGameModeBase::GetGameModeManager() const
{
	return OwnerGameModeManager;
}

FWindowAdvanced* FGameModeBase::GetWindowAdvanced() const
{
	return OwnerGameModeManager->GetOwnerWindowAdvanced();
}
