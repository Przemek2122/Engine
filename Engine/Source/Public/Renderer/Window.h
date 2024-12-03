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

#define WINDOW_DEFAULT_FLAGS (SDL_WINDOW_RESIZABLE)

/**
 * Window class. Has SDL_Window and FRender.
 * This class have ISubSystemManagerInterface for implementing subsystems
 */
class FWindow : public ISubSystemManagerInterface
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

	_NODISCARD virtual FWidgetManager* CreateWidgetManager();
#if WITH_WIDGET_DEBUGGER
	_NODISCARD virtual FWidgetDebugger* CreateWidgetDebugger();
#endif
	_NODISCARD virtual FWindowInputManager* CreateWindowInputManager();
	_NODISCARD virtual FWidgetInputManager* CreateWidgetInputManager();
	_NODISCARD virtual FMapManager* CreateMapManager();
	_NODISCARD virtual FRenderer* CreateRenderer();

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

	_NODISCARD FRenderer* GetRenderer() const;

	/** @returns Window size in px. */
	_NODISCARD FVector2D<int> GetWindowSize() const;
	/** @returns Window percent at given location. Window { 100, 200 } with position { 50, 50 } will be { 0.5, 0.25 }. */
	_NODISCARD FVector2D<float> GetWindowSizePercent(const FVector2D<int> Position) const;

	_NODISCARD FVector2D<int> GetWindowLocation() const;

	virtual void OnWindowMadeVisible();
	virtual void OnWindowMadeInvisible();

	virtual void OnWindowSizeChanged(Sint32 X, Sint32 Y);

	void SetWindowFocus(const bool bInNewFocus);
	void SetWindowIsMouseInside(const bool bInIsWindowMouseInside);

	_NODISCARD SDL_Window* GetSdlWindow() const { return Window; }
	_NODISCARD Uint32 GetWindowId() const { return WindowId; }
	_NODISCARD bool IsWindowFocused() const { return bIsWindowFocused && bIsWindowMouseInside; }
	_NODISCARD bool IsWindowMouseInside() const { return bIsWindowMouseInside; }

	_NODISCARD FMapManager* GetMapManager() const;

	/** Use this if you changed to your own. Will return casted. */
	template<typename TMapManagerClass>
	TMapManagerClass* GetMapManager() const
	{
		return static_cast<TMapManagerClass>(GetMapManager());
	}

	_NODISCARD FWidgetManager* GetWidgetManager() const;
	_NODISCARD FWindowInputManager* GetWindowInputManager() const;
	_NODISCARD FWidgetInputManager* GetWidgetInputManager() const;

	template<class TWidgetTemplate>
	_NODISCARD TWidgetTemplate* CreateWidget() const
	{
		return GetWidgetManager()->CreateWidget<TWidgetTemplate>();
	}
	template<class TWidgetTemplate>
	_NODISCARD TWidgetTemplate* CreateWidget(const std::string& WidgetName) const
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