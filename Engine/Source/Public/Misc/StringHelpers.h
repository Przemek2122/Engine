// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class FStringHelpers
{
public:
	static bool CompareStringCaseInsensitive(const std::string& A, const std::string& B);
	static bool CompareCharsCaseInsensitive(char A, char B);

	static bool ToBoolValue(const std::string& String);

	static std::string ReplaceCharInString(const std::string& BaseString, const char ReplaceFrom, const char ReplaceTo);

};
