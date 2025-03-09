// Created by Przemys³aw Wiewióra 2020 

#pragma once
#include "Includes/EngineMacros.h"

/** 32-bit integer - Default integer type for containers. */
typedef int32 ContainerInt;
/** 64-bit integer type for containers. */
typedef int64 ContainerBigInt;

/**
 * Base class for containers.
 */
template<typename TType, typename TSizeType = ContainerInt>
class CContainerBase
{
public:
	virtual ~CContainerBase() = default;

	virtual TSizeType Size() const = 0;
	virtual bool IsEmpty() const = 0;

};
