// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

enum class EFunctorType
{
	FT_NONE,		// Default value
	FT_LAMBDA,		// Lambda function
	FT_OBJECT,		// Member function
	FT_STATIC,		// Static function
};

/**
 * A functor (or function object) is a C++ class that acts like a function.
 * Supports:
 * NOTHING! It's abstract class used as base.
 *
 * IsValid() - Use to check if function is bound.
 * operator() - Use to call stored function.
 */
template<typename TReturnType, typename... TInParams>
class FFunctorBase
{
public:
	FFunctorBase() = default;
	virtual ~FFunctorBase() = default;
	
	/** This function calls stored function */
	virtual TReturnType operator()(TInParams... Params) = 0;

	/** True if any function was bound. */                                                                                                                                                
	_NODISCARD virtual bool IsValid() const = 0;

	/** Returns type of functor defined in EFunctorType */
	_NODISCARD virtual EFunctorType GetFunctorType() const = 0;

};
