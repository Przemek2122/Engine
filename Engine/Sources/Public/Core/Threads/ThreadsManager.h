// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"
#include "Thread.h"
#include "ThreadStructure.h"
#include "ThreadData.h"

/**
 * Class for managing threads using SDL2.
 */
class ENGINE_API FThreadsManager
{
	friend FThread;
	friend FThreadWorker;

public:
	FThreadsManager();
	virtual ~FThreadsManager();

	/** Should be called when class is set up to setup available threads */
	virtual void Initialize();

	virtual void DeInitialize();

	/** Runs callbacks on main thread */
	void TickThreadCallbacks();

	/** Add delegate which will be run on first available async thread */
	void AddAsyncDelegate(FDelegateSafe<>& DelegateToRunAsync);

	/** Add delegate which will be run on first available async thread with main thread AsyncCallback */
	void AddAsyncDelegate(FDelegateSafe<>& DelegateToRunAsync, FDelegateSafe<>& AsyncCallback);

	/** Add delegate which will be run on first available async thread by passing structure */
	void AddAsyncWork(const FAsyncWorkStructure& AsyncRunWithCallback);

	/** Creates thread for use, use StopThread to disable and automatically delete thread */
	template<typename TThreadClass, typename TThreadDataClass>
	TThreadDataClass* CreateThread(const std::string& NewThreadName)
	{
		// Create thread
		TThreadDataClass* ThreadData = new TThreadDataClass(this, NewThreadName);

		ThreadData->template Create<TThreadClass>();

		AllThreadsArray.Push(dynamic_cast<FThreadData*>(ThreadData));

		// Start thread
		ThreadData->Thread->StartThread();

		return ThreadData;
	}

	/** Sets flag to stop a thread, will not be immediate stop. */
	void TryStopThread(FThreadData* ThreadData);

	/** Kill all pending tasks, will be called when gamemode changes for example */
	virtual void ResetAllJobs();

	static int32 GetNumberOfLogicalCPU();

protected:
	/** Creates thread for use, use StopThread to disable and automatically delete thread */
	FThreadWorkerData* CreateThreadWorker(const std::string& NewThreadName);

	/** Starts a new thread if there is free number in pool available, see param AvailableThreadsNumbers */
	void StartNewThread();

	/** Stops one thread */
	void StopThread();

	/** Returns number of threads managed by this class */
	int32 GetNumberOfWorkerThreads() const;

	/** @returuns number of system available cores */
	static int32 GetNumberOfCores();

	/** Used by threads to actually do the job */
	FAsyncWorkStructure GetFirstAvailableJob();

	bool HasAnyJobLeft() const;

	/** Queue of async jobs to be done */
	CDeque<FAsyncWorkStructure> AsyncJobQueue;

	/** Mutex for param AsyncJobQueue */
	std::mutex AsyncJobQueueMutex;

	/** Callbacks from finishes async jobs */
	CArray<FMainThreadCallbackStructure> MainThreadCallbacks;

	/** Mutex for param MainThreadCallbacks */
	std::mutex MainThreadCallbacksMutex;

	CArray<FMainThreadCallbackStructure> MainThreadCallbacksCopy;

	int StartingNumberOfThreads;

private:
	bool InternalRemoveWorkerThread(const FThread* InThread);
	bool InternalRemoveThread(const FThread* InThread);

	/** Array with worker only threads */
	CArray<FThreadWorkerData*> WorkerThreadsArray;

	/** Array with ALL threads created */
	CArray<FThreadData*> AllThreadsArray;

	/** Mutex for param WorkerThreadsArray */
	std::mutex WorkerThreadsArrayMutex;

	/** Number with thread Id for naming */
	CArray<int> AvailableThreadsNumbers;

	/** Default name of thread, at end number will be added */
	std::string DefaultThreadName;

};
