// Created by Przemys³aw Wiewióra 2020

#pragma once

// Public macros

#include "CoreMinimal.h"

#define DEBUG ((defined _DEBUG) && _DEBUG)
#define WITH_WIDGET_DEBUGGER DEBUG

#if DEBUG
// Not inline in debug to be able to see callstack.
#define INLINE_DEBUGABLE
#else
// Inline in release
#define INLINE_DEBUGABLE inline
#endif

typedef Uint8 uint8;
typedef Uint32 uint32;
typedef Uint64 uint64;
typedef int32_t int32;
typedef int64_t int64;

inline bool Inline_ENSURE_VALID_Lambda(auto Condition)
{
	if (Condition)
	{
		return true;
	}
	else
	{
		__debugbreak();

		return false;
	}
};

#if DEBUG
// Just a stop, can be continued
#define ENSURE_VALID(Condition) Inline_ENSURE_VALID_Lambda(Condition)
//#define ENSURE_VALID_MESSAGE(Condition, Message) Inline_ENSURE_VALID_MESSAGE_Lambda(Condition, Message)
#else
// No throw in release
#define ENSURE_VALID(Condition) Condition
//#define ENSURE_VALID_MESSAGE(Condition, Message)
#endif

/**
 * Advanced text, returns std::string, can be used anywhere
 * Can be used like:
 * TEXT_O("Test button " << "1" << 1)
 */
#define TEXT_ADV(Text) (dynamic_cast<std::ostringstream&>(TEXT_OST(Text)).str())
/**
 * more advanced text, returns ostringstream
 * Can be used like:
 * TEXT_O("Test button " << "1" << 1)
 */
#define TEXT_OST(Text) (std::ostringstream().flush() << Text)
/** Most basic text implementation, just changes to char* */
#define TEXT_CHAR(Text) (const_cast<char*>(Text))
#define STRING(Text) std::string(Text)

/** Macro allowing calls to parent class. @note Constructor Super call in init list is not allowed! */
#define Super __super

/** Macro to check if CurrentClass inherits from BaseClass at compile time. FailMessage will be shown in log in case of missing class */
#define ASSERT_IS_BASE_OF(BaseClass, CurrentClass, FailMessage) static_assert(std::is_base_of_v<BaseClass, CurrentClass>, FailMessage);

#define THREAD_WAIT_MS(TimeInMS)			std::this_thread::sleep_for(std::chrono::milliseconds(TimeInMS))
#define THREAD_WAIT_NS(TimeInNS)			std::this_thread::sleep_for(std::chrono::nanoseconds(TimeInNS))
/** Wait for 0.00001 second (or 0.01 ms) */
#define THREAD_WAIT_SHORT_TIME				THREAD_WAIT_NS(10000)
#define THREAD_WAIT_FOR_MUTEX_LOCK(MutexName)	while (!((MutexName).TryLock())) THREAD_WAIT_NS(1000)

#define COUNTER_START(PropertyNameStart)					const auto PropertyNameStart{ std::chrono::high_resolution_clock::now() }
#define COUNTER_END(PropertyNameStart, PropertyNameEnd)		std::chrono::duration<double, std::nano> PropertyNameEnd{ std::chrono::high_resolution_clock::now() - PropertyNameStart }
#define COUNTER_GET_NS(PropertyNameEnd)						PropertyNameEnd.count()
#define COUNTER_GET_MS(PropertyNameEnd)						(COUNTER_GET_NS(PropertyNameEnd) / 1000000.0)
