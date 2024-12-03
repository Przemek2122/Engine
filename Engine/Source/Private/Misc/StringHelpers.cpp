// Created by Przemys³aw Wiewióra 2024

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
