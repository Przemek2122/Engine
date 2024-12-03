#pragma once

#include "CoreMinimal.h"

class EScreenSelectionEntity;

/**
 * Objects using this interface can be selected by screen selection
 */
class IScreenSelectionInterface
{
public:
	IScreenSelectionInterface();
	virtual ~IScreenSelectionInterface();

	/** You need to call register */
	void RegisterToScreenSelection(const FEntityManager* InEntityManager);

	/** Unregister does not have to be called */
	void UnregisterFromScreenSelection();

	/** Use to check if unit can be selected */
	bool NativeCanBeSelected();

	void NativeSelect();

	void NativeDeselect();

	void NativeDoAction(const FVector2D<int>& ActionLocation);

	/** Override to return location of selection */
	virtual FVector2D<int32> GetLocation() = 0;

	/** Override to return size of selection */
	virtual FVector2D<int32> GetSize() = 0;

protected:
	/** Use to determine if object can be selected */
	virtual bool CanSelect() { return true; }

	/** Called when object is selected (once) */
	virtual void OnSelect() = 0;

	/** Called when object is de-selected (once) */
	virtual void OnDeSelect() = 0;

	/** Optional action - By default it will be called when selected and right clicked - Move / Attack */
	virtual void OnDoAction(const FVector2D<int>& ActionLocation);

	virtual void RegisterToScreenSelectionInternal();

protected:
	EScreenSelectionEntity* ScreenSelectionEntity;

	bool bIsRegistered;
	bool bIsSelected;

};
