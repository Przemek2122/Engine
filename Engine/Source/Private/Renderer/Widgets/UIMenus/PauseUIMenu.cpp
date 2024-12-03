#include "CoreEngine.h"
#include "Renderer/Widgets/UIMenus/PauseUIMenu.h"

#include "Input/WindowInputManager.h"
#include "Renderer/WindowAdvanced.h"
#include "Renderer/Widgets/Samples/ButtonWidget.h"
#include "Renderer/Widgets/Samples/TextWidget.h"
#include "Renderer/Widgets/Samples/VerticalBoxWidget.h"

FPauseUIMenu::FPauseUIMenu(FWindow* InGameWindow)
	: FUIMenu(InGameWindow)
	, VerticalBoxWidget(nullptr)
	, bIsVisible(false)
{
}

void FPauseUIMenu::Initialize()
{
	static const std::string PauseVerticalBoxName = "VerticalBox_PauseUI";

	OnBindDelegateRequested();

	// Create VerticalBoxWidget
	VerticalBoxWidget = GetOwnerWindow()->CreateWidget<FVerticalBoxWidget>(PauseVerticalBoxName);
	VerticalBoxWidget->SetAnchor(EAnchor::Center);

	// Create content
	CreateMenuInVerticalBox(VerticalBoxWidget);

	// Hide by default
	Hide();
}

void FPauseUIMenu::DeInitialize()
{
	OnUnbindDelegateRequested();

	VerticalBoxWidget->DestroyWidget();
}

bool FPauseUIMenu::OnExitButtonPressed(EInputState InputState)
{
	if (InputState == EInputState::RELEASE)
	{
		if (bIsVisible)
		{
			DisablePauseMenu();
		}
		else
		{
			EnablePauseMenu();
		}
	}

	return true;
}

void FPauseUIMenu::OnExitToMenuRequested()
{
	FWindowAdvanced* WindowAdvanced = GetOwnerWindow<FWindowAdvanced>();
	if (WindowAdvanced != nullptr)
	{
		OnExitToMenu(WindowAdvanced);
	}
	else
	{
		LOG_ERROR("WindowAdvanced is nullptr!");
	}
}

void FPauseUIMenu::OnExitToMenu(FWindowAdvanced* WindowAdvanced)
{
	LOG_DEBUG("Exit to menu requested");
}

FVerticalBoxWidget* FPauseUIMenu::GetVerticalBoxWidget() const
{
	return VerticalBoxWidget;
}

void FPauseUIMenu::DisablePauseMenu()
{
	bIsVisible = false;

	Hide();
}

void FPauseUIMenu::EnablePauseMenu()
{
	bIsVisible = true;

	Show();
}

void FPauseUIMenu::Show()
{
	if (IsInitialized())
	{
		VerticalBoxWidget->SetWidgetVisibility(EWidgetVisibility::Visible);

		OnMenuShown();
	}
	else
	{
		// Initialize if not initialized
		InitializePublic();
	}
}

void FPauseUIMenu::Hide()
{
	VerticalBoxWidget->SetWidgetVisibility(EWidgetVisibility::Hidden);

	OnMenuHidden();
}

void FPauseUIMenu::OnMenuShown()
{
}

void FPauseUIMenu::OnMenuHidden()
{
}

void FPauseUIMenu::CreateMenuInVerticalBox(FVerticalBoxWidget* InVerticalBoxWidget)
{
	static const std::string PauseButtonName = "Button_PauseUI";
	static const std::string PauseTextName = "Text_PauseUI";
	static const std::string BackToMenuText = "Back to menu";

	FButtonWidget* BackButton = VerticalBoxWidget->CreateWidget<FButtonWidget>(PauseButtonName);
	BackButton->UseDefaultSize();
	BackButton->OnLeftClickRelease.BindObject(this, &FPauseUIMenu::OnExitToMenuRequested);
	FTextWidget* TextWidget = BackButton->CreateWidget<FTextWidget>(PauseTextName);
	TextWidget->SetText(BackToMenuText);
}

void FPauseUIMenu::OnBindDelegateRequested()
{
	FWindowInputManager* WindowInputManager = GetOwnerWindow()->GetWindowInputManager();

	FInputDelegateWrapper* EscapeDelegate = WindowInputManager->KeyBoardDelegates.ButtonEscape.Get();
	EscapeDelegate->Delegate.BindObject(this, &FPauseUIMenu::OnExitButtonPressed);
}

void FPauseUIMenu::OnUnbindDelegateRequested()
{
	FWindowInputManager* WindowInputManager = GetOwnerWindow()->GetWindowInputManager();

	FInputDelegateWrapper* EscapeDelegate = WindowInputManager->KeyBoardDelegates.ButtonEscape.Get();
	EscapeDelegate->Delegate.UnBindObject(this, &FPauseUIMenu::OnExitButtonPressed);
}
