// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class FPlayerController;
class FBaseController;
class FGameModeBase;

enum EStateType
{
	Base,
	Player,
	AI
};

#define USER_ID_DEFAULT_INCORRECT (-1)

/**
 * User Id for disallowing to provide raw Id.
 * It should always come from object.
 */
struct FUserId
{
	friend FGameModeBase;

private:
	FUserId(const int32 NewIdNumber);

public:
	FUserId();

	/** Get raw integer */
	int32 Get() const { return IdNumber; }

	/** Utility to compare FUserId */
	bool operator==(const FUserId& Other) const { return IdNumber == Other.IdNumber; }

private:
	int32 IdNumber;
	
};

/**
 * Class for data shared between AI and player like
 * resources for current game etc...
 */
class EState : public EEntity
{
	friend FBaseController;

public:
	EState(FEntityManager* InEntityManager);

	FBaseController* GetController() const { return BaseController; }

protected:
	FBaseController* BaseController;
	
};

class EHUDBase : public EEntity
{
	friend FPlayerController;

public:
	EHUDBase(FEntityManager* InEntityManager);

	FPlayerController* GetPlayerController() const;

protected:
	FPlayerController* PlayerController;

};

/** This class should exist for each user or AI */
class FBaseController : public EEntity
{
public:
	FBaseController(FEntityManager* InEntityManager, const FUserId& InUserId);

	void BeginPlay() override;

	template<typename TStateType = EState>
	TStateType* GetPlayerState() const
	{
		return dynamic_cast<TStateType*>(State);
	}

	FUserId GetUserId() const;
	int32 GetUserTeamId() const;
	EState* GetPlayerState() const;
	virtual EStateType GetStateType() const;

	void SetTeam(const int32 InTeamId);

protected:
	virtual EState* CreateState(FEntityManager* EntityManager);

private:
	/** Unique id for each player */
	FUserId UserId;

	/** User team */
	int32 UserTeamId;

	/** Player state class */
	EState* State;

};

/** Player */
class FPlayerController : public FBaseController
{
public:
	FPlayerController(FEntityManager* InEntityManager, const FUserId& InUserId);

	/** Begin FBaseController */
	void BeginPlay() override;
	EStateType GetStateType() const override;
	/** End FBaseController */

	template<typename THUDType = EHUDBase>
	THUDType* GetHUD() const
	{
		return dynamic_cast<THUDType*>(HUD);
	}

protected:
	virtual EHUDBase* CreateHUD(FEntityManager* EntityManager);

protected:
	EHUDBase* HUD;

};

/** AI */
class FAIController : public FBaseController
{
public:
	FAIController(FEntityManager* InEntityManager, const FUserId& InUserId);

	EStateType GetStateType() const override;

};
