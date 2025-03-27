// Created by Przemys�aw Wiewi�ra 2024

#include "CoreEngine.h"
#include "Misc/StringHelpers.h"

bool FStringHelpers::CompareStringCaseInsensitive(const std::string& A, const std::string& B)
{
    return A.size() == B.size() && std::equal(A.begin(), A.end(), B.begin(), CompareCharsCaseInsensitive);
}

bool FStringHelpers::CompareCharsCaseInsensitive(const char A, const char B)
{
    return ( std::tolower(static_cast<unsigned char>(A)) == std::tolower(static_cast<unsigned char>(B)) );
}

bool FStringHelpers::ContainsChar(const std::string& String, const char SearchFor)
{
	bool bContainsChar = false;

    for (const char& Char : String)
    {
        if (Char == SearchFor)
        {
			bContainsChar = true;

			break;
        }
    }

    return bContainsChar;
}

bool FStringHelpers::ToBoolValue(const std::string& String)
{
    static std::string BoolTrueStringValue = "true";

    bool bIsTrue = false;

    if (CompareStringCaseInsensitive(String, BoolTrueStringValue))
    {
        bIsTrue = true;
    }

    return bIsTrue;
}

std::string FStringHelpers::ReplaceCharInString(const std::string& BaseString, const char ReplaceFrom, const char ReplaceTo)
{
    std::string OutputString;

    for (const char& Char : BaseString)
    {
	    if (Char == ReplaceFrom)
	    {
            OutputString += ReplaceTo;
	    }
        else
        {
            OutputString += Char;
        }
    }

    return OutputString;
}

std::string FStringHelpers::ReplaceCharsInString(const std::string& BaseString, const CArray<char>& ReplaceFrom, const char ReplaceTo)
{
    std::string Out = BaseString;

    for (const char& FromChar : ReplaceFrom)
    {
        Out = ReplaceCharInString(Out, FromChar, ReplaceTo);
    }

    return Out;
}

std::string FStringHelpers::RemoveCharInString(const std::string& BaseString, const char RemovedChar)
{
    std::string OutputString;

    for (const char& Char : BaseString)
    {
        if (Char != RemovedChar)
        {
            OutputString += Char;
        }
    }

    return OutputString;
}

std::string FStringHelpers::RemoveCharsInString(const std::string& BaseString, const CArray<char>& ReplaceFrom)
{
    std::string Out = BaseString;

    for (const char& FromChar : ReplaceFrom)
    {
        Out = RemoveCharInString(Out, FromChar);
    }

    return Out;
}

CArray<std::string> FStringHelpers::SplitString(const std::string& BaseString, const char Delimiter)
{
	CArray<std::string> OutArray;
	std::string CurrentString;

	for (const char& Char : BaseString)
	{
		if (Char == Delimiter)
		{
			OutArray.Push(CurrentString);
			CurrentString.clear();
		}
		else
		{
			CurrentString += Char;
		}
	}

	if (!CurrentString.empty())
	{
		OutArray.Push(CurrentString);
	}

	return OutArray;
}
