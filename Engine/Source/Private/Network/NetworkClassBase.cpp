// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkClassBase.h"
#include "Threads/ThreadsManager.h"
#include "Network/NetworkThread.h"

static int64 ThreadNumber = 0;
static const std::string NETWORK_THREAD_NAME = "NetworkThread";

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	buf->base = static_cast<char*>(malloc(suggested_size)); // Allocate memory for incoming data
	buf->len = suggested_size;
}

FNetworkClassBase::FNetworkClassBase(FNetworkManager* InNetworkManager)
	: NetworkManager(InNetworkManager)
	, NetworkThreadData(nullptr)
	, NetworkThread(nullptr)
{
	LoopForUV = new uv_loop_t();

	uv_loop_init(LoopForUV);
}

FNetworkClassBase::~FNetworkClassBase()
{
	uv_loop_close(LoopForUV);

	delete LoopForUV;
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