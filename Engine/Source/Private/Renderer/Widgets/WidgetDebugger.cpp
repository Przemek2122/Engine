// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Renderer/Widgets/WidgetDebugger.h"

#if WITH_WIDGET_DEBUGGER
#include "Renderer/Widgets/Samples/ButtonWidget.h"
#include "Renderer/Widgets/Samples/TextWidget.h"
#include "Renderer/Widgets/Samples/VerticalBoxWidget.h"

FWidgetDebugger::FWidgetDebugger(FWindow* InWindow)
	: Window(InWindow)
	, DebuggerWindow(nullptr)
	, CurrentlyDebuggedWidget(nullptr)
{
}

void FWidgetDebugger::StartDebugger()
{
	FWidgetManager* WidgetManager = Window->GetWidgetManager();
	if (WidgetManager != nullptr)
	{
		FWindowCreationData WindowCreationData(true);

		static const std::string ColonDebugger = " : Widget Debugger";
		const std::string WindowName = Window->GetWindowName() + ColonDebugger;

		static const FVector2D<int> WindowLocation = Window->GetWindowLocation() + FVector2D<int>(Window->GetWindowSize().X, 0);
		static const FVector2D<int> WindowDefaultSize = FVector2D<int>(400, 600);

		DebuggerWindow = GEngine->GetEngineRender()->CreateWindow<FWindow>(WindowCreationData, WindowName, WindowLocation, WindowDefaultSize);
		if (DebuggerWindow != nullptr)
		{
			// Refresh all widgets in debugger
			RefreshDisplayedWidgets();

			// Bind to refresh on each changed of widget
			WidgetManager->OnAnyChildChangedDelegate.BindObject(this, &FWidgetDebugger::RefreshDisplayedWidgets);
		}
	}
}

void FWidgetDebugger::RefreshDisplayedWidgets()
{
	FWidgetManager* WindowWidgetManager = Window->GetWidgetManager();
	FWidgetManager* DebuggerWindowWidgetManager = DebuggerWindow->GetWidgetManager();

	if (WindowWidgetManager != nullptr && DebuggerWindowWidgetManager != nullptr)
	{
		DebuggerWindowWidgetManager->ClearChildren();

		if (CurrentlyDebuggedWidget != nullptr && !CurrentlyDebuggedWidget->IsPendingDelete())
		{
			CurrentlyDebuggedWidget->SetIsWidgetBeingDebugged(false);
			CurrentlyDebuggedWidget = nullptr;
		}

		FVerticalBoxWidget* VerticalBox = DebuggerWindowWidgetManager->CreateWidget<FVerticalBoxWidget>();
		VerticalBox->SetVerticalBoxAlignMethod(EVerticalBoxAlignMethod::AlignToLeft);
		VerticalBox->SetWidgetSizePercent({ 1, 1 });

		CArray<FWidget*> Widgets = WindowWidgetManager->GetManagedWidgets();

		CreateDebuggersForWidgets(VerticalBox, Widgets, 0);
	}
}

void FWidgetDebugger::CreateDebuggersForWidgets(FVerticalBoxWidget* InVerticalBox, const CArray<FWidget*>& InWidgets, int32 Depth)
{
	static const std::string DepthString = "^ ";

	for (FWidget* Widget : InWidgets)
	{
		std::string WidgetDepthNote;
		const std::string WidgetName = Widget->GetName();

		if (Depth == 0)
		{
			// R means it's root due to 0 depth
			WidgetDepthNote = 'R';
		}
		else
		{
			for (int32 i = 0; i < Depth; i++)
			{
				WidgetDepthNote += DepthString;
			}
		}
		
		std::string FinalWidgetName = WidgetDepthNote;
		FinalWidgetName += " - ";
		FinalWidgetName += WidgetName;

		FButtonWidget* Button = InVerticalBox->CreateWidget<FButtonWidget>();
		Button->OnLeftClickRelease.BindLambda([&, Widget]()
		{
			CreateSingleDebuggerForWidget(Widget);
		});

		FTextWidget* TextWidget = Button->CreateWidget<FTextWidget>();
		TextWidget->SetAnchor(EAnchor::LeftCenter);
		TextWidget->SetText(FinalWidgetName);

		CArray<FWidget*> Widgets = Widget->GetManagedWidgets();

		CreateDebuggersForWidgets(InVerticalBox, Widgets, Depth + 1);
	}
}

void FWidgetDebugger::CreateSingleDebuggerForWidget(FWidget* Widget)
{
	LOG_DEBUG("Widget selected for debugging: " << Widget->GetName());

	FWidgetManager* DebuggerWindowWidgetManager = DebuggerWindow->GetWidgetManager();
	if (DebuggerWindowWidgetManager != nullptr)
	{
		CurrentlyDebuggedWidget = Widget;
		CurrentlyDebuggedWidget->SetIsWidgetBeingDebugged(true);

		DebuggerWindowWidgetManager->ClearChildren();
		FVerticalBoxWidget* VerticalBox = DebuggerWindowWidgetManager->CreateWidget<FVerticalBoxWidget>();

		VerticalBox->CreateWidget<FTextWidget>()->SetText("Currently debugging: ");
		VerticalBox->CreateWidget<FTextWidget>()->SetText(Widget->GetName());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Location: " << Widget->GetWidgetLocation());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Size: " << Widget->GetWidgetSize());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Order: " << Widget->GetWidgetOrder());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Visibility: " << Widget->GetWidgetVisibilityAsString());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Interaction: " << Widget->GetWidgetInteractionAsString());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("Children count: " << Widget->GetChildrenCount());
		VerticalBox->CreateWidget<FTextWidget>()->SET_TEXT_ADV("NeedsWidgetRebuild: " << Widget->NeedsWidgetRebuild());

		FButtonWidget* Button = VerticalBox->CreateWidget<FButtonWidget>("Button_Rebuild");
		Button->CreateWidget<FTextWidget>()->SetText("Rebuild");
		Button->OnLeftClickRelease.BindLambda([&, Widget]()
		{
			Widget->RequestWidgetRebuild();
		});

		FButtonWidget* ButtonExit = VerticalBox->CreateWidget<FButtonWidget>("Button_Exit");
		ButtonExit->CreateWidget<FTextWidget>()->SetText("Exit");
		ButtonExit->OnLeftClickRelease.BindLambda([&, Widget]()
		{
			RefreshDisplayedWidgets();
		});
	}
}
#endif