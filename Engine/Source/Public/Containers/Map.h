// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "ContainerBase.h"

#include <map>

/*
 * Class for safe queue.
 * Not thread safe
 */
template<typename TKey, typename TValue, typename TSizeType = ContainerInt>
class CMap : public CContainerBase<TValue, TSizeType>
{
public:
	CMap()
	{
	}

	/** Begin CContainerBase interface */
	_NODISCARD SDL_FORCE_INLINE TSizeType Size() const override
	{
		return static_cast<TSizeType>(Map.size());
	}
	_NODISCARD SDL_FORCE_INLINE bool IsEmpty() const override
	{
		return Map.empty();
	}
	/** End CContainerBase interface */

	/** Removes all elements from the container (which are destroyed), leaving the container with a size of 0. */
	_NODISCARD SDL_FORCE_INLINE void Clear()
	{
		Map.clear();
	}
	
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool IsValidKey(const TAutoType Key)
	{
		return HasKey(Key);
	}
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool IsValidKey(const TAutoType Key) const
	{
		return HasKey(Key);
	}
	
	template<typename TAutoType>
	_NODISCARD bool HasKey(TAutoType Key)
	{
		return Map.find(Key) != Map.end();
	}

	template<typename TAutoType>
	SDL_FORCE_INLINE TValue& operator[](TAutoType Key)
	{
		if (!Map.contains(Key))
		{
			// We should never encounter default type.
			ENSURE_VALID(false);

			return DefaultType;
		}

		return Map[Key];
	}
	template<typename TAutoType>
	SDL_FORCE_INLINE TValue& operator[](TAutoType Key) const
	{
		if (!Map.contains(Key))
		{
			// We should never encounter default type.
			ENSURE_VALID(false);

			return DefaultType;
		}

		return Map[Key];
	}
	
	template<typename TAutoType>
	SDL_FORCE_INLINE TValue At(TAutoType Index)
	{
		return Map.at(Index);
	}
	template<typename TAutoType>
	SDL_FORCE_INLINE TValue At(TAutoType Index) const
	{
		return Map.at(Index);
	}
	
	template<typename TKeyAuto, typename TValueAuto>
	_NODISCARD SDL_FORCE_INLINE void Emplace(TKeyAuto Key, TValueAuto Value)
	{
		Map.emplace(Key, Value);
	}
	template<typename TKeyAuto, typename TValueAuto>
	_NODISCARD SDL_FORCE_INLINE void Emplace(TKeyAuto Key, TValueAuto Value) const
	{
		Map.emplace(Key, Value);
	}

	template<typename TKeyAuto, typename TValueAuto>
	_NODISCARD SDL_FORCE_INLINE void InsertOrAssign(TKeyAuto Key, TValueAuto Value)
	{
		Map.insert_or_assign(Key, Value);
	}
	
	template<typename TAutoType>
	SDL_FORCE_INLINE bool Remove(TAutoType Key)
	{
		return Map.erase(Key);
	}
	
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool ContainsKey(TAutoType Key)
	{
		for (auto& Pair : Map)
		{
			if (Pair.first == Key)
			{
				return true;
			}
		}

		return false;
	}
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool ContainsKey(TAutoType Key) const
	{
		for (auto& Pair : Map)
		{
			if (Pair.first == Key)
			{
				return true;
			}
		}

		return false;
	}
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool ContainsValue(TAutoType Value)
	{
		for (auto& Pair : Map)
		{
			if (Pair.second == Value)
			{
				return true;
			}
		}

		return false;
	}
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE bool ContainsValue(TAutoType Value) const
	{
		for (auto& Pair : Map)
		{
			if (Pair.second == Value)
			{
				return true;
			}
		}

		return false;
	}

	_NODISCARD SDL_FORCE_INLINE bool ContainsByPredicate(FFunctorLambda<bool, TKey, TValue> Delegate) const
	{
		bool bContains = false;

		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			if (Delegate(it->first, it->second))
			{
				bContains = true;

				continue;
			}
		}

		return bContains;
	}
	
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE TValue FindValueByKey(TAutoType Key)
	{
		typename std::map<TKey, TValue>::iterator Iterator = Map.find(Key);
		return ((Iterator == Map.end()) ? Iterator->second : TValue());
	}
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE TKey FindKeyByValue(TAutoType Value)
	{
		for (auto Iterator = Map.begin(); Iterator != Map.end(); ++Iterator)
		{
		    if (Iterator->second == Value)
		    {
				return Iterator->first;
		    }
		}

		return TKey();
	}
	
	template<typename TAutoType>
	_NODISCARD SDL_FORCE_INLINE void SetAll(TAutoType Value)
	{
		for (auto Iterator = Map.begin(); Iterator != Map.end(); ++Iterator)
		{
			Iterator->second = Value;
		}
	}

	void MapIterator(FFunctorLambda<void, TKey, TValue> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->first, it->second);
		}
	}

	void MapIteratorByIndexOnly(FFunctorLambda<void, TKey> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->first);
		}
	}

	void MapIteratorByValueOnly(FFunctorLambda<void, TValue> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->second);
		}
	}

	//void Sort(FFunctorLambda<bool, const std::pair<TKey, TValue>&, const std::pair<TKey, TValue>&> Delegate)
	//{
	//	Map.sort(Delegate);
	//}

	/** Begin of bucket functions */
	_NODISCARD SDL_FORCE_INLINE auto begin() noexcept -> auto
	{
		return Map.begin();
	}
	_NODISCARD SDL_FORCE_INLINE auto cbegin() noexcept -> auto
	{
		return Map.cbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto rbegin() noexcept -> auto
	{
		return Map.rbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto crbegin() noexcept -> auto
	{
		return Map.crbegin();
	}
	
	_NODISCARD SDL_FORCE_INLINE auto end() noexcept -> auto
	{
		return Map.end();
	}
	_NODISCARD SDL_FORCE_INLINE auto cend() noexcept -> auto
	{
		return Map.cend();
	}
	_NODISCARD SDL_FORCE_INLINE auto rend() noexcept -> auto
	{
		return Map.rend();
	}
	_NODISCARD SDL_FORCE_INLINE auto crend() noexcept -> auto
	{
		return Map.crend();
	}


	_NODISCARD SDL_FORCE_INLINE auto begin() const noexcept -> auto
	{
		return Map.begin();
	}
	_NODISCARD SDL_FORCE_INLINE auto cbegin() const noexcept -> auto
	{
		return Map.cbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto rbegin() const noexcept -> auto
	{
		return Map.rbegin();
	}
	_NODISCARD SDL_FORCE_INLINE auto crbegin() const noexcept -> auto
	{
		return Map.crbegin();
	}
	
	_NODISCARD SDL_FORCE_INLINE auto end() const noexcept -> auto
	{
		return Map.end();
	}
	_NODISCARD SDL_FORCE_INLINE auto cend() const noexcept -> auto
	{
		return Map.cend();
	}
	_NODISCARD SDL_FORCE_INLINE auto rend() const noexcept -> auto
	{
		return Map.rend();
	}
	_NODISCARD SDL_FORCE_INLINE auto crend() const noexcept -> auto
	{
		return Map.crend();
	}
	/** End of bucket functions */

public:
	// C++ Map
	std::map<TKey, TValue> Map;

protected:
	TValue DefaultType;

};
