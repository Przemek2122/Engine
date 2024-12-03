#include "CoreEngine.h"
#include "Threads/Thread.h"

#include "Threads/ThreadData.h"
#include "Threads/ThreadsManager.h"

FThreadInputData::FThreadInputData(FThreadsManager* InThreadsManager, std::string InThreadName)
	: Thread(nullptr)
	, ThreadsManager(InThreadsManager)
	, ThreadName(std::move(InThreadName))
	, bThreadAlive(true)
{
}

const std::string& FThreadInputData::GetThreadName() const
{
	return ThreadName;
}

bool FThreadInputData::IsThreadAlive() const
{
	return bThreadAlive;
}

FThread::FThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData)
	: ThreadInputData(InThreadInputData)
	, ThreadData(InThreadData)
	, SDLThread(nullptr)
{
}

FThread::~FThread()
{
	if (SDLThread != nullptr)
	{
		SDL_DetachThread(SDLThread);
	}

	delete ThreadInputData;
	delete ThreadData;
}

void FThread::StartThread()
{
	SDL_CreateThread(FThread::ThreadFunction, ThreadInputData->GetThreadName().c_str(), ThreadInputData);
}

void FThread::StopThread()
{
	ThreadInputData->bThreadAlive = false;
}

void FThread::TickThread()
{
	SDL_Delay(1);
}

void FThread::ThreadManagerFunction()
{
	while (ThreadInputData->IsThreadAlive())
	{
		TickThread();
	}
}

int FThread::ThreadFunction(void* InputData)
{
	FThreadInputData* ThreadData = static_cast<FThreadInputData*>(InputData);
	FThread* Thread = ThreadData->GetThread();

	Thread->ThreadManagerFunction();

	return 0;
}

FThreadWorker::FThreadWorker(FThreadInputData* InThreadInputData, FThreadData* InThreadData)
	: FThread(InThreadInputData, InThreadData)
{
}

FThreadWorker::~FThreadWorker()
{
}

void FThreadWorker::TickThread()
{
	FThreadsManager* ThreadsManager = GetThreadInputData()->GetThreadsManager();
	while (GetThreadInputData()->IsThreadAlive() && ThreadsManager->HasAnyJobLeft())
	{
		FAsyncWorkStructure AsyncWorkStructure = ThreadsManager->GetFirstAvailableJob();

		// Run job to be done async
		AsyncWorkStructure.DelegateToRunAsync->Execute();

		if (AsyncWorkStructure.AsyncCallback)
		{
			THREAD_WAIT_FOR_MUTEX_LOCK(ThreadsManager->MainThreadCallbacksMutex);

			ENSURE_VALID(ThreadsManager->MainThreadCallbacksMutex.IsLocked());

			// Enqueue sync callback on main thread
			ThreadsManager->MainThreadCallbacks.Push(FMainThreadCallbackStructure(AsyncWorkStructure.AsyncCallback));

			ThreadsManager->MainThreadCallbacksMutex.Unlock();
		}
	}

	FThread::TickThread();
}

void FThreadWorker::ThreadManagerFunction()
{
	FThread::ThreadManagerFunction();

	// Wait for deletion
	while (!GetThreadInputData()->GetThreadsManager()->InternalRemoveWorkerThread(this))
	{
		THREAD_WAIT_SHORT_TIME;
	}

	delete this;
}
