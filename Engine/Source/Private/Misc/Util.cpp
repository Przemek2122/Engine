// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/Util.h"
#include "Misc/FileSystem.h"
#include <numbers>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

static const std::string ThreadName = "Log";

void FUtil::LogInit(bool EnableLogging)
{
	if (EnableLogging)
	{
#if PLATFORM_ANDROID
		// @TODO: This should be change to proper organization and app name
		char* PathPtr = SDL_GetPrefPath("Org", "App");

        LogDirRuntime = PathPtr + LogDir;

		SDL_free(PathPtr);
#else
        LogDirRuntime = LogDir;
#endif

		FFileSystem::Directory::Create(LogDirRuntime);

		bIsLoggingEnabled = true;

		std::cout << "LogInit - Logging to: " << LogFilePath << std::endl;
	}
	else
	{
		bIsLoggingEnabled = false;
	}

	SDL_Thread = SDL_CreateThread(MessagesPrinter, ThreadName.c_str(), nullptr);
}

void FUtil::EndLogging()
{
	bKeepLogging = false;

	SDL_DetachThread(SDL_Thread);
}

int FUtil::MessagesPrinter(void* ptr)
{
	while (bKeepLogging)
	{
		if (MessagesQueue.Size() == 0)
		{
			SDL_Delay(2);
		}
		else
		{
			FLogMessage& Message = MessagesQueue.PeekFirst();

			switch (Message.Type)
			{
			case ELogMessageType::Message_Info:
				PrintToConsole(Message.Text, 7);

				break;

			case ELogMessageType::Message_Debug:
#ifdef _DEBUG
				PrintToConsole(Message.Text, 5);
#endif // _DEBUG
				break;

			case ELogMessageType::Message_Warning:
				PrintToConsole(Message.Text, 6);

				break;

			case ELogMessageType::Message_Error:
				PrintToConsole(Message.Text, 4);

				break;

			default:
#ifdef _DEBUG
	ENSURE_VALID(false);
#endif
				break;
			}

			if (bIsLoggingEnabled)
			{
				// @TODO Either append or keep it open

				std::ofstream LogFile;
				LogFile.open(LogFilePath, std::ios_base::app);
				LogFile << Message.Text << "\n";
				LogFile.close();
			}

			// Remove item
			MessagesQueue.DequeFrontSafe();
		}
	}

	return 0;
}

size_t FUtil::GetNanoSeconds()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

size_t FUtil::GetMircoSeconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

/* Return current Milisecond */
size_t FUtil::GetMiliSeconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

size_t FUtil::GetSeconds()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double FUtil::NanoSecondToSecond(const size_t InNanosecond)
{
	return (static_cast<double>(InNanosecond) / static_cast<double>(FullNanoSecond));
}

double FUtil::MicroSecondToSecond(const size_t InNanosecond)
{
	return (static_cast<double>(InNanosecond) / static_cast<double>(FullMicroSecond));
}

double FUtil::MilliSecondToSecond(const size_t InNanosecond)
{
	return (static_cast<double>(InNanosecond) / static_cast<double>(FullMilliSecond));
}

size_t FUtil::SecondToNanoSecond(const double InSecond)
{
	return static_cast<size_t>(InSecond * static_cast<double>(FullNanoSecond));
}

size_t FUtil::SecondToMicroSecond(const double InSecond)
{
	return static_cast<size_t>(InSecond * static_cast<double>(FullMicroSecond));
}

size_t FUtil::SecondToMilliSecond(const double InSecond)
{
	return static_cast<size_t>(InSecond * static_cast<double>(FullMilliSecond));
}

time_t FUtil::GetTime()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

int FUtil::GetSecond(struct tm InTime)
{
	return InTime.tm_sec;
}

int FUtil::GetMinute(struct tm InTime)
{
	return InTime.tm_min;
}

int FUtil::GetHour(struct tm InTime)
{
	return InTime.tm_hour;
}

int FUtil::GetDayOfWeek(struct tm InTime)
{
	return InTime.tm_wday;
}

int FUtil::GetDayOfMonth(struct tm InTime)
{
	return InTime.tm_mday;
}

int FUtil::GetDayOfYear(struct tm InTime)
{
	return InTime.tm_yday;
}

int FUtil::GetMonth(struct tm InTime)
{
	return 0;
}

int FUtil::GetYear(struct tm InTime)
{
	return 0;
}

void FUtil::StartDelay(size_t& StartMs)
{
	StartMs = GetMiliSeconds();
}

bool FUtil::IsDelayed(const size_t& StartMs, const size_t DelayMs)
{
	return static_cast<size_t>(GetMiliSeconds()) >= StartMs + DelayMs;
}

float FUtil::GetDelayPercent(const size_t& StartMs, const size_t DelayMs)
{
	const size_t CurrentTime = GetMiliSeconds();

	// Double in case we wait for eternity, probably could be float
	const float PercentOfDelayPassed = static_cast<float>(static_cast<double>(CurrentTime - StartMs) / static_cast<double>(DelayMs));

	return FMath::Clamp(PercentOfDelayPassed, 0.f, 1.f);
}

time_t FUtil::GetRawTime()
{
	time_t rawtime;
	time(&rawtime);

	return rawtime;
}

struct tm FUtil::GetTimeInfo()
{
	time_t InTime;
	time(&InTime);
	struct tm OutTime;

#if PLATFORM_WINDOWS
	localtime_s(&OutTime, &InTime);
#elif PLATFORM_ANDROID
	localtime_r(&InTime, &OutTime);
#else
	OutTime = *localtime(&InTime);
#endif

	return OutTime;
}

std::string FUtil::GetCurrentTimeNoSpecial()
{
	const tm TimeInfo = GetTimeInfo();

	std::string ctm = std::to_string(TimeInfo.tm_mday) + "_" + MonthTable[TimeInfo.tm_mon] + "_" + std::to_string(TimeInfo.tm_year + 1900) + "_"
		+ std::to_string(TimeInfo.tm_hour) + "_" + std::to_string(TimeInfo.tm_min) + "_" + std::to_string(TimeInfo.tm_sec);

	return ctm;
}

std::string FUtil::GetCurrentTimeString()
{
	const tm TimeInfo = GetTimeInfo();

	std::string ctm = std::to_string(TimeInfo.tm_mday) + "-" + MonthTable[TimeInfo.tm_mon] + "-" + std::to_string(TimeInfo.tm_year + 1900) + " "
		+ std::to_string(TimeInfo.tm_hour) + ":" + std::to_string(TimeInfo.tm_min) + ":" + std::to_string(TimeInfo.tm_sec);

	return ctm;
}

std::string FUtil::GenerateSecureSalt(const size_t Length)
{
	std::random_device rd;  // Hardware entropy
	std::mt19937 gen(rd()); // Seed with hardware randomness
	std::uniform_int_distribution<int> dis(0, UINT8_MAX);

	std::string salt(Length, '\0');
	for (size_t i = 0; i < Length; ++i) {
		salt[i] = static_cast<char>(dis(gen));
	}

	return salt;
}

Uint64 FUtil::FlipBits(uint64_t InValue, const Uint64 FlipMask)
{
	// XOR mask: alternating bit pattern for reproducible flipping
	return InValue ^ FlipMask;
}

std::string FUtil::ToBaseN(uint64_t InNumber, std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty())
	{
		return ""; // Can't convert without characters
	}

	const size_t BaseSize = InCharSet.size();

	// Handle zero case explicitly
	if (InNumber == 0)
	{
		return std::string(1, InCharSet[0]); // Return first character for zero
	}

	// Calculate max possible length for this base to reserve space
	// Formula: ceil(log_base(2^64)) = ceil(64 * log(2) / log(base))
	const size_t CalculatedMaxLength = static_cast<size_t>(
		std::ceil(64.0 * std::numbers::ln2 / std::log(static_cast<double>(BaseSize))) + 1
	);

	std::string Result;
	Result.reserve(CalculatedMaxLength);

	// Convert number by repeatedly dividing by base
	while (InNumber > 0)
	{
		Result += InCharSet[InNumber % BaseSize];  // Get remainder as next digit
		InNumber /= BaseSize;                      // Move to next position
	}

	// Reverse to get correct digit order (most significant first)
	std::ranges::reverse(Result);

	return Result;
}

std::optional<uint64_t> FUtil::FromBaseN(std::string_view InEncodedString, std::string_view InCharSet)
{
	// Validate inputs
	if (InCharSet.empty() || InEncodedString.empty())
	{
		return std::nullopt;
	}

	const size_t BaseSize = InCharSet.size();

	// Create lookup table for character to digit conversion
	std::unordered_map<char, size_t> CharToDigit;
	CharToDigit.reserve(BaseSize);

	for (size_t i = 0; i < BaseSize; ++i)
	{
		CharToDigit[InCharSet[i]] = i;
	}

	uint64_t Result = 0;
	uint64_t Multiplier = 1;

	// Process string from right to left (least significant digit first)
	for (auto it = InEncodedString.rbegin(); it != InEncodedString.rend(); ++it)
	{
		char CurrentChar = *it;

		// Check if character exists in character set
		auto DigitIt = CharToDigit.find(CurrentChar);
		if (DigitIt == CharToDigit.end())
		{
			return std::nullopt; // Invalid character found
		}

		size_t DigitValue = DigitIt->second;

		// Check for overflow before multiplication
		if (Multiplier > (UINT64_MAX - Result) / BaseSize)
		{
			return std::nullopt; // Would cause overflow
		}

		// Add digit contribution to result
		uint64_t Addition = DigitValue * Multiplier;
		if (Addition > UINT64_MAX - Result)
		{
			return std::nullopt; // Would cause overflow
		}

		Result += Addition;

		// Update multiplier for next digit
		if (Multiplier > UINT64_MAX / BaseSize)
		{
			// Check if we're at the last character to avoid false overflow detection
			if (std::next(it) != InEncodedString.rend())
			{
				return std::nullopt; // Would cause overflow on next iteration
			}
		}
		else
		{
			Multiplier *= BaseSize;
		}
	}

	return Result;
}

void FUtil::Info(std::string Message)
{
	Message = GetCurrentTimeString() + " (Info): " + Message;

	MessagesQueue.PushBackSafe({ ELogMessageType::Message_Info, Message });
}

void FUtil::Info(std::ostream& Message)
{
	Info(dynamic_cast<std::ostringstream&>(Message).str());
}

void FUtil::Debug(std::string Message)
{
#ifdef _DEBUG
	Message = GetCurrentTimeString() + " (Debug): " + Message;

	MessagesQueue.PushBackSafe({ ELogMessageType::Message_Debug, Message });
#endif
}

void FUtil::Debug(std::ostream& Message)
{
	Debug(dynamic_cast<std::ostringstream&>(Message).str());
}

void FUtil::Warn(std::string Message)
{
	Message = GetCurrentTimeString() + " (Warn): " + Message;

	MessagesQueue.PushBackSafe({ ELogMessageType::Message_Warning, Message });
}

void FUtil::Warn(std::ostream& Message)
{
	Warn(dynamic_cast<std::ostringstream&>(Message).str());
}

void FUtil::Error(std::string Message)
{
	Message = GetCurrentTimeString() + " (Error): " + Message;

	MessagesQueue.PushBackSafe({ ELogMessageType::Message_Error, Message });
}

void FUtil::Error(std::ostream& Message)
{
	Error(dynamic_cast<std::ostringstream&>(Message).str());
}

bool FUtil::HasEmptyMessagesQueue()
{
	return MessagesQueue.IsEmpty();
}

void FUtil::PrintToConsole(std::string& Message, const int Color)
{
#if PLATFORM_WINDOWS
	// Change color to red
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // text color
	SetConsoleTextAttribute(hConsole, Color);
#endif

#if PLATFORM_ANDROID
    SDL_Log("%s", Message.c_str());
#else
	std::cout << Message << std::endl;
#endif

#if PLATFORM_WINDOWS
	// Change color back to white
	SetConsoleTextAttribute(hConsole, 7);
#endif
}

FColorRGBA FUtil::GetRandomColor()
{
	return FColorRGBA(
		GetRandomValue(0, 255),
		GetRandomValue(0, 255),
		GetRandomValue(0, 255),
		255
	);
}
