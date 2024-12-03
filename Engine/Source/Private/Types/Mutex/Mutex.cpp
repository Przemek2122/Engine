// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Mutex/Mutex.h"

FMutex::FMutex()
	: bIsLocked(false)
{
}

bool FMutex::IsLocked() const
{
	return bIsLocked;
}

void FMutex::Lock()
{
	bIsLocked = true;
}

void FMutex::Unlock()
{
	bIsLocked = false;
}

bool FMutex::TryLock()
{
	if (bIsLocked)
	{
		return false;
	}

	bIsLocked = true;

	return bIsLocked;
}
