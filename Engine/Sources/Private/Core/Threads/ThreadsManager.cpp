#include "CoreEngine.h"
#include "Threads/ThreadsManager.h"

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
	MainThreadCallbacksMutex.lock();

	// Make copy
	MainThreadCallbacksCopy = MainThreadCallbacks;

	// Clear copied data
	MainThreadCallbacks.Clear();

	// Release mutex
	MainThreadCallbacksMutex.unlock();

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
	std::lock_guard<std::mutex> MutexScopeLock(AsyncJobQueueMutex);

	AsyncJobQueue.PushBack(AsyncRunWithCallback);
}

void FThreadsManager::TryStopThread(FThreadData* ThreadData)
{
	// Check if engine is closing or just session is being closed
	// If engine is closing thread will be closed by threads manager
	if (FGlobalDefines::GEngine->CanContinueMainLoop() && ThreadData != nullptr && ThreadData->ThreadInputData->IsThreadAlive())
	{
		ThreadData->GetThread()->StopThread();
	}
}

void FThreadsManager::ResetAllJobs()
{
	std::lock_guard<std::mutex> MutexScopeLock(AsyncJobQueueMutex);

	AsyncJobQueue.Clear();
}

int32 FThreadsManager::GetNumberOfLogicalCPU()
{
	return SDL_GetNumLogicalCPUCores();
}

FThreadWorkerData* FThreadsManager::CreateThreadWorker(const std::string& NewThreadName)
{
	return CreateThread<FThreadWorker, FThreadWorkerData>(NewThreadName);
}

void FThreadsManager::StartNewThread()
{
	if (!AvailableThreadsNumbers.IsEmpty())
	{
		std::lock_guard<std::mutex> MutexScopeLock(WorkerThreadsArrayMutex);

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

	std::lock_guard<std::mutex> Lock(AsyncJobQueueMutex);

	if (AsyncJobQueue.Size() == 0)
	{
		FAsyncWorkStructure AsyncWorkStructure;
		AsyncWorkStructure.DelegateToRunAsync = std::make_shared<FDelegateSafe<>>();
		AsyncWorkStructure.AsyncCallback = std::make_shared<FDelegateSafe<>>();

		return AsyncWorkStructure;
	}
	else
	{
		// Get first element
		FAsyncWorkStructure AsyncWorkStructure = AsyncJobQueue.PeekFirst();

		// Remove first element from list
		AsyncJobQueue.DequeFront();

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

	std::lock_guard<std::mutex> Lock(WorkerThreadsArrayMutex);

	// Remove thread data from array
	WorkerThreadsArray.Remove(InThread->GetThreadData());

	bWasRemoved = true;

	return bWasRemoved;
}

bool FThreadsManager::InternalRemoveThread(const FThread* InThread)
{
	bool bWasRemoved;

	std::lock_guard<std::mutex> Lock(WorkerThreadsArrayMutex);

	// Remove thread data from array
	AllThreadsArray.Remove(InThread->GetThreadData());

	bWasRemoved = true;

	return bWasRemoved;
}
