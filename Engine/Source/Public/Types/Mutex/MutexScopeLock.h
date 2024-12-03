// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class FMutex;

class FMutexScopeLock
{
public:
	FMutexScopeLock(FMutex& InMutex);
	~FMutexScopeLock();

protected:
	FMutex& Mutex;

};
