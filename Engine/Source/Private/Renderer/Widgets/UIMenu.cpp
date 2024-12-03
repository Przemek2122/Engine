#include "CoreEngine.h"
#include "Renderer/Widgets/UIMenu.h"

FUIMenu::FUIMenu(FWindow* InOwnerWindow)
	: bIsInitializedUIMenu(false)
	, OwnerWindow(InOwnerWindow)
{
#if _DEBUG
	if (OwnerWindow == nullptr)
	{
		LOG_ERROR("OwnerWindow is nullptr!");
	}
#endif
}

FUIMenu::~FUIMenu()
{
	DeInitializePublic();
}

FWindow* FUIMenu::GetOwnerWindow() const
{
	return OwnerWindow;
}

void FUIMenu::InitializePublic()
{
	if (!bIsInitializedUIMenu)
	{
		bIsInitializedUIMenu = true;

		Initialize();
	}
}

void FUIMenu::DeInitializePublic()
{
	if (bIsInitializedUIMenu)
	{
		bIsInitializedUIMenu = false;

		DeInitialize();
	}
}

bool FUIMenu::IsInitialized() const
{
	return bIsInitializedUIMenu;
}

void FUIMenu::Initialize()
{
}

void FUIMenu::DeInitialize()
{
}
