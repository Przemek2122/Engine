// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "Deque.h"

/*
 * Class for safe queue.
 * Thread safe
 */
template<typename TType, typename TSizeType = int>
class CQueueSafe : public CDeque<TType, TSizeType>
{
public:
	/* Add element at end. */
	inline void PushFrontSafe(const TType& Value)
	{
		std::lock_guard<std::mutex> Lock(Mutex);

		CDeque<TType>::PushFront(Value);
	}
	inline void PushBackSafe(const TType& Value)
	{
		std::lock_guard<std::mutex> Lock(Mutex);

		CDeque<TType>::PushBack(Value);
	}

	/* Delete first element. */
	inline void DequeFrontSafe()
	{
		std::lock_guard<std::mutex> Lock(Mutex);

		CDeque<TType>::DequeFront();
	}
	/* Delete last element. */
	inline void DequeBackSafe()
	{
		std::lock_guard<std::mutex> Lock(Mutex);

		CDeque<TType>::DequeBack();
	}

protected:
	std::mutex Mutex;

};
