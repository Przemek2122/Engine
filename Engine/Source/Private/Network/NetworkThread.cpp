// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkThread.h"

#include "Network/NetworkClassBase.h"

FNetworkThread::FNetworkThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData)
	: FThread(InThreadInputData, InThreadData)
	, NetworkObject(nullptr)
	, bIsNetworkReady(false)
{
}

FNetworkThread::~FNetworkThread()
{
}

void FNetworkThread::SetNetworkManager(FNetworkClassBase* InNetworkObject)
{
	NetworkObject = InNetworkObject;
}

void FNetworkThread::SetIsNetworkReady(const bool bInReady)
{
	bIsNetworkReady = bInReady;
}

void FNetworkThread::TickThread()
{
	if (bIsNetworkReady)
	{
		if (NetworkObject != nullptr)
		{
			NetworkObject->AsyncWork();
		}
		else
		{
			LOG_WARN("NetworkManager is missing.");
		}
	}

	// Wait as data is not ready
	SDL_Delay(1);
}

#endif