// Created by https://www.linkedin.com/in/przemek2122/ 2024

#pragma once

#include "CoreMinimal.h"

class ENGINE_API FStringHelpers
{
public:
	static bool CompareStringCaseInsensitive(const std::string& A, const std::string& B);
	static bool CompareCharsCaseInsensitive(char A, char B);

	static bool ContainsChar(const std::string& String, const char SearchFor);

	static bool ToBoolValue(const std::string& String);

	static std::string ReplaceCharInString(const std::string& BaseString, const char ReplaceFrom, const char ReplaceTo);
	static std::string ReplaceCharsInString(const std::string& BaseString, const CArray<char>& ReplaceFrom, const char ReplaceTo);

	static std::string RemoveCharInString(const std::string& BaseString, const char RemovedChar);
	static std::string RemoveCharsInString(const std::string& BaseString, const CArray<char>& ReplaceFrom);

	static CArray<std::string> SplitString(const std::string& BaseString, const char Delimiter);

};
