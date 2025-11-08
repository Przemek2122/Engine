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
