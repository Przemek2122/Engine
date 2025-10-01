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

std::string FEncryptionUtil::EncryptDataCustom(const std::string& InData, const std::string& InEncryptionKey)
{
	std::string Out = InData;

	if (!InEncryptionKey.empty())
	{
		Out = ToBaseN(InData, PREDEFINED_CHARACTERSET_BASE62);

		const std::vector<uint8_t> InputBytes = StringToBytes(Out);
		const std::vector<uint8_t> EncryptionKeyBytes = StringToBytes(InEncryptionKey);
		std::vector<uint8_t> OutBytes;
		OutBytes.reserve(InputBytes.size());

		// First base encryption
		for (int32 InputBytesIndex = 0; InputBytesIndex < static_cast<int32>(InputBytes.size()); InputBytesIndex++)
		{
			const int32 EncryptionBytesIndex = InputBytesIndex % static_cast<int32>(EncryptionKeyBytes.size());

			int32 Result;
			if (InputBytesIndex % 2)
			{
				Result = static_cast<int32>(InputBytes[InputBytesIndex]) + static_cast<int32>(EncryptionKeyBytes[EncryptionBytesIndex]);
			}
			else
			{
				Result = static_cast<int32>(InputBytes[InputBytesIndex]) - static_cast<int32>(EncryptionKeyBytes[EncryptionBytesIndex]);
			}

			OutBytes.push_back(NormalizeByte(Result));
		}

		// Let's do some shuffle
		const std::vector<uint8_t> Shuffled = ShuffleWithKey(OutBytes, EncryptionKeyBytes);

		Out = BytesToString(Shuffled);
	}

	return Out;
}

std::string FEncryptionUtil::DecryptDataCustom(const std::string& InData, const std::string& InEncryptionKey)
{
	std::string Out = InData;

	if (!InEncryptionKey.empty())
	{
		const std::vector<uint8_t> InputBytes = StringToBytes(Out);
		const std::vector<uint8_t> EncryptionKeyBytes = StringToBytes(InEncryptionKey);
		std::vector<uint8_t> OutBytes;
		OutBytes.reserve(InputBytes.size());

		// Let's undo shuffle
		const std::vector<uint8_t> Unshuffled = UnshuffleWithKey(InputBytes, EncryptionKeyBytes);

		// First base encryption
		for (int32 InputBytesIndex = 0; InputBytesIndex < static_cast<int32>(Unshuffled.size()); InputBytesIndex++)
		{
			const int32 EncryptionBytesIndex = InputBytesIndex % static_cast<int32>(EncryptionKeyBytes.size());

			int32 Result;
			if (InputBytesIndex % 2)
			{
				Result = static_cast<int32>(Unshuffled[InputBytesIndex]) - static_cast<int32>(EncryptionKeyBytes[EncryptionBytesIndex]);
			}
			else
			{
				Result = static_cast<int32>(Unshuffled[InputBytesIndex]) + static_cast<int32>(EncryptionKeyBytes[EncryptionBytesIndex]);
			}

			OutBytes.push_back(NormalizeByte(Result));
		}

		const std::string StringConverted = BytesToString(OutBytes);

		Out = FromBaseN(StringConverted, PREDEFINED_CHARACTERSET_BASE62);
	}

	return Out;
}

Uint64 FEncryptionUtil::ConvertCharsIntoInt(char InCharArray[8])
{
	Uint64 Result;
	memcpy(&Result, InCharArray, 8);
	return Result;
}

std::array<char, 8> FEncryptionUtil::ConvertIntIntoChars(const Uint64 InData)
{
	std::array<char, 8> Result;
	memcpy(Result.data(), &InData, 8);
	return Result;
}

std::string FEncryptionUtil::FromBaseN(std::string_view InEncoded, std::string_view InCharSet)
{
	// Validate input
	if (InCharSet.empty() || InEncoded.empty())
		return "";

	const size_t BaseSize = InCharSet.size();

	// Build char->value lookup
	std::unordered_map<char, size_t> CharToDigit;
	for (size_t i = 0; i < BaseSize; ++i)
		CharToDigit[InCharSet[i]] = i;

	// Result as big-endian bytes (most significant first)
	std::vector<uint8_t> Result = { 0 };

	// Process each digit: Result = Result * Base + Digit
	for (char Ch : InEncoded)
	{
		auto It = CharToDigit.find(Ch);
		if (It == CharToDigit.end())
			return ""; // Invalid character

		size_t Digit = It->second;

		// Multiply entire Result by BaseSize
		size_t Carry = 0;
		for (int i = Result.size() - 1; i >= 0; --i) // Right-to-left
		{
			size_t Temp = Result[i] * BaseSize + Carry;
			Result[i] = static_cast<uint8_t>(Temp & 0xFF);
			Carry = Temp >> 8;
		}
		while (Carry > 0)
		{
			Result.insert(Result.begin(), static_cast<uint8_t>(Carry & 0xFF));
			Carry >>= 8;
		}

		// Add Digit to Result
		Carry = Digit;
		for (int i = Result.size() - 1; i >= 0 && Carry > 0; --i)
		{
			size_t Temp = Result[i] + Carry;
			Result[i] = static_cast<uint8_t>(Temp & 0xFF);
			Carry = Temp >> 8;
		}
		while (Carry > 0)
		{
			Result.insert(Result.begin(), static_cast<uint8_t>(Carry & 0xFF));
			Carry >>= 8;
		}
	}

	return std::string(reinterpret_cast<const char*>(Result.data()), Result.size());
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

std::vector<uint8_t> FEncryptionUtil::ShuffleWithKey(const std::vector<uint8_t>& Input, const std::vector<uint8_t>& Key)
{
	std::vector<uint8_t> Output = Input;

	// Generate seed
	uint64_t Seed = 0;
	for (size_t i = 0; i < Key.size(); i++) {
		Seed ^= static_cast<uint64_t>(Key[i]) << ((i % 8) * 8);
	}

	// Shuffle
	std::mt19937_64 Generator(Seed);
	std::shuffle(Output.begin(), Output.end(), Generator);

	return Output;
}

std::vector<uint8_t> FEncryptionUtil::UnshuffleWithKey(const std::vector<uint8_t>& Shuffled,
	const std::vector<uint8_t>& Key)
{
	// Generate same permutation
	uint64_t Seed = 0;
	for (size_t i = 0; i < Key.size(); i++) {
		Seed ^= static_cast<uint64_t>(Key[i]) << ((i % 8) * 8);
	}

	std::mt19937_64 Generator(Seed);

	std::vector<size_t> Permutation(Shuffled.size());
	std::iota(Permutation.begin(), Permutation.end(), 0);
	std::shuffle(Permutation.begin(), Permutation.end(), Generator);

	// Reverse shuffle
	std::vector<uint8_t> Output(Shuffled.size());
	for (size_t i = 0; i < Shuffled.size(); i++) {
		Output[Permutation[i]] = Shuffled[i];
	}

	return Output;
}

std::vector<uint8_t> FEncryptionUtil::StringToBytes(const std::string& Str)
{
	return { Str.begin(), Str.end() };
}

std::string FEncryptionUtil::BytesToString(const std::vector<uint8_t>& Bytes)
{
	return { Bytes.begin(), Bytes.end() };
}

Uint8 FEncryptionUtil::NormalizeByte(int32 InChar)
{
	if (InChar > UINT8_MAX)
	{
		InChar = (InChar - UINT8_MAX);
	}

	if (InChar < 0)
	{
		InChar = (InChar + UINT8_MAX);
	}

	return static_cast<Uint8>(InChar);
}
