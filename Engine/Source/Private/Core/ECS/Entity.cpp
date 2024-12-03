// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Entity.h"

#include "Engine/Logic/GameModeManager.h"
#include "Renderer/WindowAdvanced.h"
#include "Renderer/Map/MapManager.h"

EEntity::EEntity(FEntityManager* InEntityManager)
	: IComponentManagerInterface(nullptr, InEntityManager->GetOwnerWindow())
	, EntityManagerOwner(InEntityManager)
	, DefaultRootComponent(nullptr)
	, bWasBeginPlayCalled(false)
	, EntityAttachment(nullptr)
	, EntityAttachmentRootComponent(nullptr)
	, AttachmentRelativeRotation(0)
{
}

void EEntity::BeginPlay()
{
	for (auto& ComponentPair : ComponentsMap)
	{
		ComponentPair.second->BeginPlay();
	}

	RegisterInputInternal();

	SetupAIActions();

	bWasBeginPlayCalled = true;
	bShouldCallBeginPlayOnNewComponents = true;
}

void EEntity::EndPlay()
{
	UnRegisterInputInternal();
}

void EEntity::Tick(float DeltaTime)
{
	for (const std::shared_ptr<FAITree>& TreeArray : AiTreeArray)
	{
		TreeArray->TickInternal();
	}
}

void EEntity::ReceiveTick(const float DeltaTime)
{
	Tick(DeltaTime);

	TickComponents(DeltaTime);
}

void EEntity::Render()
{
}

void EEntity::ReceiveRender()
{
	Render();

	RenderComponents();

#if _DEBUG
	UParentComponent* RootComponent = dynamic_cast<UParentComponent*>(GetRootComponent());
	if (RootComponent != nullptr)
	{
		FRenderer* Renderer = GetWindow()->GetRenderer();

		FVector2D<int> DebugRectLocation = RootComponent->GetAbsoluteLocation() - FVector2D<int>(2, 2);
		FVector2D<int> DebugRectSize = { 4, 4 };

		Renderer->DrawRectangle(DebugRectLocation, DebugRectSize, FColorRGBA::ColorOrange());
	}
#endif
}

bool EEntity::IsAttached() const
{
	return (EntityAttachment != nullptr);
}

void EEntity::ResetAttachment()
{
	if (EntityAttachment != nullptr)
	{
		OnDeAttachedFromEntity();

		EntityAttachment = nullptr;
		EntityAttachmentRootComponent = nullptr;
	}
}

void EEntity::AttachToEntity(EEntity* InEntityToAttachTo)
{
	if (InEntityToAttachTo != nullptr)
	{
		ResetAttachment();

		EntityAttachment = InEntityToAttachTo;
		EntityAttachmentRootComponent = InEntityToAttachTo->GetRootComponent();

		OnAttachedToEntity();

		OnAttachedComponentLocationChanged(EntityAttachmentRootComponent->GetAbsoluteLocation());
		OnAttachedComponentRotationChanged(EntityAttachmentRootComponent->GetAbsoluteRotation());
	}
}

void EEntity::SetRootComponent(UParentComponent* NewComponent)
{
	DefaultRootComponent = NewComponent;
}

UParentComponent* EEntity::GetRootComponent() const
{
	return DefaultRootComponent;
}

void EEntity::SetLocation(const FVector2D<int32> NewLocation)
{
	UParentComponent* ParentComponent = GetRootComponent();
	if (ParentComponent != nullptr)
	{
		ParentComponent->SetLocation(NewLocation);
	}
}

void EEntity::SetRotation(const int32 Rotation)
{
	UParentComponent* ParentComponent = GetRootComponent();
	if (ParentComponent != nullptr)
	{
		ParentComponent->SetRotation(Rotation);
	}
}

void EEntity::SetRelativeLocation(const FVector2D<int32> NewLocation)
{
	AttachmentRelativeLocation = NewLocation;

	if (EntityAttachmentRootComponent != nullptr)
	{
		OnAttachedComponentLocationChanged(EntityAttachmentRootComponent->GetLocation());
	}
}

void EEntity::SetRelativeRotation(const int32 NewRotation)
{
	AttachmentRelativeRotation = NewRotation;

	if (EntityAttachmentRootComponent != nullptr)
	{
		OnAttachedComponentRotationChanged(EntityAttachmentRootComponent->GetRotation());
	}
}

FVector2D<int32> EEntity::GetLocation() const
{
	FVector2D<int32> RetLocation;

	UParentComponent* ParentComponent = GetRootComponent();
	if (ParentComponent != nullptr)
	{
		RetLocation = ParentComponent->GetAbsoluteLocation();
	}

	return RetLocation;
}

int32 EEntity::GetRotation() const
{
	int32 RetRotation;

	UParentComponent* ParentComponent = GetRootComponent();
	if (ParentComponent != nullptr)
	{
		RetRotation = ParentComponent->GetAbsoluteRotation();
	}
	else
	{
		RetRotation = 0;
	}

	return RetRotation;
}

void EEntity::RegisterInput(FEventHandler* InputHandler)
{
}

void EEntity::UnRegisterInput(FEventHandler* InputHandler)
{
}

FEntityManager* EEntity::GetEntityManagerOwner() const
{
	return EntityManagerOwner;
}

FWindow* EEntity::GetWindow() const
{
	return EntityManagerOwner->GetOwnerWindow();
}

FWindowAdvanced* EEntity::GetWindowAdvanced() const
{
	return reinterpret_cast<FWindowAdvanced*>(GetWindow());
}

FMap* EEntity::GetCurrentMap() const
{
	return GetWindow()->GetMapManager()->GetCurrentMap();
}

FGameModeManager* EEntity::GetGameModeManager() const
{
	return GetWindowAdvanced()->GetGameModeManager();
}

FGameModeBase* EEntity::GetGameMode() const
{
	return GetGameModeManager()->GetCurrentGameMode();
}

void EEntity::RegisterInputInternal()
{
	FEventHandler* InputHandler = GEngine->GetEventHandler();

	RegisterInput(InputHandler);
}

void EEntity::UnRegisterInputInternal()
{
	FEventHandler* InputHandler = GEngine->GetEventHandler();

	UnRegisterInput(InputHandler);
}

void EEntity::SetupAIActions()
{
}

void EEntity::OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent)
{
	IComponentManagerInterface::OnComponentCreated(ComponentName, NewComponent);

	// If we are missing component for root, try to find one
	if (DefaultRootComponent == nullptr)
	{
		// But only with specific base
		if (UParentComponent* ParentComponent = dynamic_cast<UParentComponent*>(NewComponent))
		{
			DefaultRootComponent = ParentComponent;
		}
	}
}

void EEntity::OnAttachedToEntity()
{
	if (EntityAttachmentRootComponent != nullptr)
	{
		EntityAttachmentRootComponent->OnLocationChangedDelegate.BindObject(this, &EEntity::OnAttachedComponentLocationChanged);
		EntityAttachmentRootComponent->OnRotationChangedDelegate.BindObject(this, &EEntity::OnAttachedComponentRotationChanged);
	}
}

void EEntity::OnDeAttachedFromEntity()
{
	if (EntityAttachmentRootComponent != nullptr)
	{
		EntityAttachmentRootComponent->OnLocationChangedDelegate.UnBindObject(this, &EEntity::OnAttachedComponentLocationChanged);
		EntityAttachmentRootComponent->OnRotationChangedDelegate.UnBindObject(this, &EEntity::OnAttachedComponentRotationChanged);
	}
}
void EEntity::OnAttachedComponentLocationChanged(const FTransform2DLocation& NewLocation)
{
	if (EntityAttachmentRootComponent != nullptr)
	{
		FVector2D<int> Rot = AttachmentRelativeLocation.Rotate(EntityAttachmentRootComponent->GetRotation());

		SetLocation(NewLocation + Rot);
	}
}

void EEntity::OnAttachedComponentRotationChanged(const FTransform2DRotation NewRotation)
{
	if (EntityAttachmentRootComponent != nullptr)
	{
		FVector2D<int> Rot = AttachmentRelativeLocation.Rotate(EntityAttachmentRootComponent->GetRotation());

		SetLocation(EntityAttachmentRootComponent->GetLocation() + Rot);

		SetRotation(NewRotation + AttachmentRelativeRotation);
	}
}
