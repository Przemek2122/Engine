// Created by Przemys³aw Wiewióra 2020

#pragma once

#include <functional>
#include "Types/Functors/FunctorBase.h"

/**
 * Function storage class. Supports:
 * Lambda
 */
template<typename TReturnType, typename... TInParams> 
class FFunctorLambda : public FFunctorBase<TReturnType, TInParams...>
{
public:
	typedef FFunctorLambda<TReturnType, TInParams...> TSelfType;

	/** Constructor for lambdas */
	template<typename TTypeAuto>
	FFunctorLambda(TTypeAuto InFunction)
		: Function(std::move(InFunction))
	{
	}

	/** Copy function constructor */
	FFunctorLambda(std::function<TReturnType(TInParams ...)>& InFunction)
		: Function(std::move(InFunction))
	{
	}

	/** Move function constructor */
	FFunctorLambda(std::function<TReturnType(TInParams ...)>&& InFunction)
		: Function(std::move(InFunction))
	{
	}

	bool IsEqual(TSelfType& Other)
	{
		return (Function.target_type() == Other.Function.target_type() && GetFunctionAddress() == Other.GetFunctionAddress());
	}

	bool IsEqual(TSelfType* Other)
	{
		return (Function.target_type() == Other.Function.target_type() && GetFunctionAddress() == Other.GetFunctionAddress());
	}

	/** Begin FFunctorBase interface */
	virtual TReturnType operator()(TInParams... Params) override
	{
		return Function(Params ...);
	}

	_NODISCARD virtual bool IsValid() const override
	{
		return false;
	}

	_NODISCARD EFunctorType GetFunctorType() const override
	{
		return EFunctorType::FT_LAMBDA;
	}
	/** End FFunctorBase interface */

	/** Get low level function address - If you would like to compare, use IsEqual instead */
	long GetFunctionAddress()
	{
		return (*(long*)(char*)(&Function));
	}

protected:
	/** Stored lambda */
	std::function<TReturnType(TInParams ...)> Function;
};
