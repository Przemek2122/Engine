// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Components/HealthComponent.h"
#include "ECS/Components/RenderComponent.h"
#include "Misc/Math.h"

UHealthComponent::UHealthComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UBaseComponent(InComponentManagerInterface)
	, StartingHealth(100.f)
	, MaxHealth(StartingHealth)
	, CurrentHealth(StartingHealth)
	, bIsDead(false)
	, bUseHealthBarComponent(false)
	, HealthBarRenderComponent(nullptr)
{
}

void UHealthComponent::BeginPlay()
{
	UBaseComponent::BeginPlay();

	if (bUseHealthBarComponent)
	{
		HealthBarRenderComponent = CreateComponent<URenderComponent>("HealthBarRenderComponent");
	}
}

void UHealthComponent::SetHealthDefaults()
{
	CurrentHealth = StartingHealth;
}

void UHealthComponent::TakeDamage(const float Damage)
{
	if (IsAlive())
	{
		CurrentHealth -= FMath::Abs(Damage);

		OnHealthLowered.Execute(Damage);

		if (CurrentHealth <= 0.f)
		{
			Die();
		}
	}
}

void UHealthComponent::Heal(const float HealthToAdd)
{
	if (IsAlive())
	{
		CurrentHealth += FMath::Abs(HealthToAdd);

		OnHealthGained.Execute(HealthToAdd);

		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
}

void UHealthComponent::SetStartingHealth(const float NewHealth)
{
	StartingHealth = NewHealth;
	CurrentHealth = NewHealth;
}

void UHealthComponent::SetMaxHealthOverride(const float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

void UHealthComponent::ResetMaxHealthToDefault()
{
	MaxHealth = StartingHealth;
}

bool UHealthComponent::IsAlive() const
{
	return !IsDead();
}

bool UHealthComponent::IsDead() const
{
	return bIsDead;
}

void UHealthComponent::Die()
{
	CurrentHealth = 0.f;

	bIsDead = true;

	OnDeathDelegate.Execute();
}
