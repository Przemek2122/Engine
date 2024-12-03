// Created by Przemys³aw Wiewióra 2020-2024

#pragma once

#include "CoreMinimal.h"

class EUnitBase;

/** Do not use directly - Use FClassStorage instead */
template<typename TType, typename... TArgs>
class TClass
{
public:
	virtual ~TClass() = default;

	/** Create instance */
	virtual void* Allocate(TArgs... Args) const
	{
        return new TType(Args...);
    }

	std::string GetClassName() const
	{
		return typeid(TType).name();
	}
};

/** Storage class for storing TClass */
template<typename TBaseClass, typename... TArgs>
class FClassStorage
{
	typedef TClass<TBaseClass, TArgs...> TClassTypeWithArgs;

public:
	/** Default */
	FClassStorage()
		: StoredClass(nullptr)
		, bIsMoved(false)
	{
	}

	/** Copy */
	FClassStorage(const FClassStorage& ClassStorage)
		: StoredClass(nullptr)
		, bIsMoved(false)
	{
		PerformCopy(ClassStorage);
	}

	/** Move */
	FClassStorage(FClassStorage&& ClassStorage) noexcept
		: StoredClass(ClassStorage.StoredClass)
		, bIsMoved(false)
	{
		ClassStorage.bIsMoved = true;
	}

	virtual ~FClassStorage()
	{
		if (!bIsMoved)
		{
			delete StoredClass;
		}
	}

	/** @Note: Assign to self will result in crash */
	FClassStorage& operator=(const FClassStorage& Other)
	{
		PerformCopy(Other);

		return *this;
	}

	/** Set stored class. */
	template<typename TType>
	void Set()
	{
		// Perform check for base class, it has to be child of TType
		static_assert(std::is_base_of_v<TBaseClass, TType>, "FClassStorage::Set given class does not inherit from TBaseClass (Class set as base in FClassStorage)");

		auto TempClass = new TClass<TType, TArgs...>();

		StoredClass = (TClassTypeWithArgs*)(TempClass);

		// Should never happen due to compile time static_assert on begin of the function
		// but just in case we do not want any kind of memory leak
		if (StoredClass == nullptr)
		{
			delete TempClass;
		}
	}

	/**
	 * Function for actually creating class instance.
	 *
	 * @Note: If class is not set, it will use default BaseClass
	 * @Note: Will not be garbage collected any way so make sure to delete it after use!
	 */
	_NODISCARD TBaseClass* Allocate(TArgs... Args)
	{
		if (StoredClass == nullptr)
		{
			Set<TBaseClass>();
		}

		return static_cast<TBaseClass*>(StoredClass->Allocate(Args...));
	}

	/** @returns true if any type is in storage */
	_NODISCARD bool IsSet() const
	{
		return StoredClass != nullptr;
	}

	template<typename TType>
	_NODISCARD bool IsSame()
	{
		if (StoredClass == nullptr)
		{
			return false;
		}

		return (dynamic_cast<TClass<TType>*>(StoredClass) != nullptr);
	}

	std::string GetClassName() const
	{
		if (StoredClass == nullptr)
		{
			return "nullptr";
		}

		return StoredClass->GetClassName();
	}

	void Reset()
	{
		StoredClass = nullptr;
	}

protected:
	void PerformCopy(const FClassStorage& Other)
	{
		if (Other.StoredClass != nullptr)
		{
			// Make copy
			StoredClass = static_cast<TClassTypeWithArgs*>(malloc(sizeof(Other.StoredClass)));
			memcpy(StoredClass, Other.StoredClass, sizeof(Other.StoredClass));
		}
		else
		{
			StoredClass = nullptr;
		}
	}

protected:
	TClassTypeWithArgs* StoredClass;
	bool bIsMoved;

};

/** Storage class for storing subclasses of EEntity (Includes FEntityManager* by default) */
template<typename TBaseClass, typename... TArgs>
class FEntityClassStorage : public FClassStorage<TBaseClass, FEntityManager*, TArgs...>
{
};
