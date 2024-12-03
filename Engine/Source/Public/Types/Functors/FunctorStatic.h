// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

/**
 * Function storage class. Supports:
 * - Static function
 */
template<typename TReturnType, typename... TInParams>
class FFunctorStatic : public FFunctorBase<TReturnType, TInParams...>
{
public:	
	/** For static functions */
	FFunctorStatic(TReturnType (*InFunctionPointer)(TInParams...))
		: FunctionPointer(InFunctionPointer)
	{
	}
	virtual ~FFunctorStatic() override
	{
		//delete FunctionPointer;
	}

	/** Begin FFunctorBase interface */
	virtual TReturnType operator()(TInParams... Params) override
    {
		return (*FunctionPointer)(Params...); // Params ...
    }
	
	_NODISCARD virtual bool IsValid() const override
	{
		return FunctionPointer != nullptr;
	}

	_NODISCARD virtual EFunctorType GetFunctorType() const override
	{
		return EFunctorType::FT_STATIC;
	}
	/** End FFunctorBase interface */

	/** Assign other instance to this. */
	/*
	FFunctorStatic& operator=(FFunctorStatic const& InFunctor)
    {
		FunctionPointer = InFunctor.FunctionPointer;
		
		return this;
    }
	*/

protected:
	/** Stored function */
	TReturnType (*FunctionPointer)(TInParams ...);
};
