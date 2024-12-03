// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "ECS/Entities/CameraManager.h"

#include "Input/WindowInputManager.h"
#include "Renderer/Map/MapManager.h"

ECameraManager::ECameraManager(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, WindowMapManager(nullptr)
	, bIsRightMouseButtonPressed(false)
{
}

void ECameraManager::BeginPlay()
{
	EEntity::BeginPlay();

	FMapManager* MapManager = GetWindow()->GetMapManager();
	if (MapManager != nullptr)
	{
		WindowMapManager = MapManager;
	}

	RegisterInput();
}

void ECameraManager::EndPlay()
{
	EEntity::EndPlay();

	UnregisterInput();
}

void ECameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ECameraManager::RegisterInput()
{
	FWindowInputManager* WindowInputManager = GetWindow()->GetWindowInputManager();
	if (WindowInputManager != nullptr)
	{
		WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.BindObject(this, &ECameraManager::OnMouseMove);

		WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.BindObject(this, &ECameraManager::OnMouseRightClick);
	}
}

void ECameraManager::UnregisterInput()
{
	FWindowInputManager* WindowInputManager = GetWindow()->GetWindowInputManager();
	if (WindowInputManager != nullptr)
	{
		WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.UnBindObject(this, &ECameraManager::OnMouseMove);

		WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.UnBindObject(this, &ECameraManager::OnMouseRightClick);
	}
}

bool ECameraManager::OnMouseMove(const FVector2D<int> CurrentMouseLocation, EInputState)
{
	bool bWasInputConsumed = false;

	if (bIsRightMouseButtonPressed)
	{
		if (WindowMapManager != nullptr)
		{
			const FVector2D<int> LocationChange = CurrentMouseLocation - LastMouseLocation;

			WindowMapManager->MoveMap(LocationChange);

			bWasInputConsumed = true;
		}
	}

	LastMouseLocation = CurrentMouseLocation;

	return bWasInputConsumed;
}

bool ECameraManager::OnMouseRightClick(const FVector2D<int> CurrentMouseLocation, const EInputState InputState)
{
	bool bWasInputConsumed = false;

	if (InputState == EInputState::PRESS)
	{
		bIsRightMouseButtonPressed = true;

		LastMouseLocation = CurrentMouseLocation;
	}
	else if (InputState == EInputState::RELEASE)
	{
		bIsRightMouseButtonPressed = false;
	}

	return bWasInputConsumed;
}
