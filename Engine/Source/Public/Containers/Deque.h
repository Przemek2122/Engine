// Created by Przemys³aw Wiewióra 2020

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
	SDL_FORCE_INLINE TSizeType Size() const override
	{
		return static_cast<int>(Deque.size());
	};
	SDL_FORCE_INLINE bool IsEmpty() const override
	{
		return Deque.empty();
	}
	/** End CContainerBase interface */

	void Clear()
	{
		Deque.clear();
	}

	/** @Returns first element without removing. */
	SDL_FORCE_INLINE TType& PeekFirst()
	{
		return Deque.front();
	}
	/** @Returns first element without removing. */
	SDL_FORCE_INLINE TType PeekFirst() const
	{
		return Deque.front();
	}

	/** @Returns last element without removing. */
	SDL_FORCE_INLINE TType& PeekLast()
	{
		return Deque.back();
	}
	/** @Returns last element without removing. */
	SDL_FORCE_INLINE TType PeekLast() const
	{
		return Deque.back();
	}

	template<typename TTypeAuto>
	SDL_FORCE_INLINE TType GetPositionOf(TTypeAuto Value) const
	{
		auto Iterator = std::find(Deque->c.begin(), Deque->c.end(), Value);
		return std::distance(Deque->c.begin(), Iterator);
	}

	SDL_FORCE_INLINE TType& operator[](size_t Index)
	{
		return Deque[Index];
	}

	SDL_FORCE_INLINE const TType& operator[](size_t Index) const
	{
		return Deque[Index];
	}

	template<typename TTypeAuto>
	SDL_FORCE_INLINE void PushBack(TTypeAuto Value)
	{
		Deque.push_back(Value);
	}
	template<typename TTypeAuto>
	SDL_FORCE_INLINE void PushBack(TTypeAuto Value) const
	{
		Deque.push_back(Value);
	}
	
	template<typename TTypeAuto>
	SDL_FORCE_INLINE void PushFront(TTypeAuto Value)
	{
		Deque.push_front(Value);
	}
	template<typename TTypeAuto>
	SDL_FORCE_INLINE void PushFront(TTypeAuto Value) const
	{
		Deque.push_front(Value);
	}

	SDL_FORCE_INLINE void DequeFront()
	{
		Deque.pop_front();
	}
	SDL_FORCE_INLINE void DequeFront() const
	{
		Deque.pop_front();
	}
	SDL_FORCE_INLINE void DequeBack()
	{
		Deque.pop_back();
	}
	SDL_FORCE_INLINE void DequeBack() const
	{
		Deque.pop_back();
	}
	
	/** C++ deque. */
	std::deque<TType> Deque;
};
