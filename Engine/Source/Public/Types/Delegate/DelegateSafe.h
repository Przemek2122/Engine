// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "DelegateBase.h"
#include "Types/Functors/FunctorLambda.h"
#include "Types/Functors/FunctorObject.h"
#include "Core/Includes/GlobalDefines.h"

/**
 * Delegate extending base with bind and unbind and UnBindAll.
 * Same as FDelegate but with safer memory management.
 *
 * @Note: When passing by parameter use reference or pointer, otherwise it will not compile.
 * @Note: All functions passed in are deleted at end of this object life.
 */
template<typename TReturnType = void, typename... TInParams>
class FDelegateSafe : public FDelegateBase<TReturnType, std::shared_ptr<FFunctorBase<TReturnType, TInParams ...>>, TInParams...>
{
public:
	using FunctorType = FFunctorBase<TReturnType, TInParams ...>;
	using DelegateBase = FDelegateBase<TReturnType, std::shared_ptr<FunctorType>, TInParams...>;

	using FunctorLambdaType = FFunctorLambda<TReturnType, TInParams...>;

	// Default constructor
	FDelegateSafe() = default;

	// Copy constructor - Forbidden, not efficient + causes double deletion of Functors
	FDelegateSafe(const FDelegateSafe& InDelegate) = delete;

	// Move constructor
	FDelegateSafe(FDelegateSafe&& InDelegate) noexcept
	{
		DelegateBase::Functors = std::move(InDelegate.DelegateBase::Functors);
	}

	// Copy operator - Forbidden, not efficient + causes double deletion of Functors
	FDelegateSafe& operator=(const FDelegateSafe& InDelegate) = delete;

	// Copy assignment operator
	FDelegateSafe& operator=(FDelegateSafe&& InDelegate) noexcept
	{
		DelegateBase::Functors = std::move(InDelegate.DelegateBase::Functors);

		return *this;
	}

	/** Begin FDelegateBase interface */
	/** Executes all bound functions. */
	virtual void Execute(TInParams... InParams) override
	{
		const auto FunctorsNum = DelegateBase::Functors.Size();
	
		for (ContainerInt i = 0; i < FunctorsNum; i++)
		{
			DelegateBase::Functors[i]->operator()(InParams ...);
		}
	}

	/** Executes all bound functions using Lambda to define how it executes. */
	using ExecuteByLambdaDefinitionFunctor = FFunctorBase<TReturnType, TInParams...>;
	using ExecuteByLambdaDefinition = FFunctorLambda<void, ExecuteByLambdaDefinitionFunctor*, TInParams...>;
	virtual void ExecuteByLambda(ExecuteByLambdaDefinition Lambda, TInParams... InParams)
	{
		for (ContainerInt i = 0; i < DelegateBase::Functors.Size(); i++)
		{
			Lambda(DelegateBase::Functors[i].get(), InParams ...);
		}
	}
	/** End FDelegateBase interface */

	/** Bind C++ raw Lambda - Fast but not recommended */
	template <typename TTypeAuto>
	void BindLambda(TTypeAuto Lambda)
	{
		DelegateBase::Functors.Push(std::make_shared<FunctorLambdaType>(Lambda));
	}

	/** You can make new FFunctorLambda<TReturnType, TInParams...> */
	void BindLambda(FunctorLambdaType& Lambda)
	{
		DelegateBase::Functors.Push(std::make_shared<FunctorLambdaType>(Lambda));
	}

	/** Remove lambda, only by previously added type (function pointer must be equal) */
	void UnBindLambda(FunctorLambdaType& Lambda)
	{
		int Index = FindIndexOfLambdaFunctor(Lambda);

		if (Index != INDEX_NONE)
		{
			DelegateBase::Functors.RemoveAt(Index);
		}
	}

	/** Remove C++ raw Lambda - Fast but not recommended */
	template <typename TTypeAuto>
	void UnBindLambda(TTypeAuto& Lambda)
	{
		std::shared_ptr<FunctorLambdaType> TemporarySharedLambdaPointer = std::make_shared<FunctorLambdaType>(Lambda);

		int Index = FindIndexOfLambdaFunctor(*TemporarySharedLambdaPointer);

		if (Index != INDEX_NONE)
		{
			DelegateBase::Functors.RemoveAt(Index);
		}
	}

	/*
	 * Example on how to bind your object
	 * OnYourDelegateChanged.BindObject(this, &FYourClass::YourFunctionName);
	 */
	template<class TClass>
	void BindObject(TClass* InClassObject, TReturnType(TClass::* InFunctionPointer)(TInParams...))
	{
		using FunctorObjectType = FFunctorObject<TClass, TReturnType, TInParams...>;

		DelegateBase::Functors.Push(std::make_shared<FunctorObjectType>(InClassObject, InFunctionPointer));
	}

	template<class TClass>
	void BindObject(FFunctorObject<TClass, TReturnType, TInParams...>& Functor)
	{
		using FunctorObjectType = FFunctorObject<TClass, TReturnType, TInParams...>;

		DelegateBase::Functors.Push(std::make_shared<FunctorObjectType>(Functor));
	}

	template<class TClass>
	void UnBindObject(TClass* InClassObject, TReturnType(TClass::* InFunctionPointer)(TInParams...))
	{
		using FunctorObjectType = FFunctorObject<TClass, TReturnType, TInParams...>;

		std::shared_ptr<FunctorObjectType> TemporaryFunctor = std::make_shared<FunctorObjectType>(InClassObject, InFunctionPointer);

		int Index = FindIndexOfObjectFunctor(*TemporaryFunctor);

		if (Index != INDEX_NONE)
		{
			DelegateBase::Functors.RemoveAt(Index);
		}
	}

	template<class TClass>
	void UnBindObject(FFunctorObject<TClass, TReturnType, TInParams...>& Functor)
	{
		using FunctorObjectType = FFunctorObject<TClass, TReturnType, TInParams...>;

		//std::shared_ptr<FunctorObjectType> TemporaryFunctor = std::make_shared<FunctorObjectType>(Functor);

		int Index = FindIndexOfObjectFunctor(Functor);

		if (Index != INDEX_NONE)
		{
			DelegateBase::Functors.RemoveAt(Index);
		}
	}

	/** Remove all bound functions. */
	void UnBindAll()
	{
		DelegateBase::Functors.Clear();
	}

protected:
	ContainerInt FindIndexOfLambdaFunctor(FunctorLambdaType& LambdaFunctor)
	{
		ContainerInt Index = INDEX_NONE;

		for (ContainerInt i = 0; i < DelegateBase::Functors.Size(); i++)
		{
			std::shared_ptr<FunctorType>& Functor = DelegateBase::Functors[i];

			// Quick check if it is lambda - Fast
			if (Functor->GetFunctorType() == EFunctorType::FT_LAMBDA)
			{
				FunctorLambdaType* FunctorLambda = dynamic_cast<FunctorLambdaType*>(Functor.get());

				if (FunctorLambda->IsEqual(LambdaFunctor))
				{
					Index = i;

					break;
				}
			}
		}

		return Index;
	}

	template<class TClass>
	ContainerInt FindIndexOfObjectFunctor(FFunctorObject<TClass, TReturnType, TInParams...>& ObjectFunctor)
	{
		using FunctorObjectType = FFunctorObject<TClass, TReturnType, TInParams...>;

		ContainerInt Index = INDEX_NONE;

		for (ContainerInt i = 0; i < DelegateBase::Functors.Size(); i++)
		{
			std::shared_ptr<FunctorType>& Functor = DelegateBase::Functors[i];

			// Quick check if it is lambda - Fast
			if (Functor->GetFunctorType() == EFunctorType::FT_OBJECT)
			{
				FunctorObjectType* FunctorObject = dynamic_cast<FunctorObjectType*>(Functor.get());

				if (FunctorObject->IsEqual(ObjectFunctor))
				{
					Index = i;

					break;
				}
			}
		}

		return Index;
	}
	
};
