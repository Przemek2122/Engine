// Created by https://www.linkedin.com/in/przemek2122/ 2024

#include "CoreEngine.h"
#include "Types/Mutex/Mutex.h"

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
