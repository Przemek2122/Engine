// Created by Przemys³aw Wiewióra 2020

#pragma once
#include "CoreMinimal.h"

class FUIMenu;

/**
 * Class managing other class pointer lifecycle
 * @Note if you declare this class twice with the same pointer, it will crash.
 */
template<typename TTypeToStore>
class FAutoDeletePointer
{
public:
	FAutoDeletePointer()
		: StoredObject(nullptr)
		, bHasAnyObject(false)
	{
	}

	/** Normal constructor - Create object to store */
	template<typename ...TInParams>
	explicit FAutoDeletePointer(TInParams... Params)
		: StoredObject(new TTypeToStore(Params...))
		, bHasAnyObject(true)
	{
	}

	/** Normal constructor - Pass in object to store */
	explicit FAutoDeletePointer(TTypeToStore* Params)
		: StoredObject(Params)
		, bHasAnyObject(true)
	{
	}

	/** Copy constructor - Makes previous FAutoDeletePointer not usable anymore. */
	explicit FAutoDeletePointer(FAutoDeletePointer& OtherAutoDeletePointer) noexcept 
		: StoredObject(OtherAutoDeletePointer.StoredObject)
		, bHasAnyObject(true)
	{
		OtherAutoDeletePointer.MarkAsMoved();
	}

	/** Move constructor - Makes previous FAutoDeletePointer not usable anymore. */
	FAutoDeletePointer(FAutoDeletePointer&& OtherAutoDeletePointer) noexcept 
		: StoredObject(OtherAutoDeletePointer.StoredObject)
		, bHasAnyObject(true)
	{
		OtherAutoDeletePointer.MarkAsMoved();
	}

	/** Destructor - Destroy stored class. */
	virtual ~FAutoDeletePointer()
	{
		if (bHasAnyObject)
		{
			delete StoredObject;
		}
	}

	/** Move assignment operator */
	FAutoDeletePointer& operator=(FAutoDeletePointer&& OtherAutoDeletePointer) noexcept
	{
		// We can not move class if there is already a class.
		ENSURE_VALID(this->StoredObject == nullptr);

		this->StoredObject = OtherAutoDeletePointer.StoredObject;

		OtherAutoDeletePointer.MarkAsMoved();

		bHasAnyObject = (this->StoredObject != nullptr);

        return *this;
    }

	/** Copy assignment operator. */
	//FAutoDeletePointer& operator=(FAutoDeletePointer OtherAutoDeletePointer) = delete;

	TTypeToStore* Get() const
	{
#if _DEBUG
		// This pointer was
		// not initialized
		// OR
		// moved to other
		// It is missing an object!
		if (ENSURE_VALID(bHasAnyObject))
		{
			if (ENSURE_VALID(StoredObject != nullptr))
			{
				return StoredObject;
			}
			else
			{
				return nullptr;
			}
		}
#endif

		return StoredObject;
	}

	/**
	 * Get stored element.
	 */
	inline virtual TTypeToStore* operator->() const
	{
		return Get();
	}

	bool HasAnyObject() const
	{
		return bHasAnyObject;
	}

	friend bool operator==(const FAutoDeletePointer& A, const FAutoDeletePointer& B)
	{
		return (A.StoredObject == B.StoredObject);
	}

protected:
	/** Used by 'Move constructor' */
	void MarkAsMoved()
	{
		// Moving already moved pointer or moving pointer without class.
		ENSURE_VALID(bHasAnyObject);

		bHasAnyObject = false;
		StoredObject = nullptr;
	}

protected:
	/** Our managed object. */
	TTypeToStore* StoredObject;

	/** This property tells us if this class has any object. Can be false if moved or never initialized. */
	bool bHasAnyObject;
};
