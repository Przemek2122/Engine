#pragma once

#include "CoreMinimal.h"

class FWindowAdvanced;
enum class EInputState;
class FRTSGameEngine;
class FVerticalBoxWidget;

/**
 * UI Menu for pause menu
 *
 * @Note: To ACTIVATE BINDING a call to InitializePublic(from anywhere)
 * or OnBindDelegateRequested(in constructor - this function will trigger InitializePublic) is required
 */
class FPauseUIMenu : public FUIMenu
{
public:
	FPauseUIMenu(FWindow* InGameWindow);

	/** Default is escape but can be anything - Use OnBindDelegateRequested and OnUnbindDelegateRequested to change. */
	bool OnExitButtonPressed(EInputState InputState);

	void OnExitToMenuRequested();

	/** Called from OnExitToMenuRequested when user wants to exit to menu */
	virtual void OnExitToMenu(FWindowAdvanced* WindowAdvanced);

	FVerticalBoxWidget* GetVerticalBoxWidget() const;

protected:
	/** Begin FUIMenu */
	void Initialize() override;
	void DeInitialize() override;
	/** End FUIMenu */

	/** Override without calling parent to create custom logic for your menu. By default it will create back to menu button. */
	virtual void CreateMenuInVerticalBox(FVerticalBoxWidget* InVerticalBoxWidget);

	void DisablePauseMenu();
	void EnablePauseMenu();

	void Show();
	void Hide();

	virtual void OnMenuShown();
	virtual void OnMenuHidden();

	/** Binding button - Call for custom behaviour on enabling Menu - Default is on ESCAPE button, here you can change it */
	virtual void OnBindDelegateRequested();

	/** UnBinding button - Call for custom behaviour on enabling Menu - Default is on ESCAPE button, here you can change it */
	virtual void OnUnbindDelegateRequested();

private:
	/** Vertical box with menu */
	FVerticalBoxWidget* VerticalBoxWidget;

	/** Defines visibility of current menu */
	bool bIsVisible;

};
