// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Window.h"

#include "Input/WindowInputManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Map/MapManager.h"
#if WITH_WIDGET_DEBUGGER
#include "Renderer/Widgets/WidgetDebugger.h"
#endif
#include "Renderer/Widgets/WidgetInputManager.h"

FWindow::FWindow(const std::string& InName, FVector2D<int32> InLocation, const FVector2D<int> InSize, const Uint32 InWindowFlags)
	: Window(SDL_CreateWindow(InName.c_str(), InSize.X, InSize.Y, InWindowFlags))
	, Renderer(nullptr)
	, WindowName(InName)
	, Size(InSize)
	, WindowFlags(InWindowFlags)
	, bIsWindowFocused(false)
	, bIsWindowVisible(true)
	, bIsWindowMouseInside(true)
	, WidgetManager(nullptr)
#if WITH_WIDGET_DEBUGGER
	, WidgetDebugger(nullptr)
#endif
	, WindowInputManager(nullptr)
	, WidgetInputManager(nullptr)
	, MapManager(nullptr)
{
	if (Window != nullptr)
	{
		LOG_INFO("Window created. (" << WindowName << ")");

		WindowId = SDL_GetWindowID(Window);
	}
	else
	{
		LOG_ERROR("Can not create window: " << STRING(SDL_GetError()) << " ! (" << WindowName << ")");

		GEngine->ForceExit(EEngineErrorCode::CanNotCreateWindow);
	}
}

FWindow::~FWindow()
{
	delete Renderer;	
	delete WidgetManager;
#if WITH_WIDGET_DEBUGGER
	delete WidgetDebugger;
#endif
	delete WidgetInputManager;
	delete MapManager;

	if (Window != nullptr)
	{
		SDL_DestroyWindow(Window);

		LOG_INFO("Window destroyed. (" << WindowName << ")");
	}
	else
	{
		LOG_WARN("Window not destroyed (pointer invalid)! ("<< WindowName << ")");
	}
}

void FWindow::Init()
{
	Renderer = CreateRenderer();
	WidgetManager = CreateWidgetManager();
	WindowInputManager = CreateWindowInputManager();
	WidgetInputManager = CreateWidgetInputManager();
	MapManager = CreateMapManager();
}

void FWindow::DeInit()
{
	WidgetManager->DeInit();
}

void FWindow::ReceiveTick()
{
	if (bIsWindowVisible)
	{
		Tick();
	}

	TickSubSystems();
}

FWidgetManager* FWindow::CreateWidgetManager()
{
	return new FWidgetManager(this);
}

#if WITH_WIDGET_DEBUGGER
FWidgetDebugger* FWindow::CreateWidgetDebugger()
{
	return new FWidgetDebugger(this);
}
#endif

FWindowInputManager* FWindow::CreateWindowInputManager()
{
	return new FWindowInputManager(this);
}

FWidgetInputManager* FWindow::CreateWidgetInputManager()
{
	return new FWidgetInputManager(this);
}

FMapManager* FWindow::CreateMapManager()
{
	return new FMapManager(this);
}

FRenderer* FWindow::CreateRenderer()
{
	return new FRenderer(this);
}

void FWindow::Tick()
{
	const float DeltaTime = GEngine->GetDeltaTime();

	WidgetManager->ReceiveTick();
	MapManager->TickMap(DeltaTime);
}

void FWindow::Render()
{
	Renderer->PreRender();
	Renderer->Render();

	MapManager->DrawMap();
	WidgetManager->RenderWidgets();
	RenderSubSystems();

	Renderer->PostRender();
}

#if DEBUG
void FWindow::StartWidgetDebugger()
{
	WidgetDebugger = CreateWidgetDebugger();
	WidgetDebugger->StartDebugger();
}
#endif

void FWindow::SetWindowSize(const int X, const int Y, const bool bUpdateSDL)
{
	if (bUpdateSDL)
	{
		SDL_SetWindowSize(Window, X, Y);
	}

	Size.X = X;
	Size.Y = Y;

	CArray<FWidget*> Widgets = WidgetManager->GetManagedWidgets();
	for (FWidget* Widget : Widgets)
	{
		Widget->OnWindowChanged();
	}

	WidgetManager->RequestWidgetRebuild();
}

std::string FWindow::GetWindowName() const
{
	return WindowName;
}

FRenderer* FWindow::GetRenderer() const
{
	return Renderer;
}

FVector2D<int> FWindow::GetWindowSize() const
{
	FVector2D<int> Size;

	SDL_GetWindowSize(Window, &Size.X, &Size.Y);
	
	return Size;
}

FVector2D<float> FWindow::GetWindowSizePercent(const FVector2D<int> Position) const
{
	const FVector2D<int> WindowSize = GetWindowSize();

	return (FVector2D<float>(WindowSize) / FVector2D<float>(100)) * Position;
}

FVector2D<int> FWindow::GetWindowLocation() const
{
	FVector2D<int> WindowLocation;
	
	SDL_GetWindowPosition(Window, &WindowLocation.X, &WindowLocation.Y);

	return WindowLocation;
}

void FWindow::OnWindowMadeVisible()
{
	bIsWindowVisible = true;
}

void FWindow::OnWindowMadeInvisible()
{
	bIsWindowVisible = false;
}

void FWindow::OnWindowSizeChanged(const Sint32 X, const Sint32 Y)
{
	SetWindowSize(X, Y, false);
}

void FWindow::SetWindowFocus(const bool bInNewFocus)
{
	bIsWindowFocused = bInNewFocus;

	// @TODO Could be forced to focus but unsure if that's needed
}

void FWindow::SetWindowIsMouseInside(const bool bInIsWindowMouseInside)
{
	bIsWindowMouseInside = bInIsWindowMouseInside;
}

FMapManager* FWindow::GetMapManager() const
{
	return MapManager;
}

FWidgetManager* FWindow::GetWidgetManager() const
{
	return WidgetManager;
}

FWindowInputManager* FWindow::GetWindowInputManager() const
{
	return WindowInputManager;
}

FWidgetInputManager* FWindow::GetWidgetInputManager() const
{
	return WidgetInputManager;
}
