// Created by Przemys�aw Wiewi�ra 2020

#pragma once

#include "ContainerBase.h"
#include "Types/Functors/FunctorLambda.h"
#include "Includes/EngineMacros.h"
#include "Misc/Math.h"

/**
 * Dynamic array template for any type.
 */
template<typename TType, typename TSizeType = ContainerInt>
class CArray : public CContainerBase<TType, TSizeType>
{
public:
	/** Begin CContainerBase interface */
	NO_DISCARD TSizeType Size() const override
	{
		return static_cast<TSizeType>(Vector.size());
	}
	NO_DISCARD bool IsEmpty() const override
	{
		return Vector.empty();
	}
	/** End CContainerBase interface */

	bool IsValidIndex(const TSizeType Index) const
	{
		return (Index >= 0 && Index < Size());
	}

	bool IsLastIndex(const TSizeType Index) const
	{
		return (Index == Size() - 1);
	}

    TSizeType GetLastIndex() const
    {
        return (Size() - 1);
    }

	void Resize(const TSizeType Number)
	{
		Vector.reserve(Number);
	}

	CArray()
	{
	}

	/** Bracket list constructor */
	CArray(std::initializer_list<TType> List)
	{
		Resize(static_cast<TSizeType>(List.size()));

		for (auto ListItem : List)
		{
			Vector.push_back(ListItem);
		}
	}

	void operator=(const CArray<TType, TSizeType>& Other)
	{
		Vector = Other.Vector;
	}

	void operator+=(const CArray<TType, TSizeType>& Other)
	{
		for (auto& Value : Other.Vector)
		{
			Push(Value);
		}
	}

	bool operator==(const CArray<TType, TSizeType>& Other) const
	{
		return Vector == Other.Vector;
	}

	bool operator!=(const CArray<TType, TSizeType>& Other) const
	{
		return Vector != Other.Vector;
	}

	void Push(TType Value)
	{
		Vector.push_back(Value);
	}
	void Push(TType Value) const
	{
		Vector.push_back(Value);
	}
	void Push(TType* Value)
	{
		Vector.push_back(Value);
	}
	void Push(TType* Value) const
	{
		Vector.push_back(Value);
	}

	void InsertByLambda(TType* Object, FFunctorLambda<bool, TType*, TType*> Function)
	{
		for (auto i = 0; i < Vector.size(); i++)
		{
			if (Function(Vector[i], Object))
			{
				InsertAt(i, Object);

				return;
			}
		}

		// Add at the end if not found
		Push(Object);
	}
	void InsertByLambda(TType& Object, FFunctorLambda<bool, TType&, TType&> Function)
	{
		for (auto i = 0; i < Vector.size(); i++)
		{
			if (Function(Vector[i], Object))
			{
				InsertAt(i, Object);

				return;
			}
		}

		// Add at the end if not found
		Push(Object);
	}
	
	template<typename TTypeAuto>
	void InsertAt(TSizeType Index, TTypeAuto Value)
	{
		Vector.insert(Vector.begin() + Index, Value); 
	}

	/** Iterate and remove object or objects every time lambda returns true. */
	void RemoveByPredicate(FFunctorLambda<bool, TType&> Function, const bool bRemoveOnlyOneInstance = false)
	{
		for (auto i = 0; i < Vector.size(); )
		{
			if (Function(Vector[i]))
			{
				RemoveAt(i);

				if (bRemoveOnlyOneInstance)
				{
					break;
				}
			}
			else
			{
				// Increment only if not removed
				++i;
			}
		}
	}
	
	bool RemoveAt(const TSizeType Index)
	{
		if (Index >= 0 && Index < Size())
		{
			Vector.erase(Vector.begin() + Index);
			
			return true;
		}

		return false;
	}
	
	/** Remove first match. */
	template<typename TTypeAuto>
	bool Remove(TTypeAuto Value)
	{
		auto DeleteIndex = FindIndexOf(Value);

		if (DeleteIndex != -1)
		{
			return RemoveAt(DeleteIndex);
		}
		
		return false;
	}
	
	/** Remove all matches. */
	template<typename TTypeAuto>
	bool RemoveAll(TTypeAuto Value)
	{
		TSizeType RemovedElements = 0;

		for (TSizeType i = 0; i < Size(); )
		{
			if (Vector[i] == Value)
			{
				RemoveAt(i);
			}
			else
			{
				// Increment only if not removed
				++i;
			}
		}

		return RemovedElements > 0;
	}

	/** Remove duplicated elements */
	void RemoveDuplicates()
	{
		Vector.erase(unique(Vector.begin(), Vector.end()), Vector.end());
	}
	
	TType& operator[](TSizeType Index)
	{
		if (IsValidIndex(Index))
		{
			return Vector[Index];
		}
		else
		{
			// Request out of range.
			ENSURE_VALID(false);

			return DefaultType;
		}
	}
	const TType& operator[](TSizeType Index) const
	{
		if (IsValidIndex(Index))
		{
			return Vector[Index];
		}
		else
		{
			// Request out of range.
			ENSURE_VALID(false);

			return DefaultType;
		}
	}
	
	template<typename TTypeAuto>
	constexpr TType& At(TTypeAuto Index)
	{
		return Vector.at(Index);
	}

	TSizeType GetRandomIndex() const
	{
		TSizeType OutIndex;

		if (Size() == 0)
		{
			OutIndex = 0;
		}
		else
		{
			OutIndex = FMath::RandRange(0, Size() - 1);
		}

		return OutIndex;
	}

	TType GetRandomValue() const
	{
		const TSizeType RandomIndex = GetRandomIndex();

		// Could be invalid if array is empty
		if (IsValidIndex(RandomIndex))
		{
			return Vector[RandomIndex];
		}

		return DefaultType;
	}

	bool Contains(const TType& Value) const
	{
		return (std::find(Vector.begin(), Vector.end(), Value) != Vector.end());
	}

	bool ContainsByPredicate(FFunctorLambda<bool, TType&> Function) const
	{
		bool bContains = false;

		for (TType Value : Vector)
		{
			if (Function(Value))
			{
				bContains = true;

				break;
			}
		}

		return bContains;
	}

	/** @return Index or -1 if not found */
	template<typename TTypeAuto>
	NO_DISCARD TSizeType FindIndexOf(TTypeAuto Value)
	{
		const TSizeType VectorSize = static_cast<TSizeType>(Vector.size());
		
		for (TSizeType i = 0; i < VectorSize; ++i)
		{
			if (Vector[i] == Value)
			{
				return i;
			}
		}
		
		return -1;
	}

	/**
	 * Search for index, will return index when @Function returns true.
	 * @note do not use index if function returns false as it might be uninitialised
	 * @returns true if found, false otherwise
	 */
	bool FindIndexByPredicate(FFunctorLambda<bool, TType&> Function, TSizeType& Index)
	{
		for (auto i = 0; i < Vector.size(); i++)
		{
			if (Function(Vector[i]))
			{
				Index = i;

				return true;
			}
		}

		return false;
	}

	template<typename TTypeAuto>
	void Fill(TTypeAuto Value)
	{
		Vector._Ufill(Value);
	}
	
	constexpr void Swap(std::vector<TType>& Other)
	{
		Vector.swap(Other);
	}
	
	template<typename TTypeAuto>
	void SetNum(TTypeAuto NewSize)
	{
		Vector.resize(NewSize);
	}
	
	/** Removes all elements from the container (which are destroyed), leaving the container with a size of 0. */
	void Clear()
	{
		Vector.clear();
	}

	/** Begin of bucket functions */
	NO_DISCARD auto begin() noexcept -> auto
	{
		return Vector.begin();
	}
	NO_DISCARD auto cbegin() noexcept -> auto
	{
		return Vector.cbegin();
	}
	NO_DISCARD auto rbegin() noexcept -> auto
	{
		return Vector.rbegin();
	}
	NO_DISCARD auto crbegin() noexcept -> auto
	{
		return Vector.crbegin();
	}
	
	NO_DISCARD auto end() noexcept -> auto
	{
		return Vector.end();
	}
	NO_DISCARD auto cend() noexcept -> auto
	{
		return Vector.cend();
	}
	NO_DISCARD auto rend() noexcept -> auto
	{
		return Vector.rend();
	}
	NO_DISCARD auto crend() noexcept -> auto
	{
		return Vector.crend();
	}


	NO_DISCARD auto begin() const noexcept -> auto
	{
		return Vector.begin();
	}
	NO_DISCARD auto cbegin() const noexcept -> auto
	{
		return Vector.cbegin();
	}
	NO_DISCARD auto rbegin() const noexcept -> auto
	{
		return Vector.rbegin();
	}
	NO_DISCARD auto crbegin() const noexcept -> auto
	{
		return Vector.crbegin();
	}
	
	NO_DISCARD auto end() const noexcept -> auto
	{
		return Vector.end();
	}
	NO_DISCARD auto cend() const noexcept -> auto
	{
		return Vector.cend();
	}
	NO_DISCARD auto rend() const noexcept -> auto
	{
		return Vector.rend();
	}
	NO_DISCARD auto crend() const noexcept -> auto
	{
		return Vector.crend();
	}
	/** End of bucket functions */

public:
	/** C++ Vector */
	std::vector<TType> Vector;

protected:
	TType DefaultType;

};
