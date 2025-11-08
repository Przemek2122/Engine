// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"

class ENGINE_API FMutex
{
public:
	FMutex();

	bool IsLocked() const;

	void Lock();
	void Unlock();

	/** Will try to lock mutex, will return false if locked already, true if succesfuly locked */
	bool TryLock();

protected:
	bool bIsLocked;

};
