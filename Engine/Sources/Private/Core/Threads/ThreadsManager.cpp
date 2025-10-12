#include "CoreEngine.h"
#include "Threads/ThreadsManager.h"

#include "Types/Mutex/MutexScopeLock.h"

FThreadsManager::FThreadsManager()
	: StartingNumberOfThreads(1)
	, DefaultThreadName("DefaultThread_")
{
}

FThreadsManager::~FThreadsManager()
{
}

void FThreadsManager::Initialize()
{
	// Calculate number of cores
	int32 NumberOfCores = SDL_GetNumLogicalCPUCores();

	// Ensure we always have at least one thread.
	if (NumberOfCores <= 0)
	{
		NumberOfCores = 1;
	}

	// Create available slots for threads
	for (int32 ThreadIndex = 0; ThreadIndex < NumberOfCores; ThreadIndex++)
	{
		AvailableThreadsNumbers.Push(ThreadIndex);
	}

	// Create default number of threads
	for (int32 i = 0; i < StartingNumberOfThreads; i++)
	{
		StartNewThread();
	}
}

void FThreadsManager::DeInitialize()
{
	// Stop created threads
	for (FThreadData* ThreadWorkerData : AllThreadsArray)
	{
		if (ThreadWorkerData->ThreadInputData->IsThreadAlive())
		{
			ThreadWorkerData->Thread->StopThread();
		}
	}

	LOG_INFO("Number of threads to stop: " << AllThreadsArray.Size());

	COUNTER_START(StopThreadsCoutnerStart);

	for (FThreadData* ThreadData : AllThreadsArray)
	{
		if (ThreadData != nullptr && ThreadData->ThreadInputData != nullptr)
		{
			LOG_WARN("Closing threads: " << ThreadData->ThreadInputData->GetThreadName());
		}
	}

	static constexpr int32 TimeToWaitEachLoopIterationInMS = 2;
	static constexpr int32 TimeToWaitBeforeWarn = 2000;

	int32 WaitTime = 0;

	// Wait for all threads to finish
	while (AllThreadsArray.Size())
	{
		THREAD_WAIT_MS(TimeToWaitEachLoopIterationInMS);

		WaitTime += TimeToWaitEachLoopIterationInMS;

		if (WaitTime > TimeToWaitBeforeWarn)
		{
			LOG_ERROR("Unable to stop all threads!");
		}
	}

	ENSURE_VALID(AllThreadsArray.Size() == 0);
	ENSURE_VALID(WorkerThreadsArray.Size() == 0);

	COUNTER_END(StopThreadsCoutnerStart, StopThreadsCoutnerEnd);

	std::cout << "Stopping threads took: " << COUNTER_GET_MS(StopThreadsCoutnerEnd) << " ms" << std::endl;
}

void FThreadsManager::TickThreadCallbacks()
{
	// Lock
	while (MainThreadCallbacksMutex.TryLock())
	{
		THREAD_WAIT_NS(100);
	}

	// Make copy
	MainThreadCallbacksCopy = MainThreadCallbacks;

	// Clear copied data
	MainThreadCallbacks.Clear();

	// Release mutex
	MainThreadCallbacksMutex.Unlock();

	for (FMainThreadCallbackStructure& ThreadCallback : MainThreadCallbacksCopy)
	{
		ThreadCallback.AsyncCallback->Execute();
	}

	MainThreadCallbacksCopy.Clear();
}

void FThreadsManager::AddAsyncDelegate(FDelegateSafe<void>& DelegateToRunAsync)
{
	FAsyncWorkStructure AsyncWorkStructure;
	AsyncWorkStructure.DelegateToRunAsync = std::make_shared<FDelegateSafe<void>>(std::move(DelegateToRunAsync));

	AddAsyncWork(AsyncWorkStructure);
}

void FThreadsManager::AddAsyncDelegate(FDelegateSafe<void>& DelegateToRunAsync, FDelegateSafe<void>& AsyncCallback)
{
	FAsyncWorkStructure AsyncWorkStructure;
	AsyncWorkStructure.DelegateToRunAsync = std::make_shared<FDelegateSafe<void>>(std::move(DelegateToRunAsync));
	AsyncWorkStructure.AsyncCallback = std::make_shared<FDelegateSafe<void>>(std::move(AsyncCallback));

	AddAsyncWork(AsyncWorkStructure);
}

void FThreadsManager::AddAsyncWork(const FAsyncWorkStructure& AsyncRunWithCallback)
{
	while (AsyncJobQueueMutex.IsLocked())
	{
		THREAD_WAIT_SHORT_TIME;
	}

	FMutexScopeLock MutexScopeLock(AsyncJobQueueMutex);

	AsyncJobQueue.PushBack(AsyncRunWithCallback);
}

void FThreadsManager::TryStopThread(FThreadData* ThreadData)
{
	if (ThreadData != nullptr && ThreadData->ThreadInputData->IsThreadAlive())
	{
		ThreadData->GetThread()->StopThread();
	}
}

void FThreadsManager::ResetAllJobs()
{
	while (AsyncJobQueueMutex.IsLocked())
	{
		THREAD_WAIT_SHORT_TIME;
	}

	FMutexScopeLock MutexScopeLock(AsyncJobQueueMutex);

	AsyncJobQueue.Clear();
}

FThreadWorkerData* FThreadsManager::CreateThreadWorker(const std::string& NewThreadName)
{
	return CreateThread<FThreadWorker, FThreadWorkerData>(NewThreadName);
}

void FThreadsManager::StartNewThread()
{
	if (!AvailableThreadsNumbers.IsEmpty())
	{
		FMutexScopeLock MutexScopeLock(WorkerThreadsArrayMutex);

		const int32 NewThreadIndex = AvailableThreadsNumbers[0];
		AvailableThreadsNumbers.RemoveAt(0);

		const std::string NewThreadName = DefaultThreadName + std::to_string(NewThreadIndex + 1);

		FThreadWorkerData* ThreadData = CreateThreadWorker(NewThreadName);
		ThreadData->ThreadNumber = NewThreadIndex;

		WorkerThreadsArray.Push(ThreadData);
	}
}

void FThreadsManager::StopThread()
{
	int32 ThreadIndex = WorkerThreadsArray.Size() - 1;

	FThreadWorkerData* LastAliveThread = WorkerThreadsArray[ThreadIndex];

	// Find last still alive thread
	while (ThreadIndex >= 0 && !LastAliveThread->ThreadInputData->IsThreadAlive())
	{
		ThreadIndex--;

		LastAliveThread = WorkerThreadsArray[ThreadIndex];
	}

	AvailableThreadsNumbers.Push(LastAliveThread->ThreadNumber);

	// Stop thread
	if (LastAliveThread->ThreadInputData->IsThreadAlive())
	{
		LastAliveThread->Thread->StopThread();
	}
	else
	{
		LOG_WARN("Stopping thread failed. Already stopped.");
	}
}

int32 FThreadsManager::GetNumberOfWorkerThreads() const
{
	return WorkerThreadsArray.Size();
}

int32 FThreadsManager::GetNumberOfCores()
{
	return SDL_GetNumLogicalCPUCores();
}

FAsyncWorkStructure FThreadsManager::GetFirstAvailableJob()
{
	// @TODO Remove mutex and find and smart way of distributing tasks

	THREAD_WAIT_FOR_MUTEX_LOCK(AsyncJobQueueMutex);

	if (AsyncJobQueue.Size() == 0)
	{
		AsyncJobQueueMutex.Unlock();

		FAsyncWorkStructure AsyncWorkStructure;
		AsyncWorkStructure.DelegateToRunAsync = std::make_shared<FDelegateSafe<>>();
		AsyncWorkStructure.AsyncCallback = std::make_shared<FDelegateSafe<>>();

		return AsyncWorkStructure;
	}
	else
	{
		ENSURE_VALID(AsyncJobQueueMutex.IsLocked());

		// Get first element
		FAsyncWorkStructure AsyncWorkStructure = AsyncJobQueue.PeekFirst();

		// Remove first element from list
		AsyncJobQueue.DequeFront();

		AsyncJobQueueMutex.Unlock();

		return AsyncWorkStructure;
	}
}

bool FThreadsManager::HasAnyJobLeft() const
{
	return (AsyncJobQueue.Size() > 0);
}

bool FThreadsManager::InternalRemoveWorkerThread(const FThread* InThread)
{
	bool bWasRemoved;

	if (!WorkerThreadsArrayMutex.TryLock())
	{
		bWasRemoved = false;
	}
	else
	{
		// Remove thread data from array
		WorkerThreadsArray.Remove(InThread->GetThreadData());

		WorkerThreadsArrayMutex.Unlock();

		bWasRemoved = true;
	}

	return bWasRemoved;
}

bool FThreadsManager::InternalRemoveThread(const FThread* InThread)
{
	bool bWasRemoved;

	if (!WorkerThreadsArrayMutex.TryLock())
	{
		bWasRemoved = false;
	}
	else
	{
		// Remove thread data from array
		AllThreadsArray.Remove(InThread->GetThreadData());

		WorkerThreadsArrayMutex.Unlock();

		bWasRemoved = true;
	}

	return bWasRemoved;
}
