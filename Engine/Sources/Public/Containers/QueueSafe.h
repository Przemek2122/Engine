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
		while (!Mutex.try_lock())
		{
			THREAD_WAIT_SHORT_TIME;
		}

		CDeque<TType>::PushFront(Value);

		Mutex.unlock();
	}
	inline void PushBackSafe(const TType& Value)
	{
		while (!Mutex.try_lock())
		{
			THREAD_WAIT_SHORT_TIME;
		}

		CDeque<TType>::PushBack(Value);

		Mutex.unlock();
	}

	/* Delete first element. */
	inline void DequeFrontSafe()
	{
		while (!Mutex.try_lock())
		{
			THREAD_WAIT_SHORT_TIME;
		}

		CDeque<TType>::DequeFront();

		Mutex.unlock();
	}
	/* Delete last element. */
	inline void DequeBackSafe()
	{
		while (!Mutex.try_lock())
		{
			THREAD_WAIT_SHORT_TIME;
		}

		CDeque<TType>::DequeBack();

		Mutex.unlock();
	}

protected:
	mutable std::mutex Mutex;

};
