// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class FThreadData;
class FThreadWorkerData;
class FThread;
class FThreadsManager;

struct FThreadInputData
{
	friend FThread;
	friend FThreadData;
	friend FThreadWorkerData;
	friend FThreadsManager;

public:
	FThreadInputData(FThreadsManager* InThreadsManager, std::string InThreadName);

	FThread* GetThread() const { return Thread; }
	FThreadsManager* GetThreadsManager() const { return ThreadsManager; }

	const std::string& GetThreadName() const;

	bool IsThreadAlive() const;

private:
	FThread* Thread;
	FThreadsManager* ThreadsManager;

	/** Thread name visible in debugger etc... */
	std::string ThreadName;

	/** This setting is used to stop thread. */
	bool bThreadAlive;

};

/**
 * Note: this thread by defaut does not remove itself
 */
class FThread
{
	friend FThreadsManager;
	friend FThreadData;

protected:
	FThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData);
	virtual ~FThread();

	/** Called on main thread */
	virtual void StartThread();

	/** Called on main thread */
	virtual void StopThread();

	/** It will tick until FThreadInputData has bThreadAlive set to false */
	virtual void TickThread();

	virtual void ThreadManagerFunction();

	FThreadInputData* GetThreadInputData() const { return ThreadInputData; }
	FThreadData* GetThreadData() const { return ThreadData; }

private:
	/** Thread function used by SDL */
	static int ThreadFunction(void* InputData);

private:
	FThreadInputData* ThreadInputData;
	FThreadData* ThreadData;

	SDL_Thread* SDLThread;

};

/**
 * Thread class for ThreadManager
 * Takes and executes jobs from ThreadManager
 * This thread class removes itself on stop
 */
class FThreadWorker : public FThread
{
	friend FThreadData;
	friend FThreadWorkerData;

protected:
	FThreadWorker(FThreadInputData* InThreadInputData, FThreadData* InThreadData);
	~FThreadWorker() override;

	void TickThread() override;
	void ThreadManagerFunction() override;

};
