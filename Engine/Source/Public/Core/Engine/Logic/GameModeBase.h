// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseController.h"
#include "Renderer/Map/Map.h"

struct FUserId;
class FBaseController;
class FWindowAdvanced;
class FGameModeManager;

/**
 * Base class for game modes.
 */
class FGameModeBase
{
public:
	FGameModeBase(FGameModeManager* InGameModeManager);
	virtual ~FGameModeBase() = default;

	virtual void Initialize();
	virtual void DeInitialize();

	/**
	 * Use to start gameplay
	 * Will call Start()
	 */
	void Begin();

	/**
	 * Use to end gameplay
	 * Will call End()
	 */
	void Finish();

	/** Creating player instance */
	FPlayerController* AddPlayer();

	/** Creating AI instance */
	FAIController* AddBot();

	/** Remove player (FPlayerController) or AI (FAIController) by ID */
	bool RemoveUser(const FUserId& InUserIdToRemove);

	/** @returns FPlayerState found by UserId */
	FPlayerController* GetPlayerControllerByUserId(const FUserId& InUserId);

	/** @returns FPlayerState found by UserId */
	FAIController* GetAIControllerByUserId(const FUserId& InUserId);

	/** Array with all controllers */
	const CArray<FBaseController*>& GetControllerArray() const;

	virtual bool IsInProgress() const { return bIsInProgress; }

	FGameModeManager* GetGameModeManager() const;
	FWindowAdvanced* GetWindowAdvanced() const;

	/** Called when start is called. Make sure to check if gameplay is not already in progress using IsInProgress() */
	FDelegate<void> OnGameplayStart;

	const FPlayerController* GetLocalController() const;

protected:
	virtual void Start();
	virtual void End();

	/** This function is used to set default controller. For example local one */
	virtual void SetDefaultControllers();

	/** Override to set custom UserState class for each player */
	virtual FPlayerController* CreatePlayerController();

	/** Override to set custom UserState class for each AI */
	virtual FAIController* CreateAIController();

	template<class TControllerClass>
	TControllerClass* CreateController()
	{
		TControllerClass* Controller = nullptr;

		if (CurrentMap != nullptr)
		{
			FEntityManager* EntityManager = CurrentMap->GetEntityManager();
			if (EntityManager != nullptr)
			{
				CurrentUserIndex++;

				Controller = EntityManager->CreateEntity<TControllerClass>(FUserId(CurrentUserIndex));
			}
		}

		return Controller;
	}

	virtual void OnPlayerAdded(FPlayerController* PlayerController);
	virtual void OnBotAdded(FAIController* AIController);

	void SetPlayerController(FPlayerController* PlayerController);

	FMap* GetCurrentMap() const;

private:
	/** Has Start been called? */
	bool bIsInProgress;

	/** Parent */
	FGameModeManager* OwnerGameModeManager;

	/** All user state instances */
	CArray<FBaseController*> UserStateArray;

	/** Current user state index */
	int32 CurrentUserIndex;

	/** Cached map for gamemode */
	FMap* CurrentMap;

	/** Local user controller */
	FPlayerController* LocalUserController;

	/** Local user id - Should be same as in LocalUserController */
	FUserId LocalUserId;

};
