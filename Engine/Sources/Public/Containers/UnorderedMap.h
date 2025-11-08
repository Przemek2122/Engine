// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "ContainerBase.h"

#include <unordered_map>
#include <map>

/*
 * Class for safe queue.
 * Not thread safe
 */
template<typename TKey, typename TValue, typename TSizeType = int>
class CUnorderedMap : public CContainerBase<TValue, TSizeType>
{
public:
	/** Default constructor */
	CUnorderedMap()
	{
	}

	/** Init list */
	CUnorderedMap(std::initializer_list<std::pair<const std::string, std::string>> init_list)
		: Map(init_list)
	{
	}

	/** Begin CContainerBase interface */
	NO_DISCARD TSizeType Size() const override
	{
		return static_cast<TSizeType>(Map.size());
	}
	NO_DISCARD bool IsEmpty() const override
	{
		return Map.empty();
	}
	/** End CContainerBase interface */

	/** Removes all elements from the container (which are destroyed), leaving the container with a size of 0. */
	NO_DISCARD void Clear()
	{
		Map.clear();
	}

	template<typename TTypeAuto>
	void SetNum(TTypeAuto NewSize)
	{
		Map.resize(NewSize);
	}
	
	template<typename TAutoType>
	NO_DISCARD bool IsValidKey(TAutoType Key)
	{
		return Map.contains(Key);
	}

	template<typename TAutoType>
	TValue& operator[](TAutoType Index)
	{
		return Map[Index];
	}
	template<typename TAutoType>
	TValue& operator[](TAutoType Index) const
	{
		return Map[Index];
	}
	
	template<typename TAutoType>
	TValue At(TAutoType Index)
	{
		return Map.at(Index);
	}
	template<typename TAutoType>
	TValue At(TAutoType Index) const
	{
		return Map.at(Index);
	}
	
	template<typename TKeyAuto, typename TValueAuto>
	NO_DISCARD void Emplace(TKeyAuto Key, TValueAuto Value)
	{
		Map.emplace(Key, Value);
	}
	template<typename TKeyAuto, typename TValueAuto>
	NO_DISCARD void Emplace(TKeyAuto Key, TValueAuto Value) const
	{
		Map.emplace(Key, Value);
	}

	template<typename TKeyAuto, typename TValueAuto>
	NO_DISCARD void InsertOrAssign(TKeyAuto Key, TValueAuto Value)
	{
		Map.insert_or_assign(Key, Value);
	}
	
	template<typename TAutoType>
	bool Remove(TAutoType Key)
	{
		return Map.erase(Key);
	}
	
	template<typename TAutoType>
	NO_DISCARD bool ContainsKey(TAutoType Key)
	{
		return Map.contains(Key);
	}

	template<typename TAutoType>
	NO_DISCARD bool ContainsKey(TAutoType Key) const
	{
		return Map.contains(Key);
	}

	/** Very slow iterator */
	template<typename TAutoType>
	NO_DISCARD bool ContainsValue(TAutoType Value)
	{
		for (const auto& Pair : Map)
		{
			if (Pair.second == Value)
			{
				return true;
			}
		}
		return false;
	}

	/** Very slow iterator */
	template<typename TAutoType>
	NO_DISCARD bool ContainsValue(TAutoType Value) const
	{
		for (const auto& Pair : Map)
		{
			if (Pair.second == Value)
			{
				return true;
			}
		}
		return false;
	}

	NO_DISCARD bool ContainsByPredicate(FFunctorLambda<bool, TKey, TValue> Delegate) const
	{
		bool bContains = false;

		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			if (Delegate(it->first, it->second))
			{
				bContains = true;

				break;
			}
		}

		return bContains;
	}
	
	template<typename TAutoType>
	NO_DISCARD TValue FindValueByKey(TAutoType Key)
	{
		return Map.find(Key)->second;
	}
	template<typename TAutoType>
	NO_DISCARD TValue FindKeyByValue(TAutoType Value)
	{
		return Map.find(Value);
	}
	
	template<typename TAutoType>
	NO_DISCARD void SetAll(TAutoType Value)
	{
		typename std::map<TKey, TValue>::iterator Iterator;
		
		for (Iterator = Map.begin(); Iterator != Map.end(); ++Iterator)
		{
			Iterator->second = Value;
		}
	}

	NO_DISCARD void MapIterator(FFunctorLambda<void, TKey, TValue> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->first, it->second);
		}
	}

	NO_DISCARD void MapIteratorByIndexOnly(FFunctorLambda<void, TKey> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->first);
		}
	}

	NO_DISCARD void MapIteratorByValueOnly(FFunctorLambda<void, TValue> Delegate)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			Delegate(it->second);
		}
	}

	/** Begin of bucket functions */
	NO_DISCARD auto begin() noexcept -> auto
	{
		return Map.begin();
	}
	NO_DISCARD auto cbegin() noexcept -> auto
	{
		return Map.cbegin();
	}
	NO_DISCARD auto rbegin() noexcept -> auto
	{
		return Map.rbegin();
	}
	NO_DISCARD auto crbegin() noexcept -> auto
	{
		return Map.crbegin();
	}
	
	NO_DISCARD auto end() noexcept -> auto
	{
		return Map.end();
	}
	NO_DISCARD auto cend() noexcept -> auto
	{
		return Map.cend();
	}
	NO_DISCARD auto rend() noexcept -> auto
	{
		return Map.rend();
	}
	NO_DISCARD auto crend() noexcept -> auto
	{
		return Map.crend();
	}


	NO_DISCARD auto begin() const noexcept -> auto
	{
		return Map.begin();
	}
	NO_DISCARD auto cbegin() const noexcept -> auto
	{
		return Map.cbegin();
	}
	NO_DISCARD auto rbegin() const noexcept -> auto
	{
		return Map.rbegin();
	}
	NO_DISCARD auto crbegin() const noexcept -> auto
	{
		return Map.crbegin();
	}
	
	NO_DISCARD auto end() const noexcept -> auto
	{
		return Map.end();
	}
	NO_DISCARD auto cend() const noexcept -> auto
	{
		return Map.cend();
	}
	NO_DISCARD auto rend() const noexcept -> auto
	{
		return Map.rend();
	}
	NO_DISCARD auto crend() const noexcept -> auto
	{
		return Map.crend();
	}
	/** End of bucket functions */

public:
	// C++ Map
	std::unordered_map<TKey, TValue> Map;
};
