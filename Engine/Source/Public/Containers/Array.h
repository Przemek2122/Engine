// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "ContainerBase.h"
#include "Types/Functors/FunctorLambda.h"
#include "Includes/EngineMacros.h"

#include <vector>

#include "Misc/Math.h"

/**
 * Dynamic array template for any type.
 */
template<typename TType, typename TSizeType = ContainerInt>
class CArray : public CContainerBase<TType, TSizeType>
{
public:
	/** Begin CContainerBase interface */
	_NODISCARD SDL_FORCE_INLINE TSizeType Size() const override
	{
		return static_cast<TSizeType>(Vector.size());
	}
	_NODISCARD SDL_FORCE_INLINE bool IsEmpty() const override
	{
		return Vector.empty();
	}
	/** End CContainerBase interface */

	bool IsValidIndex(const TSizeType Index) const
	{
		return (Index >= 0 && Index < Size());
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

	SDL_FORCE_INLINE void Push(TType Value)
	{
		Vector.push_back(Value);
	}
	SDL_FORCE_INLINE void Push(TType Value) const
	{
		Vector.push_back(Value);
	}
	SDL_FORCE_INLINE void Push(TType* Value)
	{
		Vector.push_back(Value);
	}
	SDL_FORCE_INLINE void Push(TType* Value) const
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
	SDL_FORCE_INLINE void InsertAt(TSizeType Index, TTypeAuto Value)
	{
		Vector.insert(Vector.begin() + Index, Value); 
	}
	
	SDL_FORCE_INLINE bool RemoveAt(const TSizeType Index)
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
	SDL_FORCE_INLINE bool Remove(TTypeAuto Value)
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
	SDL_FORCE_INLINE bool RemoveAll(TTypeAuto Value)
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
	
	SDL_FORCE_INLINE TType& operator[](TSizeType Index)
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
	SDL_FORCE_INLINE const TType& operator[](TSizeType Index) const
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
	SDL_FORCE_INLINE constexpr TType& At(TTypeAuto Index)
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

	SDL_FORCE_INLINE TType GetRandomValue() const
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

	/** @return Index or -1 if not found */
	template<typename TTypeAuto>
	_NODISCARD SDL_FORCE_INLINE TSizeType FindIndexOf(TTypeAuto Value)
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

	/** @returns true if found @note do not use index if function returns false as it might be unitialised */
	bool FindByLambdaByComparingObjects(TType& Object, FFunctorLambda<bool, TType&, TType&> Function, TSizeType& Index)
	{
		for (auto i = 0; i < Vector.size(); i++)
		{
			if (Function(Vector[i], Object))
			{
				Index = i;

				return true;
			}
		}

		return false;
	}


	/** @returns true if found @note do not use index if function returns false as it might be uninitialised */
	template<typename TAutoType>
	bool FindByLambda(TAutoType Function, TSizeType& Index)
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
	SDL_FORCE_INLINE void Fill(TTypeAuto Value)
	{
		Vector._Ufill(Value);
	}
	
	SDL_FORCE_INLINE constexpr void Swap(std::vector<TType>& Other)
	{
		Vector.swap(Other);
	}
	
	template<typename TTypeAuto>
	SDL_FORCE_INLINE void SetNum(TTypeAuto NewSize)
	{
		Vector.resize(NewSize);
	}
	
	/** Removes all elements from the container (which are destroyed), leaving the container with a size of 0. */
	SDL_FORCE_INLINE void Clear()
	{
		Vector.clear();
	}

	/** Begin of bucket functions */
	_NODISCARD SDL_FORCE_INLINE auto begin() noexcept -> auto
	{
		return Vector.begin();
	}
	_NODISCARD SDL_FORCE_INLINE auto cbegin() noexcept -> auto
	{
		return Vector.cbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto rbegin() noexcept -> auto
	{
		return Vector.rbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto crbegin() noexcept -> auto
	{
		return Vector.crbegin();
	}
	
	_NODISCARD SDL_FORCE_INLINE auto end() noexcept -> auto
	{
		return Vector.end();
	}
	_NODISCARD SDL_FORCE_INLINE auto cend() noexcept -> auto
	{
		return Vector.cend();
	}
	_NODISCARD SDL_FORCE_INLINE auto rend() noexcept -> auto
	{
		return Vector.rend();
	}
	_NODISCARD SDL_FORCE_INLINE auto crend() noexcept -> auto
	{
		return Vector.crend();
	}


	_NODISCARD SDL_FORCE_INLINE auto begin() const noexcept -> auto
	{
		return Vector.begin();
	}
	_NODISCARD SDL_FORCE_INLINE auto cbegin() const noexcept -> auto
	{
		return Vector.cbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto rbegin() const noexcept -> auto
	{
		return Vector.rbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto crbegin() const noexcept -> auto
	{
		return Vector.crbegin();
	}
	
	_NODISCARD SDL_FORCE_INLINE auto end() const noexcept -> auto
	{
		return Vector.end();
	}
	_NODISCARD SDL_FORCE_INLINE auto cend() const noexcept -> auto
	{
		return Vector.cend();
	}
	_NODISCARD SDL_FORCE_INLINE auto rend() const noexcept -> auto
	{
		return Vector.rend();
	}
	_NODISCARD SDL_FORCE_INLINE auto crend() const noexcept -> auto
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
