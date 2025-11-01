// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Types/Mutex/MutexScopeLock.h"

#include "Types/Mutex/Mutex.h"

FMutexScopeLock::FMutexScopeLock(FMutex& InMutex)
	: Mutex(InMutex)
{
	THREAD_WAIT_FOR_MUTEX_LOCK(Mutex);
}

FMutexScopeLock::~FMutexScopeLock()
{
	Mutex.Unlock();
}
