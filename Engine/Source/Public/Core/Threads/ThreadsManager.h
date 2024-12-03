// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "Thread.h"
#include "ThreadStructure.h"
#include "ThreadData.h"
#include "Types/Mutex/Mutex.h"

/**
 * Class for managing threads using SDL2.
 */
class FThreadsManager
{
	friend FThread;
	friend FThreadWorker;

public:
	FThreadsManager();
	virtual ~FThreadsManager();

	/** Should be called when class is set up to setup available threads */
	virtual void Initialize();

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

		// Start thread
		ThreadData->Thread->StartThread();

		return ThreadData;
	}

	/** Sets flag to stop a thread, will not be immediate stop. */
	void TryStopThread(FThreadData* ThreadData);

	/** Kill all pending tasks, will be called when gamemode changes for example */
	virtual void ResetAllJobs();

protected:
	/** Creates thread for use, use StopThread to disable and automatically delete thread */
	FThreadWorkerData* CreateThreadWorker(const std::string& NewThreadName);

	/** Starts a new thread if there is free number in pool available, see param AvailableThreadsNumbers */
	void StartNewThread();

	/** Stops one thread */
	void StopThread();

	/** Returns number of threads managed by this class */
	int GetNumberOfWorkerThreads() const;

	/** @returuns number of system available cores */
	static int GetNumberOfCores();

	/** Used by threads to actually do the job */
	FAsyncWorkStructure GetFirstAvailableJob();

	bool HasAnyJobLeft() const;

	/** Queue of async jobs to be done */
	CDeque<FAsyncWorkStructure> AsyncJobQueue;

	/** Mutex for param AsyncJobQueue */
	FMutex AsyncJobQueueMutex;

	/** Callbacks from finishes async jobs */
	CArray<FMainThreadCallbackStructure> MainThreadCallbacks;

	/** Mutex for param MainThreadCallbacks */
	FMutex MainThreadCallbacksMutex;

	CArray<FMainThreadCallbackStructure> MainThreadCallbacksCopy;

	int StartingNumberOfThreads;

private:
	bool InternalRemoveWorkerThread(const FThread* InThread);

	/** Array with created threads */
	CArray<FThreadWorkerData*> WorkerThreadsArray;

	/** Mutex for param WorkerThreadsArray */
	FMutex WorkerThreadsArrayMutex;

	/** Number with thread Id for naming */
	CArray<int> AvailableThreadsNumbers;

	/** Default name of thread, at end number will be added */
	std::string DefaultThreadName;

};
