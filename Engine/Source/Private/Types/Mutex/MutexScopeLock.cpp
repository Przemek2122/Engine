// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Mutex/MutexScopeLock.h"

#include "Mutex/Mutex.h"

FMutexScopeLock::FMutexScopeLock(FMutex& InMutex)
	: Mutex(InMutex)
{
	if (Mutex.IsLocked())
	{
		LOG_ERROR("Mutex already locked, this should never happen");
	}
	else
	{
		Mutex.Lock();
	}
}

FMutexScopeLock::~FMutexScopeLock()
{
	Mutex.Unlock();
}
