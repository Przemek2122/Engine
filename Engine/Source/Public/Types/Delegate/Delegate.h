// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "DelegateBase.h"
#include "Types/Functors/FunctorLambda.h"
#include "Types/Functors/FunctorObject.h"
#include "Types/Functors/FunctorStatic.h"

/**
 * Delegate extending base with bind and unbind and UnBindAll.
 *
 * @Note: When passing by parameter use reference or pointer, otherwise it will not compile.
 * @Note: All functions passed in are deleted at end of this object life.
 */
template<typename TReturnType = void, typename... TInParams>
class FDelegate : public FDelegateBase<TReturnType, FFunctorBase<TReturnType, TInParams...>*, TInParams...>
{
public:
	using DelegateBase = FDelegateBase<TReturnType, FFunctorBase<TReturnType, TInParams...>*, TInParams...>;

	// Default constructor
	FDelegate()
	{
	}

	// Copy constructor - Forbidden, not efficient + causes double deletion of Functors
	// Could be fixed by creating flag telling destructor if Functors are moved or not
	FDelegate(const FDelegate& InDelegate) = delete;

	// Move constructor
	FDelegate(FDelegate&& InDelegate) noexcept
	{
		DelegateBase::Functors = std::move(InDelegate.DelegateBase::Functors);
	}

	virtual ~FDelegate() override
	{
		for (const typename DelegateBase::Functor* FunctorObject : DelegateBase::Functors)
		{
			delete FunctorObject;
		}
	}

	// Copy operator - Forbidden, not efficient + causes double deletion of Functors
	// Could be fixed by creating flag telling destructor if Functors are moved or not
	FDelegate& operator=(const FDelegate& InDelegate) = delete;

	// Copy assignment operator
	FDelegate& operator=(FDelegate&& InDelegate) noexcept
	{
		DelegateBase::Functors = std::move(InDelegate.DelegateBase::Functors);

		return *this;
	}

	/** Begin FDelegateBase interface */
	/** Executes all bound functions. */
	virtual void Execute(TInParams... InParams) override
	{
		for (ContainerInt i = 0; i < DelegateBase::Functors.Size(); i++)
		{
			if (ENSURE_VALID(DelegateBase::Functors[i] != nullptr))
			{
				DelegateBase::Functors[i]->operator()(InParams ...);
			}
		}
	}

	/** Executes all bound functions using Lambda to define how it executes. */
	using ExecuteByLambdaDefinitionFunctor = FFunctorBase<TReturnType, TInParams...>;
	using ExecuteByLambdaDefinition = FFunctorLambda<void, ExecuteByLambdaDefinitionFunctor*, TInParams...>;
	virtual void ExecuteByLambda(ExecuteByLambdaDefinition Lambda, TInParams... InParams)
	{
		for (ContainerInt i = 0; i < DelegateBase::Functors.Size(); i++)
		{
			Lambda(DelegateBase::Functors[i], InParams ...);
		}
	}
	/** End FDelegateBase interface */

	/** You can make new FFunctorLambda<TReturnType, TInParams...>, Lambdas will be auto destroyed with this delegate. */
	void BindLambda(FFunctorLambda<TReturnType, TInParams...>* Lambda)
	{
		DelegateBase::Functors.Push(Lambda);
	}
	template <typename TTypeAuto>
	void BindLambda(TTypeAuto Lambda)
	{
		// @TODO Can be memory leak
		DelegateBase::Functors.Push(new FFunctorLambda<TReturnType, TInParams...>(Lambda));
	}
	
	void UnBindLambda(FFunctorLambda<TReturnType, TInParams...>& Lambda)
	{
		DelegateBase::Functors.Remove(Lambda);
	}
	void UnBindLambda(FFunctorLambda<TReturnType, TInParams...>&& Lambda)
	{
		DelegateBase::Functors.Remove(Lambda);
	}

	/*
	 * Example on how to bind your object
	 * OnYourDelegateChanged.BindObject(this, &FYourClass::YourFunctionName);
	 * Real example from code:
	 * OnWidgetOrderChanged.BindObject(this, &FWidgetInputManager::AddWidgetDependingOnOrder);
	 */
	template<typename TClass>
	void BindObject(TClass* InClassObject, TReturnType(TClass::* InFunctionPointer)(TInParams...))
	{
		DelegateBase::Functors.Push(new FFunctorObject<TClass, TReturnType, TInParams...>(InClassObject, InFunctionPointer));
	}

	template<typename TClass>
	void BindObject(FFunctorObject<TClass, TReturnType, TInParams...>* Functor)
	{
		DelegateBase::Functors.Push(Functor);
	}

	template<typename TClass>
	void UnBindObject(TClass* InClassObject, TReturnType(TClass::* InFunctionPointer)(TInParams...))
	{
		ContainerInt Index;
		const bool bIsFound = DelegateBase::Functors.FindByLambda([&](FFunctorBase<TReturnType, TInParams...>* Object)
		{
			if (FFunctorObject<TClass, TReturnType, TInParams...>* FunctorObject = dynamic_cast<FFunctorObject<TClass, TReturnType, TInParams...>*>(Object))
			{
				if (FunctorObject->GetClassObject() == InClassObject && FunctorObject->IsFunctionPointerSame(InFunctionPointer))
				{
					return true;
				}
			}

			return false;
		}, Index);

		if (bIsFound)
		{
			delete DelegateBase::Functors[Index];

			DelegateBase::Functors.RemoveAt(Index);
		}
	}

	template<typename TClass>
	void UnBindObject(FFunctorObject<TClass, TReturnType, TInParams...>* Functor)
	{
		DelegateBase::Functors.Remove(Functor);
	}

	void BindStatic(TReturnType(*InFunctionPointer)(TInParams...))
	{
		DelegateBase::Functors.Push(new FFunctorStatic<TReturnType, TInParams...>(InFunctionPointer));
	}
	
	/** Remove function by functor reference. Use with caution. */
	void UnBindAll()
	{
		// Clean DelegateBase::Functors
		for (auto* FunctorObject : DelegateBase::Functors)
		{
			delete FunctorObject;
		}

		DelegateBase::Functors.Clear();
	}
	
};
