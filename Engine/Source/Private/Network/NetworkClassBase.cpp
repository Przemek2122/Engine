// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkClassBase.h"
#include "Threads/ThreadsManager.h"
#include "Network/NetworkThread.h"

static int64 ThreadNumber = 0;
static const std::string NETWORK_THREAD_NAME = "NetworkThread";

FNetworkClassBase::FNetworkClassBase(FNetworkManager* InNetworkManager)
	: NetworkManager(InNetworkManager)
	, NetworkThreadData(nullptr)
	, NetworkThread(nullptr)
{
}

FNetworkClassBase::~FNetworkClassBase()
{
}

void FNetworkClassBase::Initialize()
{
	FThreadsManager* ThreadManager = GEngine->GetThreadsManager();
	if (ThreadManager != nullptr)
	{
		ThreadName = std::string(NETWORK_THREAD_NAME) + std::to_string(ThreadNumber);
		ThreadNumber++;

		NetworkThreadData = ThreadManager->CreateThread<FNetworkThread, FThreadData>(ThreadName);
		NetworkThread = dynamic_cast<FNetworkThread*>(NetworkThreadData->GetThread());
		if (NetworkThread != nullptr)
		{
			NetworkThread->SetNetworkManager(this);
			NetworkThread->SetIsNetworkReady(true);
		}
		else
		{
			LOG_WARN("Network thread is missing");
		}
	}
}

void FNetworkClassBase::DeInitialize()
{
	FThreadsManager* ThreadManager = GEngine->GetThreadsManager();
	if (ThreadManager != nullptr && NetworkThreadData != nullptr)
	{
		ThreadManager->TryStopThread(NetworkThreadData);
	}
}

#endif