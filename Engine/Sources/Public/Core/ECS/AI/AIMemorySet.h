// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"

/**
 * AI Memory set is used to store memory for whole AI.
 * It can be shared between AI Trees
 */
class ENGINE_API FAIMemorySet
{
protected:
	/** Virtual method for casting */
	virtual void InternalVirtualMethod() { }

public:
	FAIMemorySet() = default;
	virtual ~FAIMemorySet() = default;

};
