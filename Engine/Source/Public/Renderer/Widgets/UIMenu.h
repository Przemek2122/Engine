#pragma once

#include "CoreMinimal.h"

/**
 * Class for UI widgets
 * Like MainMenu, PauseMenu, etc.
 */
class FUIMenu
{
public:
	/** Window is very important for widgets as it stores widget that we can later add more widgets to. */
	FUIMenu(FWindow* InOwnerWindow);
	virtual ~FUIMenu();

	FWindow* GetOwnerWindow() const;

	template<typename TWindowClass>
	TWindowClass* GetOwnerWindow() const
	{
		return static_cast<TWindowClass*>(OwnerWindow);
	}

	void InitializePublic();
	void DeInitializePublic();

	bool IsInitialized() const;

protected:
	/** Generates default menu layout */
	virtual void Initialize();

	/** Clears widgets and calls delegate */
	virtual void DeInitialize();

private:
	bool bIsInitializedUIMenu;
	FWindow* OwnerWindow;
};
