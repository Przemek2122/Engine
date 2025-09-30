// Created by Przemysław Wiewióra 2020-2025

#include "CoreEngine.h"
#include "Misc/EncryptionUtil.h"
#include <unordered_set>

std::string FEncryptionUtil::GenerateSecureSalt(const size_t Length)
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

Uint64 FEncryptionUtil::FlipBits(uint64_t InValue, const Uint64 FlipMask)
{
	// XOR mask: alternating bit pattern for reproducible flipping
	return InValue ^ FlipMask;
}

std::string FEncryptionUtil::FromBaseN(std::string_view InEncoded, std::string_view InCharSet)
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

std::string FEncryptionUtil::ToBaseN(const std::string_view InData, const std::string_view InCharSet)
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

std::string FEncryptionUtil::ToBaseN_Irreversible(const std::string_view InData, const std::string_view InCharSet)
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

std::string FEncryptionUtil::ToBaseNNum(uintmax_t InNumber, const std::string_view InCharSet)
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

uintmax_t FEncryptionUtil::FromBaseNNum(const std::string_view InEncoded, const std::string_view InCharSet)
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

std::string FEncryptionUtil::EncryptCustomBaseValidated(const std::string_view InString, const std::string_view InCharSet, const std::string_view InKey, bool bEncrypt, bool bValidateInput)
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
