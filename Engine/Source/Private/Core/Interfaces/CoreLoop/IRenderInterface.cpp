#include "CoreEngine.h"
#include "Core/Interfaces/CoreLoop/IRenderInterface.h"

#include "Engine/EngineRenderingManager.h"

IRenderInterface::IRenderInterface()
	: bIsRegistered(false)
	, RegisteredPhase(ERenderPhase::None)
	, RenderFunctor(FFunctorObject(this, &IRenderInterface::Render))
{
	Register();
}

IRenderInterface::~IRenderInterface()
{
	if (bIsRegistered)
	{
		GEngine->GetEngineRenderingManager()->UnRegisterInterface(RenderFunctor, RegisteredPhase);
	}
}

void IRenderInterface::Register()
{
	if (!bIsRegistered)
	{
		bIsRegistered = true;

		RegisteredPhase = GetRenderPhase();

		GEngine->GetEngineRenderingManager()->RegisterInterface(RenderFunctor, RegisteredPhase);
	}
}
