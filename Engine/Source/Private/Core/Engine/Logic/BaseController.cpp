// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Engine/Logic/BaseController.h"

FUserId::FUserId(const int32 NewIdNumber)
	: IdNumber(NewIdNumber)
{
}

FUserId::FUserId()
	: IdNumber(USER_ID_DEFAULT_INCORRECT)
{
}

EState::EState(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, BaseController(nullptr)
{
}

EHUDBase::EHUDBase(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, PlayerController(nullptr)
{
}

FPlayerController* EHUDBase::GetPlayerController() const
{
	return PlayerController;
}

FBaseController::FBaseController(FEntityManager* InEntityManager, const FUserId& InUserId)
	: EEntity(InEntityManager)
	, UserId(InUserId)
	, State(nullptr)
{
}

void FBaseController::BeginPlay()
{
	FEntityManager* EntityManager = GetEntityManagerOwner();

	State = CreateState(EntityManager);
	State->BaseController = this;

	Super::BeginPlay();
}

FUserId FBaseController::GetUserId() const
{
	return UserId;
}

int32 FBaseController::GetUserTeamId() const
{
	return UserTeamId;
}

EState* FBaseController::GetPlayerState() const
{
	return State;
}

EStateType FBaseController::GetStateType() const
{
	return EStateType::Base;
}

void FBaseController::SetTeam(const int32 InTeamId)
{
	UserTeamId = InTeamId;
}

EState* FBaseController::CreateState(FEntityManager* EntityManager)
{
	return EntityManager->CreateEntity<EState>();
}

FPlayerController::FPlayerController(FEntityManager* InEntityManager, const FUserId& InUserId)
	: FBaseController(InEntityManager, InUserId)
	, HUD(nullptr)
{
}

void FPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FEntityManager* EntityManager = GetEntityManagerOwner();

	HUD = CreateHUD(EntityManager);
	HUD->PlayerController = this;
}

EStateType FPlayerController::GetStateType() const
{
	return EStateType::Player;
}

EHUDBase* FPlayerController::CreateHUD(FEntityManager* EntityManager)
{
	return EntityManager->CreateEntity<EHUDBase>();
}

FAIController::FAIController(FEntityManager* InEntityManager, const FUserId& InUserId)
	: FBaseController(InEntityManager, InUserId)
{
}

EStateType FAIController::GetStateType() const
{
	return EStateType::AI;
}
