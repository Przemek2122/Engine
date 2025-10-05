#include "CoreEngine.h"
#include "Core/ECS/Entities/WeaponBase.h"

#include "Timer/TimerManager.h"

EWeaponBase::EWeaponBase(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, bCanAttack(true)
	, bIsWaitingForCooldown(false)
	, CooldownAttackTime(0.5)
{
}

void EWeaponBase::Attack()
{
	if (CanAttack())
	{
		PerformAttack();

		if (ShouldDelayAttacks())
		{
			FDelegateSafe<void, FOptionalTimerParams*> OnAttackDelayFinished;
			OnAttackDelayFinished.BindObject(this, &EWeaponBase::AttackCooldownFinished);

			AttackCooldownTimer = FTimerManager::CreateTimerSync(OnAttackDelayFinished, CooldownAttackTime);

			bIsWaitingForCooldown = true;
		}

		OnAttackPerformed();
	}
}

void EWeaponBase::SetCanAttack(const bool bInCanAttack)
{
	bCanAttack = bInCanAttack;

	OnCanAttackChanged();
}

void EWeaponBase::SetAttackCooldown(const float NewDelay)
{
	CooldownAttackTime = NewDelay;
}

void EWeaponBase::PerformAttack()
{
}

void EWeaponBase::AttackCooldownFinished(FOptionalTimerParams* OptionalTimerParams)
{
	OnAttackDelayFinished();

	bIsWaitingForCooldown = false;
}

void EWeaponBase::OnCanAttackChanged()
{
}

void EWeaponBase::OnAttackPerformed()
{
}

void EWeaponBase::OnAttackDelayFinished()
{
}
