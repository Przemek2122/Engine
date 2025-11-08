// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"

class FMutex;

/**
 * Will wait until mutex is locked and unlock when it disappears from scope
 */
class ENGINE_API FMutexScopeLock
{
public:
	FMutexScopeLock(FMutex& InMutex);
	~FMutexScopeLock();

protected:
	FMutex& Mutex;

};
