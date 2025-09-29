// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/Util.h"
#include "Misc/FileSystem.h"
#include <numbers>
#include <unordered_set>

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

std::string FUtil::FromBaseN(std::string_view InEncoded, std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty() || InEncoded.empty())
	{
		return "";
	}

	const size_t BaseSize = InCharSet.size();

	// Create a lookup map for character to digit value
	std::unordered_map<char, size_t> CharToDigit;
	for (size_t i = 0; i < BaseSize; ++i)
	{
		CharToDigit[InCharSet[i]] = i;
	}

	// Convert encoded string to digit array
	std::vector<size_t> Digits;
	Digits.reserve(InEncoded.size());
	for (char c : InEncoded)
	{
		auto it = CharToDigit.find(c);
		if (it == CharToDigit.end())
		{
			return ""; // Invalid character
		}
		Digits.push_back(it->second);
	}

	// Convert digits from target base to bytes (base 256)
	std::vector<unsigned char> Bytes;

	for (size_t digit : Digits)
	{
		// Multiply current bytes by BaseSize and add digit
		size_t Carry = digit;
		for (unsigned char& byte : Bytes)
		{
			size_t Temp = byte * BaseSize + Carry;
			byte = static_cast<unsigned char>(Temp % 256);
			Carry = Temp / 256;
		}

		// Add new bytes if needed
		while (Carry > 0)
		{
			Bytes.push_back(static_cast<unsigned char>(Carry % 256));
			Carry /= 256;
		}
	}

	// Reverse bytes to get correct order
	std::ranges::reverse(Bytes);

	// Convert bytes to string
	return std::string(reinterpret_cast<const char*>(Bytes.data()), Bytes.size());
}

std::string FUtil::ToBaseN(const std::string_view InData, const std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty())
	{
		return ""; // Can't convert without characters
	}
	const size_t BaseSize = InCharSet.size();

	// Get raw bytes
	const unsigned char* Bytes = reinterpret_cast<const unsigned char*>(InData.data());
	const size_t ByteCount = InData.size();

	// Handle empty input
	if (ByteCount == 0)
	{
		return std::string(1, InCharSet[0]);
	}

	// Convert bytes to digits in target base
	std::vector<size_t> Digits = { 0 };

	for (size_t i = 0; i < ByteCount; ++i)
	{
		// Multiply current number by 256 and add next byte
		size_t Carry = Bytes[i];
		for (size_t& Digit : Digits)
		{
			size_t Temp = Digit * 256 + Carry;
			Digit = Temp % BaseSize;
			Carry = Temp / BaseSize;
		}

		// Add new digits if needed
		while (Carry > 0)
		{
			Digits.push_back(Carry % BaseSize);
			Carry /= BaseSize;
		}
	}

	// Calculate minimum length to preserve all data
	const size_t MinDigitsPerByte = static_cast<size_t>(
		std::ceil(8.0 * std::numbers::ln2 / std::log(static_cast<double>(BaseSize)))
	);
	const size_t MinOutputLength = ByteCount * MinDigitsPerByte;

	// Pad with leading zeros to ensure all data is represented
	while (Digits.size() < MinOutputLength)
	{
		Digits.push_back(0);
	}

	// Build result string (digits are in reverse order)
	std::string Result;
	Result.reserve(Digits.size());
	for (auto it = Digits.rbegin(); it != Digits.rend(); ++it)
	{
		Result += InCharSet[*it];
	}

	return Result;
}

std::string FUtil::ToBaseN_Irreversible(const std::string_view InData, const std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty())
	{
		return ""; // Can't convert without characters
	}
	const size_t BaseSize = InCharSet.size();

	// Get raw bytes
	const unsigned char* Bytes = reinterpret_cast<const unsigned char*>(InData.data());
	const size_t ByteCount = InData.size();

	// Handle empty input
	if (ByteCount == 0)
	{
		return std::string(1, InCharSet[0]);
	}

	// Convert bytes to digits in target base
	std::vector<size_t> Digits = { 0 };

	for (size_t i = 0; i < ByteCount; ++i)
	{
		// Multiply current number by 256 and add next byte
		size_t Carry = Bytes[i];
		for (size_t& Digit : Digits)
		{
			size_t Temp = Digit * 256 + Carry;
			Digit = Temp % BaseSize;
			Carry = Temp / BaseSize;
		}

		// Add new digits if needed
		while (Carry > 0)
		{
			Digits.push_back(Carry % BaseSize);
			Carry /= BaseSize;
		}
	}

	// Handle all-zero case
	if (Digits.size() == 1 && Digits[0] == 0)
	{
		return std::string(1, InCharSet[0]);
	}

	// Build result string (digits are in reverse order)
	std::string Result;
	Result.reserve(Digits.size());
	for (auto it = Digits.rbegin(); it != Digits.rend(); ++it)
	{
		Result += InCharSet[*it];
	}

	return Result;
}

std::string FUtil::ToBaseNNum(uintmax_t InNumber, const std::string_view InCharSet)
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
	// uintmax_t is at least 64 bits
	const size_t CalculatedMaxLength = static_cast<size_t>(
		std::ceil(sizeof(uintmax_t) * 8.0 * std::numbers::ln2 / std::log(static_cast<double>(BaseSize))) + 1
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

uintmax_t FUtil::FromBaseNNum(const std::string_view InEncoded, const std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty() || InEncoded.empty())
	{
		return 0;
	}

	const size_t BaseSize = InCharSet.size();

	// Create a lookup map for character to digit value
	std::unordered_map<char, size_t> CharToDigit;
	for (size_t i = 0; i < BaseSize; ++i)
	{
		CharToDigit[InCharSet[i]] = i;
	}

	// Convert from base-N to number
	uintmax_t Result = 0;

	for (char c : InEncoded)
	{
		auto it = CharToDigit.find(c);
		if (it == CharToDigit.end())
		{
			return 0; // Invalid character, return 0
		}

		// Multiply by base and add digit
		Result = Result * BaseSize + it->second;
	}

	return Result;
}

std::string FUtil::EncryptCustomBaseValidated(const std::string_view InString, const std::string_view InCharSet, const std::string_view InKey, bool bEncrypt, bool bValidateInput)
{
	if (InString.empty() || InCharSet.empty() || InKey.empty())
	{
		return std::string(InString);
	}

	// Optional input validation
	if (bValidateInput)
	{
		std::unordered_set<char> ValidChars(InCharSet.begin(), InCharSet.end());

		for (char c : InString)
		{
			if (ValidChars.find(c) == ValidChars.end())
			{
				throw std::invalid_argument("Input string contains invalid character: '" +
					std::string(1, c) + "'");
			}
		}
	}

	// Rest of the function is the same as EncryptCustomBase...
	const size_t CharSetSize = InCharSet.size();

	std::vector<char> ShuffledChars(InCharSet.begin(), InCharSet.end());

	uint64_t Seed = 0;
	for (char c : InKey)
	{
		Seed = Seed * 31 + static_cast<uint64_t>(c);
	}

	for (size_t i = CharSetSize - 1; i > 0; --i)
	{
		Seed = Seed * 1103515245 + 12345;
		size_t j = Seed % (i + 1);
		std::swap(ShuffledChars[i], ShuffledChars[j]);
	}

	std::unordered_map<char, char> EncryptMap;
	std::unordered_map<char, char> DecryptMap;

	for (size_t i = 0; i < CharSetSize; ++i)
	{
		EncryptMap[InCharSet[i]] = ShuffledChars[i];
		DecryptMap[ShuffledChars[i]] = InCharSet[i];
	}

	std::string Result;
	Result.reserve(InString.size());

	const auto& UseMap = bEncrypt ? EncryptMap : DecryptMap;

	for (char c : InString)
	{
		auto it = UseMap.find(c);
		Result += (it != UseMap.end()) ? it->second : c;
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
