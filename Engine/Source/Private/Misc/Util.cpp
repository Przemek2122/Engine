// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/Util.h"
#include "Misc/FileSystem.h"

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
