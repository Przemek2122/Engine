// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Misc/Util.h"
#include "Misc/FileSystem.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

namespace FUtil
{
	bool isLoggingEnabled = false;
	std::string LogDir = "log";
	std::string LogPrefix = "log_";
	std::string LogFilePath = LogDir + "/" + LogPrefix + GetCurrentTimeNoSpecial();

	bool KeepLogging = true;
	SDL_Thread* LogThread;
	CQueueSafe<FLogMessage> MessagesQueue;

	static const std::string MonthTable[12] = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12" };

	void LogInit(bool EnableLogging)
	{
		if (EnableLogging)
		{
			FFileSystem::Directory::Create(LogDir);

			isLoggingEnabled = true;

			std::cout << "LogInit - Logging to: " << LogFilePath << std::endl;
		}
		else
		{
			isLoggingEnabled = false;
		}

		SDL_CreateThread(MessagesPrinter, "Log", nullptr);
	}

	static int MessagesPrinter(void* ptr)
	{
		while (KeepLogging)
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

				if (isLoggingEnabled)
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

	size_t GetNanoSeconds()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	size_t GetMircoSeconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	/* Return current Milisecond */
	size_t GetMiliSeconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	size_t GetSeconds()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	double NanoSecondToSecond(const size_t InNanosecond)
	{
		return (static_cast<double>(InNanosecond) / static_cast<double>(FullNanoSecond));
	}

	double MicroSecondToSecond(const size_t InNanosecond)
	{
		return (static_cast<double>(InNanosecond) / static_cast<double>(FullMicroSecond));
	}

	double MilliSecondToSecond(const size_t InNanosecond)
	{
		return (static_cast<double>(InNanosecond) / static_cast<double>(FullMilliSecond));
	}

	size_t SecondToNanoSecond(const double InSecond)
	{
		return static_cast<size_t>(InSecond * static_cast<double>(FullNanoSecond));
	}

	size_t SecondToMicroSecond(const double InSecond)
	{
		return static_cast<size_t>(InSecond * static_cast<double>(FullMicroSecond));
	}

	size_t SecondToMilliSecond(const double InSecond)
	{
		return static_cast<size_t>(InSecond * static_cast<double>(FullMilliSecond));
	}

	time_t GetTime()
	{
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}

	int GetSecond(struct tm InTime)
	{
		return InTime.tm_sec;
	}

	int GetMinute(struct tm InTime)
	{
		return InTime.tm_min;
	}

	int GetHour(struct tm InTime)
	{
		return InTime.tm_hour;
	}

	int GetDayOfWeek(struct tm InTime)
	{
		return InTime.tm_wday;
	}

	int GetDayOfMonth(struct tm InTime)
	{
		return InTime.tm_mday;
	}

	int GetDayOfYear(struct tm InTime)
	{
		return InTime.tm_yday;
	}

	int GetMonth(struct tm InTime)
	{
		return 0;
	}

	int GetYear(struct tm InTime)
	{
		return 0;
	}

	void StartDelay(size_t& StartMs)
	{
		StartMs = GetMiliSeconds();
	}

	bool IsDelayed(const size_t& StartMs, const size_t DelayMs)
	{
		return static_cast<size_t>(GetMiliSeconds()) >= StartMs + DelayMs;
	}

	float GetDelayPercent(const size_t& StartMs, const size_t DelayMs)
	{
		const size_t CurrentTime = GetMiliSeconds();

		// Double in case we wait for eternity, probably could be float
		const float PercentOfDelayPassed = static_cast<float>(static_cast<double>(CurrentTime - StartMs) / static_cast<double>(DelayMs));

		return FMath::Clamp(PercentOfDelayPassed, 0.f, 1.f);
	}

	time_t GetRawTime()
	{
		time_t rawtime;
		time(&rawtime);

		return rawtime;
	}

	struct tm GetTimeInfo()
	{
		time_t rawtime;
		time(&rawtime);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);

		return timeinfo;
	}

	std::string GetCurrentTimeNoSpecial()
	{
		time_t RawTime64;
		time(&RawTime64);
		tm TimeInfo;
		localtime_s(&TimeInfo, &RawTime64);

		std::string ctm = std::to_string(TimeInfo.tm_mday) + "_" + MonthTable[TimeInfo.tm_mon] + "_" + std::to_string(TimeInfo.tm_year + 1900) + "_"
			+ std::to_string(TimeInfo.tm_hour) + "_" + std::to_string(TimeInfo.tm_min) + "_" + std::to_string(TimeInfo.tm_sec);

		return ctm;
	}

	std::string GetCurrentTime()
	{
		time_t RawTime64;
		time(&RawTime64);
		tm TimeInfo;
		localtime_s(&TimeInfo, &RawTime64);

		std::string ctm = std::to_string(TimeInfo.tm_mday) + "-" + MonthTable[TimeInfo.tm_mon] + "-" + std::to_string(TimeInfo.tm_year + 1900) + " "
			+ std::to_string(TimeInfo.tm_hour) + ":" + std::to_string(TimeInfo.tm_min) + ":" + std::to_string(TimeInfo.tm_sec);

		return ctm;
	}

	void Info(std::string Message)
	{
		Message = GetCurrentTime() + " (Info): " + Message;

		MessagesQueue.PushBackSafe({ ELogMessageType::Message_Info, Message });
	}

	void Info(std::ostream& Message)
	{
		Info(dynamic_cast<std::ostringstream&>(Message).str());
	}

	void Debug(std::string Message)
	{
#ifdef _DEBUG
		Message = GetCurrentTime() + " (Debug): " + Message;

		MessagesQueue.PushBackSafe({ ELogMessageType::Message_Debug, Message });
#endif
	}

	void Debug(std::ostream& Message)
	{
		Debug(dynamic_cast<std::ostringstream&>(Message).str());
	}

	void Warn(std::string Message)
	{
		Message = GetCurrentTime() + " (Warn): " + Message;

		MessagesQueue.PushBackSafe({ ELogMessageType::Message_Warning, Message });
	}

	void Warn(std::ostream& Message)
	{
		Warn(dynamic_cast<std::ostringstream&>(Message).str());
	}

	void Error(std::string Message)
	{
		Message = GetCurrentTime() + " (Error): " + Message;

		MessagesQueue.PushBackSafe({ ELogMessageType::Message_Error, Message });
	}

	void Error(std::ostream& Message)
	{
		Error(dynamic_cast<std::ostringstream&>(Message).str());
	}

	void PrintToConsole(std::string& Message, const int Color)
	{
#if defined(_WIN32) || defined(_WIN64)
		// Change color to red
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // text color
		SetConsoleTextAttribute(hConsole, Color);
#endif

		std::cout << Message << std::endl;

#if defined(_WIN32) || defined(_WIN64)
		// Change color back to white
		SetConsoleTextAttribute(hConsole, 7);
#endif
	}

	FColorRGBA GetRandomColor()
	{
		return FColorRGBA(
			GetRandomValue(0, 255),
			GetRandomValue(0, 255),
			GetRandomValue(0, 255),
			255
		);
	}
}

