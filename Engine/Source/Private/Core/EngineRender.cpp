// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "EngineRender.h"

FWindowCreationData::FWindowCreationData(const bool bInIsThisWindowDebugger)
	: bIsThisWindowDebugger(bInIsThisWindowDebugger)
{
}

FEngineRender::FEngineRender()
	: bIsRenderTickFinished(true)
{
}

FEngineRender::~FEngineRender()
{
	for (int i = ManagedWindows.Size() - 1; i >= 0; i--)
	{
		DestroyWindow(ManagedWindows[i]);
	}
}

void FEngineRender::Tick()
{
	StartRenderTick();
	
	for (FWindow* Window : ManagedWindows.Vector)
	{
		Window->ReceiveTick();
	}
}

void FEngineRender::StartRenderTick()
{
	if (bIsRenderTickFinished)
	{
		bIsRenderTickFinished = false;

		// @TODO Move to thread.
		RenderTick();
	}
#if _DEBUG
	else
	{
		ENSURE_VALID(false);
	}
#endif
}

bool FEngineRender::IsRenderTickFinished() const
{
	return bIsRenderTickFinished;
}

void FEngineRender::AddWindow(FWindow* InWindow)
{
	ManagedWindows.Push(InWindow);

	WindowToIdMap.Emplace(SDL_GetWindowID(InWindow->GetSdlWindow()), InWindow);
}

void FEngineRender::RemoveWindow(FWindow* InWindow)
{
	ManagedWindows.Remove(InWindow);

	WindowToIdMap.Remove(InWindow->WindowId);
}

void FEngineRender::RenderTick()
{
	for (FWindow* Window : ManagedWindows.Vector)
	{
		if (Window != nullptr)
		{
			Window->Render();
		}
#ifdef _DEBUG
		else
		{
			ENSURE_VALID(false);
		}
#endif
	}

	EndRenderTick();
}

void FEngineRender::EndRenderTick()
{
	bIsRenderTickFinished = true;
}

void FEngineRender::DestroyWindow(FWindow* InWindow)
{
	RemoveWindow(InWindow);

	InWindow->DeInit();

	delete InWindow;
}

FWindow* FEngineRender::GetFocusedWindow() const
{
	for (ContainerInt i = 0; i < ManagedWindows.Size(); i++)
	{
		if (ManagedWindows[i]->IsWindowFocused())
		{
			return ManagedWindows[i];
		}
	}

	return nullptr;
}

FWindow* FEngineRender::GetWindowById(const Uint32 WindowId)
{
	if (WindowToIdMap.HasKey(WindowId))
	{
		return WindowToIdMap[WindowId];
	}

	return nullptr;
}

void FEngineRender::OnWindowExposed(const Uint32 WindowId)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		OnWindowMadeVisible(Window);
	}
}

void FEngineRender::OnWindowHidden(const Uint32 WindowId)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		OnWindowMadeInVisible(Window);
	}
}

void FEngineRender::OnWindowMoved(const Uint32 WindowId, const Sint32 X, const Sint32 Y)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		// @TODO it was removed in SDL3?
		//Window->SetWindowLocation(X, Y, false);
	}
}

void FEngineRender::OnWindowResized(const Uint32 WindowId, const Sint32 X, const Sint32 Y)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		Window->SetWindowSize(X, Y, false);
	}
}

void FEngineRender::OnWindowSizeChanged(const Uint32 WindowId, const Sint32 X, const Sint32 Y)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		Window->SetWindowSize(X, Y, false);
	}
}

void FEngineRender::OnWindowMinimized(const Uint32 WindowId)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		OnWindowMadeVisible(Window);
	}
}

void FEngineRender::OnWindowMaximized(const Uint32 WindowId)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		OnWindowMadeInVisible(Window);
	}
}

void FEngineRender::SetWindowFocus(const Uint32 WindowId, const bool bIsFocused)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		Window->SetWindowFocus(bIsFocused);
	}
}

void FEngineRender::SetWindowIsMouseInside(Uint32 WindowId, const bool bIsInside)
{
	FWindow* Window = GetWindowById(WindowId);
	if (Window != nullptr)
	{
		Window->SetWindowIsMouseInside(bIsInside);
	}
}

void FEngineRender::OnWindowMadeVisible(FWindow* Window)
{
	Window->OnWindowMadeVisible();
}

void FEngineRender::OnWindowMadeInVisible(FWindow* Window)
{
	Window->OnWindowMadeInvisible();
}

void FEngineRender::OnWindowSizeChanged(FWindow* Window, const Sint32 X, const Sint32 Y)
{
	Window->OnWindowSizeChanged(X, Y);
}

void FEngineRender::OnWindowLocationChanged(FWindow* Window, const Sint32 X, const Sint32 Y)
{
	Window->OnWindowSizeChanged(X, Y);
}
