#include "CoreEngine.h"
#include "ECS/Entities/ProjectileEntity.h"

#include "ECS/Components/MoveComponent.h"
#include "ECS/Components/RenderComponent.h"

EProjectileEntity::EProjectileEntity(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
{
	UParentComponent* NewRootComponent = CreateComponent<UParentComponent>("RootComponent");

	RenderComponent = NewRootComponent->CreateComponent<URenderComponent>("RenderComponent");
	MovementComponent = NewRootComponent->CreateComponent<UMoveComponent>("MovementComponent");
}

void EProjectileEntity::SetProjectileMovementParams(const EProjectileMovementParams& ProjectileMovementParams) const
{
	// Set movement settings
	MovementComponent->SetMovementMethod(EMovementMethod::Linear);
	MovementComponent->SetLinearSpeedPerSecond(ProjectileMovementParams.LinearSpeedPerSecond);

	// Set rotation
	GetRootComponent()->SetRotation(ProjectileMovementParams.Angle);
}

URenderComponent* EProjectileEntity::GetRenderComponent() const
{
	return RenderComponent;
}

UMoveComponent* EProjectileEntity::GetMovementComponent() const
{
	return MovementComponent;
}
