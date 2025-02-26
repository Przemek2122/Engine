// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "ECS/SubSystems/SubSystemManagerInterface.h"
#include "Widgets/WidgetManager.h"

class FWindowInputManager;
class FMapManager;
class FEntityManager;
class FWidgetInputManager;
class FWidgetManager;
class FWidget;

#if WITH_WIDGET_DEBUGGER
class FWidgetDebugger;
#endif

#define WINDOW_DEFAULT_FLAGS (SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY)

/**
 * Window class. Has SDL_Window and FRender.
 * This class have ISubSystemManagerInterface for implementing subsystems
 */
class ENGINE_API FWindow : public ISubSystemManagerInterface
{
	friend FEngineRender;

protected:
	/**
	 * Creates SDL Window.
	 * Take a look here for available flags (or at SDL_vide.h): https://wiki.libsdl.org/SDL_WindowFlags
	 */
	FWindow(const std::string& InName, FVector2D<int32> InLocation, const FVector2D<int> InSize, Uint32 InWindowFlags = WINDOW_DEFAULT_FLAGS);
	~FWindow() override;

	virtual void Init();
	virtual void DeInit();

	void ReceiveTick();

	NO_DISCARD virtual FWidgetManager* CreateWidgetManager();
#if WITH_WIDGET_DEBUGGER
	NO_DISCARD virtual FWidgetDebugger* CreateWidgetDebugger();
#endif
	NO_DISCARD virtual FWindowInputManager* CreateWindowInputManager();
	NO_DISCARD virtual FWidgetInputManager* CreateWidgetInputManager();
	NO_DISCARD virtual FMapManager* CreateMapManager();
	NO_DISCARD virtual FRenderer* CreateRenderer();

public:
	/** Render this window using renderer. */
	virtual void Tick();
	/** Render this window using renderer. */
	virtual void Render();

#if WITH_WIDGET_DEBUGGER
	/** Call to enable debugger for widgets */
	virtual void StartWidgetDebugger();
#endif

	/** Call to change window size. */
	void SetWindowSize(const int X, const int Y, const bool bUpdateSDL = true);

	std::string GetWindowName() const;

	NO_DISCARD FRenderer* GetRenderer() const;

	/** @returns Window size in px. */
	NO_DISCARD FVector2D<int> GetWindowSize() const;
	/** @returns Window percent at given location. Window { 100, 200 } with position { 50, 50 } will be { 0.5, 0.25 }. */
	NO_DISCARD FVector2D<float> GetWindowSizePercent(const FVector2D<int> Position) const;

	NO_DISCARD FVector2D<int> GetWindowLocation() const;

	virtual void OnWindowMadeVisible();
	virtual void OnWindowMadeInvisible();

	virtual void OnWindowCloseRequested();

	virtual void OnWindowSizeChanged(Sint32 X, Sint32 Y);

	void SetWindowFocus(const bool bInNewFocus);
	void SetWindowForcedFocus(const bool bInNewFocus);
	void SetWindowIsMouseInside(const bool bInIsWindowMouseInside);

	NO_DISCARD SDL_Window* GetSdlWindow() const { return Window; }
	NO_DISCARD Uint32 GetWindowId() const { return WindowId; }
	NO_DISCARD bool IsWindowFocused() const { return bIsWindowFocused && bIsWindowMouseInside; }
	NO_DISCARD bool IsWindowMouseInside() const { return bIsWindowMouseInside; }

	NO_DISCARD FMapManager* GetMapManager() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TMapManagerClass>
	TMapManagerClass* GetMapManager() const
	{
		return static_cast<TMapManagerClass>(GetMapManager());
	}

	NO_DISCARD FWidgetManager* GetWidgetManager() const;
	NO_DISCARD FWindowInputManager* GetWindowInputManager() const;
	NO_DISCARD FWidgetInputManager* GetWidgetInputManager() const;

	template<class TWidgetTemplate>
	NO_DISCARD TWidgetTemplate* CreateWidget() const
	{
		return GetWidgetManager()->CreateWidget<TWidgetTemplate>();
	}
	template<class TWidgetTemplate>
	NO_DISCARD TWidgetTemplate* CreateWidget(const std::string& WidgetName) const
	{
		return GetWidgetManager()->CreateWidget<TWidgetTemplate>(WidgetName);
	}
	bool DestroyWidget(FWidget* Widget) const
	{
		return GetWidgetManager()->DestroyChildWidget(Widget);
	}

protected:
	SDL_Window* Window;
	FRenderer* Renderer;

	std::string WindowName;
	FVector2D<int> Size;
	Uint32 WindowFlags;
	Uint32 WindowId;

	bool bIsWindowFocused;
	bool bIsWindowVisible;
	bool bIsWindowMouseInside;

	FWidgetManager* WidgetManager;
#if WITH_WIDGET_DEBUGGER
	FWidgetDebugger* WidgetDebugger;
#endif
	FWindowInputManager* WindowInputManager;
	FWidgetInputManager* WidgetInputManager;

	FMapManager* MapManager;

};