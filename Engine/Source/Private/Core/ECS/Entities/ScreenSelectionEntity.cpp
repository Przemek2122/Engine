#include "CoreEngine.h"
#include "ECS/Entities/ScreenSelectionEntity.h"
#include "ECS/Interfaces/ScreenSelectionInterface.h"
#include "Input/EventHandler.h"
#include "Input/WindowInputManager.h"
#include "Renderer/Map/Map.h"

EScreenSelectionEntity::EScreenSelectionEntity(FEntityManager* InEntityManager)
	: EEntity(InEntityManager)
	, bForcedEndInteraction(false)
	, bIsSelecting(false)
	, ClickInsteadOfSelectionTolerance(2)
	, SelectionType(ESelectionType::SelectMultipleObject)
	, ScreenDragState(EScreenDragState::None)
{
}

void EScreenSelectionEntity::EndPlay()
{
	OnEndPlay.Execute();

	EEntity::EndPlay();
}

void EScreenSelectionEntity::Tick(float DeltaTime)
{
	EEntity::Tick(DeltaTime);

	if (bForcedEndInteraction)
	{
		switch (SelectionType)
		{
			case ESelectionType::SelectMultipleObject:
			{
				// Not implemented
				ENSURE_VALID(false);

				break;
			}
			case ESelectionType::DragAndDrop:
			{
				if (ScreenDragState == EScreenDragState::Updating)
				{
					ScreenDragState = EScreenDragState::End;

					EndScreenDrag(FGlobalDefines::GEngine->GetEventHandler()->GetMouseLocationCurrent());
				}

				break;
			}
		}

		bForcedEndInteraction = false;
	}
}

void EScreenSelectionEntity::Render()
{
	EEntity::Render();

#if ENGINE_ALLOW_ENTITY_SCREEN_SELECTION_DEBUG
	FRenderer* Renderer = GetWindow()->GetRenderer();

	for (IScreenSelectionInterface* ScreenSelectable : ScreenSelectableObjects)
	{
		Renderer->DrawRectangleOutline(ScreenSelectable->GetScreenSelectionLocation(), ScreenSelectable->GetScreenSelectionSize(), FColorRGBA::ColorDodgerBlue());
	}
#endif
}

void EScreenSelectionEntity::ForceEndInteraction()
{
	bForcedEndInteraction = true;
}

void EScreenSelectionEntity::SetSelectionType(ESelectionType InSelectionType)
{
	SelectionType = InSelectionType;

	CurrentlySelectedObjects.Clear();
}

void EScreenSelectionEntity::RegisterScreenSelectable(IScreenSelectionInterface* InScreenSelectable)
{
	ScreenSelectableObjects.Push(InScreenSelectable);
}

void EScreenSelectionEntity::UnRegisterScreenSelectable(IScreenSelectionInterface* InScreenSelectable)
{
	ScreenSelectableObjects.Remove(InScreenSelectable);
}

bool EScreenSelectionEntity::OnMouseMove(FVector2D<int32> InMousePosition, EInputState InputState)
{
	bool bWasInputConsumed = false;

	switch (SelectionType)
	{
		case ESelectionType::SelectMultipleObject:
		{
			const FVector2D<int32> InMousePositionConverted = ConvertLocationFromScreenSpace(InMousePosition);

			if (bIsSelecting)
			{
				CheckScreenSelection(InMousePositionConverted);
			}

			break;
		}
		case ESelectionType::DragAndDrop:
		{
			const FVector2D<int32> InMousePositionConverted = ConvertLocationFromScreenSpace(InMousePosition);

			if (ScreenDragState == EScreenDragState::Updating)
			{
				ContinueScreenDrag(InMousePositionConverted);
			}

			break;
		}
	}

	return bWasInputConsumed;
}

bool EScreenSelectionEntity::OnMouseLeftClick(FVector2D<int32> InMousePosition, EInputState InputState)
{
	bool bWasInputConsumed = false;

	switch (SelectionType)
	{
		case ESelectionType::SelectMultipleObject:
		{
			const FVector2D<int32> InMousePositionConverted = ConvertLocationFromScreenSpace(InMousePosition);

			switch (InputState)
			{
				case EInputState::PRESS:
				{
					SelectionStart = InMousePositionConverted;

					OnStartSelecting();

					bWasInputConsumed = true;

					break;
				}

				case EInputState::RELEASE:
				{
					OnEndSelecting();

					if (SelectionStart.DistanceTo(InMousePositionConverted) < ClickInsteadOfSelectionTolerance)
					{
						OnClickInsteadOfSelection(InMousePositionConverted);
					}

					bWasInputConsumed = true;

					break;
				}

				default:
				{
					LOG_ERROR("EScreenSelectionEntity::OnMouseLeftClick: Unknown InputState");
				}
			}

			break;
		}
		case ESelectionType::DragAndDrop:
		{
			const FVector2D<int32> InMousePositionConverted = ConvertLocationFromScreenSpace(InMousePosition);

			switch (InputState)
			{
				case EInputState::PRESS:
				{
					ScreenDragState = EScreenDragState::Start;

					StartScreenDrag(InMousePositionConverted);

					bWasInputConsumed = true;

					break;
				}

				case EInputState::RELEASE:
				{
					if (ScreenDragState == EScreenDragState::Updating)
					{
						ScreenDragState = EScreenDragState::End;

						EndScreenDrag(InMousePositionConverted);

						bWasInputConsumed = true;
					}

					break;
				}
			}
		}
	}

	return bWasInputConsumed;
}

bool EScreenSelectionEntity::OnMouseRightClick(FVector2D<int32> InMousePosition, EInputState InputState)
{
	bool bWasInputConsumed = false;
	switch (SelectionType)
	{
		case ESelectionType::SelectMultipleObject:
		{
			const FVector2D<int32> InMousePositionConverted = ConvertLocationFromScreenSpace(InMousePosition);

			switch (InputState)
			{
				case EInputState::PRESS:
				{
					FMap* CurrentMap = GetCurrentMap();
					if (CurrentMap != nullptr)
					{
						for (IScreenSelectionInterface* CurrentlySelectedObject : CurrentlySelectedObjects)
						{
							CurrentlySelectedObject->NativeDoAction(InMousePositionConverted);
						}

						bWasInputConsumed = true;
					}

					break;
				}
			}

			break;
		}
		case ESelectionType::DragAndDrop:
		{

			break;
		}
	}

	return bWasInputConsumed;
}

void EScreenSelectionEntity::OnStartSelecting()
{
	bIsSelecting = true;
}

void EScreenSelectionEntity::OnEndSelecting()
{
	bIsSelecting = false;
}

void EScreenSelectionEntity::OnClickInsteadOfSelection(const FVector2D<int32>& InMousePositionConverted)
{
	// Update position as 
	SelectionStart = InMousePositionConverted;

	CheckScreenSelection(InMousePositionConverted);
}

bool EScreenSelectionEntity::IsInSelectionArea(const FVector2D<int32>& InSelectionTargetStart, const FVector2D<int32>& InSelectionTargetSize, const FVector2D<int32>& InSelectionStart, const FVector2D<int32>& InSelectionEnd)
{
	const bool bIsSelectedHorizontalToRight = InSelectionStart.X >= InSelectionTargetStart.X &&
		InSelectionEnd.X <= InSelectionTargetStart.X + InSelectionTargetSize.X;

	const bool bIsSelectedHorizontalToLeft = InSelectionEnd.X >= InSelectionTargetStart.X &&
		InSelectionStart.X <= InSelectionTargetStart.X + InSelectionTargetSize.X;

	const bool bIsSelectedVerticalToDown = InSelectionEnd.Y >= InSelectionTargetStart.Y &&
		InSelectionStart.Y <= InSelectionTargetStart.Y + InSelectionTargetSize.Y;

	const bool bIsSelectedVerticalToUp = InSelectionStart.Y >= InSelectionTargetStart.Y &&
		InSelectionEnd.Y <= InSelectionTargetStart.Y + InSelectionTargetSize.Y;

	return (bIsSelectedHorizontalToRight || bIsSelectedHorizontalToLeft) && (bIsSelectedVerticalToDown || bIsSelectedVerticalToUp);
}

const CArray<IScreenSelectionInterface*>& EScreenSelectionEntity::GetCurrentlySelectedObjects() const
{
	return CurrentlySelectedObjects;
}

void EScreenSelectionEntity::RegisterInput(FEventHandler* InputHandler)
{
	FWindowInputManager* WindowInputManager = GetWindow()->GetWindowInputManager();

	// Bind input
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.BindObject(this, &EScreenSelectionEntity::OnMouseMove);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_LEFT")->Delegate.BindObject(this, &EScreenSelectionEntity::OnMouseLeftClick);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.BindObject(this, &EScreenSelectionEntity::OnMouseRightClick);
}

void EScreenSelectionEntity::UnRegisterInput(FEventHandler* InputHandler)
{
	FWindowInputManager* WindowInputManager = GetWindow()->GetWindowInputManager();

	// Input must be unregistered, otherwise it might be called after the entity is destroyed
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_MOVE")->Delegate.UnBindObject(this, &EScreenSelectionEntity::OnMouseMove);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_LEFT")->Delegate.UnBindObject(this, &EScreenSelectionEntity::OnMouseLeftClick);
	WindowInputManager->MouseDelegates.GetMouseDelegateByName("MOUSE_BUTTON_RIGHT")->Delegate.UnBindObject(this, &EScreenSelectionEntity::OnMouseRightClick);
}

void EScreenSelectionEntity::CheckScreenSelection(const FVector2D<int32>& InMousePositionConverted)
{
	const FVector2D<int32> SelectionEnd = InMousePositionConverted;

	CurrentlySelectedObjects.Clear();

	for (ContainerInt i = 0; i < ScreenSelectableObjects.Size(); i++)
	{
		IScreenSelectionInterface* ScreenSelectable = ScreenSelectableObjects[i];

		const FVector2D<int32> ScreenSelectableLocation = ScreenSelectable->GetScreenSelectionLocation();
		const FVector2D<int32> ScreenSelectableSize = ScreenSelectable->GetScreenSelectionSize();

		if (ScreenSelectable->NativeCanBeSelected() && IsInSelectionArea(ScreenSelectableLocation, ScreenSelectableSize, SelectionStart, SelectionEnd))
		{
			ScreenSelectable->NativeSelect();

			AddToCurrentlySelectedObjects(ScreenSelectable);
		}
		else
		{
			ScreenSelectable->NativeDeselect();

			RemoveFromCurrentlySelectedObjects(ScreenSelectable);
		}
	}
}

void EScreenSelectionEntity::StartScreenDrag(const FVector2D<int32>& InMousePositionConverted)
{
	CurrentlySelectedObjects.Clear();

	SelectionStart = InMousePositionConverted;

	for (ContainerInt i = 0; i < ScreenSelectableObjects.Size(); i++)
	{
		IScreenSelectionInterface* ScreenSelectable = ScreenSelectableObjects[i];

		const FVector2D<int32> ScreenSelectableLocation = ScreenSelectable->GetScreenSelectionLocation();
		const FVector2D<int32> ScreenSelectableSize = ScreenSelectable->GetScreenSelectionSize();

		if (ScreenSelectable->NativeCanBeSelected() && IsInSelectionArea(ScreenSelectableLocation, ScreenSelectableSize, InMousePositionConverted, InMousePositionConverted))
		{
			CurrentlySelectedObjects.Push(ScreenSelectable);

			ScreenSelectable->NativeStartDragAction(SelectionStart);
		}
	}

	ScreenDragState = EScreenDragState::Updating;
}

void EScreenSelectionEntity::ContinueScreenDrag(const FVector2D<int32>& InMousePositionConverted)
{
	for (IScreenSelectionInterface* ScreenSelectable : CurrentlySelectedObjects)
	{
		ScreenSelectable->NativeUpdateDragAction(SelectionStart, InMousePositionConverted);
	}
}

void EScreenSelectionEntity::EndScreenDrag(const FVector2D<int32>& InMousePositionConverted)
{
	for (IScreenSelectionInterface* ScreenSelectable : CurrentlySelectedObjects)
	{
		ScreenSelectable->NativeEndDragAction(SelectionStart, InMousePositionConverted);
	}

	CurrentlySelectedObjects.Clear();

	ScreenDragState = EScreenDragState::None;
}

void EScreenSelectionEntity::AddToCurrentlySelectedObjects(IScreenSelectionInterface* InScreenSelectable)
{
	CurrentlySelectedObjects.Push(InScreenSelectable);
}

void EScreenSelectionEntity::RemoveFromCurrentlySelectedObjects(IScreenSelectionInterface* InScreenSelectable)
{
	CurrentlySelectedObjects.Remove(InScreenSelectable);
}

FVector2D<int32> EScreenSelectionEntity::ConvertLocationFromScreenSpace(const FVector2D<int32>& InLocation) const
{
	FMap* Map = GetCurrentMap();

	return (InLocation - Map->GetMapRenderOffset());
}
