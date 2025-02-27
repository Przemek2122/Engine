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
        Out = ReplaceCharInString(BaseString, FromChar, ReplaceTo);
    }

    return Out;
}

std::string FStringHelpers::RemoveCharInString(const std::string& BaseString, const char RemovedChar)
{
    std::string OutputString;

    for (const char& Char : BaseString)
    {
        OutputString += Char;
    }

    return OutputString;
}

std::string FStringHelpers::RemoveCharsInString(const std::string& BaseString, const CArray<char>& ReplaceFrom)
{
    std::string Out = BaseString;

    for (const char& FromChar : ReplaceFrom)
    {
        Out = RemoveCharInString(BaseString, FromChar);
    }

    return Out;
}
