// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "ContainerBase.h"

/*
 * Class for safe queue.
 * Not thread safe
 */
template<typename TType, typename TSizeType = int>
class CDeque : public CContainerBase<TType, TSizeType>
{
public:
	/** Begin CContainerBase interface */
	TSizeType Size() const override
	{
		return static_cast<int>(Deque.size());
	};
	bool IsEmpty() const override
	{
		return Deque.empty();
	}
	/** End CContainerBase interface */

	void Clear()
	{
		Deque.clear();
	}

	/** @Returns first element without removing. */
	TType& PeekFirst()
	{
		return Deque.front();
	}
	/** @Returns first element without removing. */
	TType PeekFirst() const
	{
		return Deque.front();
	}

	/** @Returns last element without removing. */
	TType& PeekLast()
	{
		return Deque.back();
	}
	/** @Returns last element without removing. */
	TType PeekLast() const
	{
		return Deque.back();
	}

	/**
	 * @Returns a vector of the first 'count' elements.
	 * Useful for: Getting the first N messages in a queue.
	 */
	std::vector<TType> PeekFirst(TSizeType count) const
	{
		// 1. Safety: Don't try to get more than we have
		size_t safeCount = std::min((size_t)count, Deque.size());

		// 2. Create vector to hold results
		std::vector<TType> result;
		result.reserve(safeCount); // Optimization: reserve memory

		// 3. Copy elements from the beginning
		// Assuming Deque is std::deque or std::vector
		for (size_t i = 0; i < safeCount; ++i) {
			result.push_back(Deque[i]);
		}

		return result;
	}

	/**
	 * @Returns a vector of the last 'count' elements.
	 * Useful for: "Load last 10 messages" in chat history.
	 * Returns them in chronological order (Oldest -> Newest).
	 */
	std::vector<TType> PeekLast(TSizeType count) const
	{
		// 1. Safety: Don't try to get more than we have
		size_t safeCount = std::min((size_t)count, Deque.size());

		// 2. Create vector
		std::vector<TType> result;
		result.reserve(safeCount);

		// 3. Calculate start index
		// If size is 100 and we want 5, we start at index 95
		size_t startIndex = Deque.size() - safeCount;

		// 4. Copy elements from start index to end
		for (size_t i = startIndex; i < Deque.size(); ++i) {
			result.push_back(Deque[i]);
		}

		return result;
	}

	std::vector<TType> GetRange(TSizeType Offset, TSizeType Count) const
	{
		if (Offset >= Deque.size() || Count == 0)
		{
			return { };
		}

		Count = std::min(static_cast<TSizeType>(Count), static_cast<TSizeType>(Deque.size() - Offset));

		std::vector<TType> OutResult;
		OutResult.reserve(Count);

		auto Start = Deque.begin() + Offset;
		auto End = Start + Count;
		OutResult.insert(OutResult.end(), Start, End);

		return OutResult;
	}

	template<typename TTypeAuto>
	TType GetPositionOf(TTypeAuto Value) const
	{
		auto Iterator = std::find(Deque->c.begin(), Deque->c.end(), Value);
		return std::distance(Deque->c.begin(), Iterator);
	}

	TType& operator[](size_t Index)
	{
		return Deque[Index];
	}

	const TType& operator[](size_t Index) const
	{
		return Deque[Index];
	}

	template<typename TTypeAuto>
	void PushBack(TTypeAuto Value)
	{
		Deque.push_back(Value);
	}
	template<typename TTypeAuto>
	void PushBack(TTypeAuto Value) const
	{
		Deque.push_back(Value);
	}
	
	template<typename TTypeAuto>
	void PushFront(TTypeAuto Value)
	{
		Deque.push_front(Value);
	}
	template<typename TTypeAuto>
	void PushFront(TTypeAuto Value) const
	{
		Deque.push_front(Value);
	}

	void DequeFront()
	{
		Deque.pop_front();
	}
	void DequeFront() const
	{
		Deque.pop_front();
	}
	void DequeBack()
	{
		Deque.pop_back();
	}
	void DequeBack() const
	{
		Deque.pop_back();
	}
	
	/** C++ deque. */
	std::deque<TType> Deque;
};
