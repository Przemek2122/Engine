// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "Thread.h"

class FThread;
struct FThreadInputData;

/**
 * Basic thread data
 */
class FThreadData
{
	friend FThreadsManager;

public:
	FThreadData(FThreadsManager* InThreadsManager, const std::string& InNewThreadName);
	virtual ~FThreadData() = default;

	FThread* GetThread() const { return Thread; };

protected:
	template<typename TThreadClass>
	TThreadClass* Create()
	{
		TThreadClass* NewThread = new TThreadClass(ThreadInputData, this);

		// Create thread
		Thread = NewThread;

		// Set thread references
		ThreadInputData->Thread = Thread;

		return NewThread;
	}

protected:
	FThreadInputData* ThreadInputData;
	FThread* Thread;

};

class FThreadWorkerData : public FThreadData
{
	friend FThreadsManager;

public:
	FThreadWorkerData(FThreadsManager* InThreadsManager, const std::string& InNewThreadName);

protected:
	int ThreadNumber;
};
