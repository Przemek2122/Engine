#pragma once

#include "CoreMinimal.h"
#include "ECS/Entity.h"

enum class EInputState;
class IScreenSelectionInterface;

/** Defines how we select objects */
enum class ESelectionType
{
	/** Default, allows selection of single (click) or many objects (click and drag) and moving them with another button */
	SelectMultipleObject,
	/** Used when there is a need of moving and object */
	DragAndDrop
};

/** Defines current drag and drop operation state */
enum class EScreenDragState
{
	None,

	/** Used when starting drag */
	Start,
	/** Used after initial Start of drag */
	Updating,
	/** Used at end of drag */
	End
};

/**
 * Entity for managing selectable objects
 */
class ENGINE_API EScreenSelectionEntity : public EEntity
{
public:
	EScreenSelectionEntity(FEntityManager* InEntityManager);
	~EScreenSelectionEntity() override = default;

	/** Begin EEntity */
	void EndPlay() override;
	/** End EEntity */

	virtual void SetSelectionType(ESelectionType InSelectionType);

	virtual void RegisterScreenSelectable(IScreenSelectionInterface* InScreenSelectable);
	virtual void UnRegisterScreenSelectable(IScreenSelectionInterface* InScreenSelectable);

	virtual bool OnMouseMove(FVector2D<int32> InMousePosition, EInputState InputState);
	virtual bool OnMouseLeftClick(FVector2D<int32> InMousePosition, EInputState InputState);
	virtual bool OnMouseRightClick(FVector2D<int32> InMousePosition, EInputState InputState);

	/** Called when selection is started */
	virtual void OnStartSelecting();

	/** Called when selection is finished */
	virtual void OnEndSelecting();

	/** Called when should select but it was in place - click */
	virtual void OnClickInsteadOfSelection(const FVector2D<int32>& InMousePositionConverted);

	/** Returns true if area between Selection Start and End is covering SelectionTarget (does not need to fully cover it) */
	static bool IsInSelectionArea(const FVector2D<int32>& InSelectionTargetStart, const FVector2D<int32>& InSelectionTargetSize, const FVector2D<int32>& InSelectionStart, const FVector2D<int32>& InSelectionEnd);

	const CArray<IScreenSelectionInterface*>& GetCurrentlySelectedObjects() const;

	/** Delegate called on EndPlay (destroy this entity) - Used in IScreenSelectionInterface */
	FDelegate<> OnEndPlay;

protected:
	/** Begin EEntity */
	void RegisterInput(FEventHandler* InputHandler) override;
	void UnRegisterInput(FEventHandler* InputHandler) override;
	/** End EEntity */

	void CheckScreenSelection(const FVector2D<int32>& InMousePositionConverted);
	void StartScreenDrag(const FVector2D<int32>& InMousePositionConverted);
	void ContinueScreenDrag(const FVector2D<int32>& InMousePositionConverted);
	void EndScreenDrag(const FVector2D<int32>& InMousePositionConverted);

	virtual void AddToCurrentlySelectedObjects(IScreenSelectionInterface* InScreenSelectable);
	virtual void RemoveFromCurrentlySelectedObjects(IScreenSelectionInterface* InScreenSelectable);

	FVector2D<int32> ConvertLocationFromScreenSpace(const FVector2D<int32>& InLocation) const;

protected:
	/** Array with screen selectable items */
	CArray<IScreenSelectionInterface*> ScreenSelectableObjects;

	/** Currently selected objects list */
	CArray<IScreenSelectionInterface*> CurrentlySelectedObjects;

	/** Selecting when left mouse button is pressed */
	bool bIsSelecting;

	/** Defines how far can it be from click to release to be click instead of selection */
	int32 ClickInsteadOfSelectionTolerance;

	/** Start of the selection */
	FVector2D<int32> SelectionStart;

	/** How should we select objects */
	ESelectionType SelectionType;

	/** What is current state of drag and drop operation? */
	EScreenDragState ScreenDragState;

};
