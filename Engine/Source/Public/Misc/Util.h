// Created by Przemys�aw Wiewi�ra 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "Containers/QueueSafe.h"

enum class ENGINE_API ELogMessageType : Uint8
{
	Message_Info,
	Message_Debug,
	Message_Warning,
	Message_Error,
};

struct ENGINE_API FLogMessage
{
	ELogMessageType Type;
	std::string Text;
};

// Different XOR masks for bit flipping
static constexpr uint64_t ALTERNATING_1 = 0xAAAAAAAAAAAAAAAAULL; // 10101010...
static constexpr uint64_t ALTERNATING_2 = 0x5555555555555555ULL; // 01010101...
static constexpr uint64_t CHECKERBOARD = 0xA5A5A5A5A5A5A5A5ULL; // 10100101...
static constexpr uint64_t INVERSE_CHECKER = 0x5A5A5A5A5A5A5AULL;   // 01011010...

// Nibble-based patterns (4-bit groups)
static constexpr uint64_t NIBBLE_FLIP = 0xF0F0F0F0F0F0F0F0ULL; // 11110000...
static constexpr uint64_t NIBBLE_LOW = 0x0F0F0F0F0F0F0F0FULL; // 00001111...

// Byte-based patterns
static constexpr uint64_t BYTE_HIGH = 0xFF00FF00FF00FF00ULL; // High nibbles
static constexpr uint64_t BYTE_LOW = 0x00FF00FF00FF00FFULL; // Low nibbles

// Mathematical constants
/** Best are hidden, make your own for safety */
static constexpr uint64_t PI_BASED = 0x243F6A8885A308D3ULL; // Pi fractional part

// Prime-based
static constexpr uint64_t LARGE_PRIME = 0xFFFFFFFFFFFFFFC5ULL; // 2^64 - 59
static constexpr uint64_t MERSENNE = 0x1FFFFFFFFFFFFFULL;   // 2^61 - 1

// Sparse patterns
static constexpr uint64_t EVERY_8TH = 0x0101010101010101ULL; // Bit 0,8,16,24...
static constexpr uint64_t CORNERS = 0x8000000000000001ULL; // Just first and last

static constexpr std::string_view CHARACTER_SET_BASE62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * Global utilities class, with:
 * - log (Info, Debug, Warn, Error)
 * - time conversions
 */
class ENGINE_API FUtil
{
public:
	/**
	 * Use this function as one of first functions.
	 * Creates thread for logs
	 * @param EnableLogging specify if it should log to file or not
	 */
	static void LogInit(bool EnableLogging = true);
	static void EndLogging();

	/** Thread for messages on console. */
	static int MessagesPrinter(void* ptr);

	/** @returns current nano-second since epoch(1 January 1970) - signed integer type of at least 64 bits */
	static size_t GetNanoSeconds();
	/** @returns current micro-second since epoch(1 January 1970) - signed integer type of at least 55 bits */
	static size_t GetMircoSeconds();
	/** @returns current mili-second since epoch(1 January 1970) - signed integer type of at least 45 bits */
	static size_t GetMiliSeconds();
	/** @returns current second since epoch(1 January 1970) - signed integer type of at least 35 bits */
	static size_t GetSeconds();

	/** Convert nanoseconds to seconds (1e9) */
	static double NanoSecondToSecond(const size_t InNanosecond);

	/* Convert MicroSeconds to seconds (1e6) */
	static double MicroSecondToSecond(const size_t InNanosecond);

	/* Convert MilliSeconds to seconds (1e3) */
	static double MilliSecondToSecond(const size_t InNanosecond);

	/* Convert seconds to nanoseconds (1e9) */
	static size_t SecondToNanoSecond(const double InSecond);

	/* Convert seconds to MicroSeconds (1e6) */
	static size_t SecondToMicroSecond(const double InSecond);

	/* Convert seconds to MilliSeconds (1e3) */
	static size_t SecondToMilliSecond(const double InSecond);

	/** @returns time_t with current time. */
	INLINE_DEBUGABLE static time_t GetTime();
	/** @returns time_t with current time. */
	INLINE_DEBUGABLE static struct tm GetTimeInfo();

	/** @returns second of minute */
	INLINE_DEBUGABLE static int GetSecond(struct tm InTime = GetTimeInfo());
	/** @returns minute of hour */
	INLINE_DEBUGABLE static int GetMinute(struct tm InTime = GetTimeInfo());
	/** @returns hour of day */
	INLINE_DEBUGABLE static int GetHour(struct tm InTime = GetTimeInfo());
	/** @returns day of Week */
	INLINE_DEBUGABLE static int GetDayOfWeek(struct tm InTime = GetTimeInfo());
	/** @returns day of month */
	INLINE_DEBUGABLE static int GetDayOfMonth(struct tm InTime = GetTimeInfo());
	/** @returns day of month */
	INLINE_DEBUGABLE static int GetDayOfYear(struct tm InTime = GetTimeInfo());
	/** @returns month of year */
	INLINE_DEBUGABLE static int GetMonth(struct tm InTime = GetTimeInfo());
	/** @returns year */
	INLINE_DEBUGABLE static int GetYear(struct tm InTime = GetTimeInfo());

	/* Starts delay. Uses millisecond (1e3) */
	static void StartDelay(size_t& StartMs);

	/*
	 * Checks and returns true if it's delayed enough.
	 * Delay is raw milliseconds to pass. Do not add StartMs.
	 */
	static bool IsDelayed(const size_t& StartMs, size_t DelayMs);

	/**
	 * @return Percent of delay (Value between 0-1 (clamped))
	 */
	static float GetDelayPercent(const size_t& StartMs, size_t DelayMs);

	/** @returns rawtime */
	INLINE_DEBUGABLE static time_t GetRawTime();

	/* Returns current time in format 12_01_2019_12_37_23. */
	INLINE_DEBUGABLE static std::string GetCurrentTimeNoSpecial();

	/* Returns current time in format 12:01:2019 12:37:23. */
	INLINE_DEBUGABLE static std::string GetCurrentTimeString();

	static std::string GenerateSecureSalt(const size_t Length);

	/**
	 * @brief Flips bits of a uint64_t using XOR with a fixed pattern
	 *
	 * Uses pattern 0xA5A5A5A5A5A5A5A5 (alternating 10100101 bits).
	 * Deterministic and reversible - calling twice returns original value.
	 *
	 * @param InValue Input 64-bit value
	 * @return Value with bits flipped according to pattern
	 */
	static Uint64 FlipBits(const Uint64 InValue, const Uint64 FlipMask);

	/**
	 * @brief Converts a number to any base using a custom character set
	 *
	 * @param InNumber The number to convert
	 * @param InCharSet String containing characters to use (defines the base)
	 * @return String representation in the specified base
	 *
	 * Examples:
	 *   ToBaseN(255, "01") -> "11111111" (binary)
	 *   ToBaseN(255, "01234567") -> "377" (octal)
	 *   ToBaseN(255, "0123456789ABCDEF") -> "FF" (hex)
	 *   ToBaseN(1234, "abc") -> "bbbacb" (base-3 with custom chars)
	 */
	static std::string ToBaseN(uint64_t InNumber, std::string_view InCharSet = CHARACTER_SET_BASE62);

	/**
	 * @brief Converts a string from any base back to a 64-bit unsigned integer
	 *
	 * @param InEncodedString The encoded string to decode
	 * @param InCharSet String containing characters used for encoding (defines the base)
	 * @return Optional containing the decoded number, or std::nullopt if invalid
	 *
	 * Examples:
	 *   FromBaseN("11111111", "01") -> 255 (from binary)
	 *   FromBaseN("377", "01234567") -> 255 (from octal)
	 *   FromBaseN("FF", "0123456789ABCDEF") -> 255 (from hex)
	 */
	static std::optional<uint64_t> FromBaseN(std::string_view InEncodedString, std::string_view InCharSet = CHARACTER_SET_BASE62);

	/** Logs (Info): with white(default) color. */
	static void Info(std::string Message);
	/** Logs (Info): with white(default) color. Print any value (std::ostream). */
	static void Info(std::ostream& Message);

/** Logs (Info): with white(default) color. */
#define LOG_INFO(Message) (FUtil::Info(std::ostringstream().flush() << Message))

	/** Logs (Debug): with log color. LOGS ONLY WHEN IN _DEBUG mode. */
	static void Debug(std::string Message);
	/** Logs (Debug): with log color. LOGS ONLY WHEN IN _DEBUG mode. Print any value (std::ostream). */
	static void Debug(std::ostream& Message);

#if _DEBUG
/** Logs (Debug): with log color. LOGS ONLY WHEN IN _DEBUG mode. */
#define LOG_DEBUG(Message) (FUtil::Debug(std::ostringstream().flush() << Message))
#else
#define LOG_DEBUG(Message)
#endif

	/** Logs (Warn): with yellow color. Has overloads. */
	static void Warn(std::string Message);
	/** Logs (Warn): with yellow color. Print any value (std::ostream). */
	static void Warn(std::ostream& Message);

/** Logs (Warn): with yellow color. Logs with file and line. */
#define LOG_WARN(Message) (FUtil::Warn(std::ostringstream().flush() << Message << std::endl << " in " << __FILE__ << "@" << __LINE__))

	
	/** Logs (Error): with red color */
	static void Error(std::string Message);
	/** Logs (Error): with red color. Print any value (std::ostream). */
	static void Error(std::ostream& Message);
	
/** Logs (Error): with red color. Logs with file and line. */
#define LOG_ERROR(Message) (FUtil::Error(std::ostringstream().flush() << Message << std::endl << " in " << __FILE__ << "@" << __LINE__))

	static bool HasEmptyMessagesQueue();

	/* Print text to console with specified Color.
	 * @param Color see link below.
	 * @see https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c for Color types
	 * This function doesn't log to file. */
	static void PrintToConsole(std::string& Message, const int Color = 7);
	
	template<typename TValue = int>
	static TValue GetRandomValue(TValue Min, TValue Max)
	{
		return ((rand() % Max) + Min);
	}

	template<typename TValue = float>
	static TValue GetRandomValueFloating(TValue Min, TValue Max)
	{
		return Min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max - Min)));
	}

	static FColorRGBA GetRandomColor();
	
	template<typename TValue = int>
	static FVector2D<TValue> GetPointAngle(const TValue Distance, const TValue Angle)
	{
		FVector2D<TValue> Vector;
		
		Vector.X = cos(Angle) * Distance;
		Vector.Y = sin(Angle) * Distance;

		return Vector;
	}

private:
	/** Logging thread */
	static inline SDL_Thread* SDL_Thread = nullptr;

	/** Queue for messages. */
	static inline CQueueSafe<FLogMessage> MessagesQueue;

	/** constant for Milli second */
	static constexpr double FullMilliSecond = 1e3;

	/** constant for Micro second */
	static constexpr double FullMicroSecond = 1e6;

	/** constant for Nano second */
	static constexpr double FullNanoSecond = 1e9;

	/** Array with each month */
	static inline const std::string MonthTable[12] = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12" };

	/** Log directory. Default: "log". */
	static inline const std::string LogDir = "log";

    static inline std::string LogDirRuntime;

	/** Log prefix. Default: "log_". eg: log_13_01_19... */
	static inline const std::string LogPrefix = "log_";

	/** Path to file with log. */
	static inline const std::string LogFilePath = LogDir + "/" + LogPrefix + FUtil::GetCurrentTimeNoSpecial();

	/** Sets whether logging to file is enabled or not with LogInit() function. */
	static inline bool bIsLoggingEnabled = false;

	/** Set to false to stop logging (on thread). */
	static inline bool bKeepLogging = true;

};
