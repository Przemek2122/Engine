// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"

/**
 * Base Delegate storing functors.
 * Call ExecuteByLambda to execute all bound functions.
 */
template<typename TReturnType, typename TFunctorType, typename... TInParams>
class FDelegateBase
{
public:
	typedef FFunctorBase<TReturnType, TInParams ...> Functor;

	FDelegateBase() = default;
	virtual ~FDelegateBase()
	{
		Functors.Clear();
	}

	/** @returns true if there is at least one function bound. */
	_NODISCARD virtual bool IsBound() const
	{
		return (Functors.Size() > 0);
	}
	
	/** Executes all bound functions. */
	virtual void Execute(TInParams... InParams) = 0;

protected:
	CArray<TFunctorType> Functors;
	
};
