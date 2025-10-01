// Created by Przemysław Wiewióra 2020-2025

#pragma once

#include "CoreMinimal.h"

/**
 * Global utilities class, with:
 * - Converting numbers or string to custom encoding (reversible or not depending on choice)
 * - Simple encryption
 */
class ENGINE_API FEncryptionUtil
{
public:
	// Different XOR masks for bit flipping
	static constexpr uint64_t ALTERNATING_1 = 0xAAAAAAAAAAAAAAAAULL; // 10101010...
	static constexpr uint64_t ALTERNATING_2 = 0x5555555555555555ULL; // 01010101...
	static constexpr uint64_t CHECKERBOARD = 0xA5A5A5A5A5A5A5A5ULL; // 10100101...
	static constexpr uint64_t INVERSE_CHECKER = 0x5A5A5A5A5A5A5AULL;   // 01011010...

	// Nibble-based patterns (4-bit groups)
	static constexpr uint64_t NIBBLE_FLIP = 0xF0F0F0F0F0F0F0F0ULL; // 11110000...
	static constexpr uint64_t NIBBLE_LOW = 0x0F0F0F0F0F0F0F0FULL; // 00001111...

	// Byte-based patterns
	static constexpr uint64_t BYTE_HIGH = 0xFF00FF00FF00FF00ULL; // High nibbles
	static constexpr uint64_t BYTE_LOW = 0x00FF00FF00FF00FFULL; // Low nibbles

	// Mathematical constants
	/** Best are hidden, make your own for safety */
	static constexpr uint64_t PI_BASED = 0x243F6A8885A308D3ULL; // Pi fractional part

	// Prime-based
	static constexpr uint64_t LARGE_PRIME = 0xFFFFFFFFFFFFFFC5ULL; // 2^64 - 59
	static constexpr uint64_t MERSENNE = 0x1FFFFFFFFFFFFFULL;   // 2^61 - 1

	// Sparse patterns
	static constexpr uint64_t EVERY_8TH = 0x0101010101010101ULL; // Bit 0,8,16,24...
	static constexpr uint64_t CORNERS = 0x8000000000000001ULL; // Just first and last

	/** Predefined character sets */
	static constexpr std::string_view PREDEFINED_CHARACTERSET_BINARY = "01";
	static constexpr std::string_view PREDEFINED_CHARACTERSET_OCTAL = "01234567";
	static constexpr std::string_view PREDEFINED_CHARACTERSET_DECIMAL = "0123456789";
	static constexpr std::string_view PREDEFINED_CHARACTERSET_HEX_LOWER = "0123456789abcdef";
	static constexpr std::string_view PREDEFINED_CHARACTERSET_HEX_UPPER = "0123456789ABCDEF";
	static constexpr std::string_view PREDEFINED_CHARACTERSET_BASE36 = "0123456789abcdefghijklmnopqrstuvwxyz"; /** 0-9, a-z */
	static constexpr std::string_view PREDEFINED_CHARACTERSET_BASE62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; /** 0-9, a-z, A-Z */

	static std::string GenerateSecureSalt(const size_t Length);

	/**
	 * @brief Flips bits of a uint64_t using XOR with a fixed pattern
	 *
	 * Uses pattern 0xA5A5A5A5A5A5A5A5 (alternating 10100101 bits).
	 * Deterministic and reversible - calling twice returns original value.
	 *
	 * @param InValue Input 64-bit value
	 * @return Value with bits flipped according to pattern
	 */
	static Uint64 FlipBits(const Uint64 InValue, const Uint64 FlipMask);

	static std::string EncryptDataCustom(const std::string& InDataa, const std::string& InEncryptionKey);
	static std::string DecryptDataCustom(const std::string& InData, const std::string& InEncryptionKey);

	static Uint64 ConvertCharsIntoInt(char InCharArray[8]);
	static std::array<char, 8> ConvertIntIntoChars(Uint64 InData);

	/** Reverse code for ToBaseN */
	static std::string FromBaseN(std::string_view InEncoded, std::string_view InCharSet);

	/** Reverse for ToBaseNNum */
	static uintmax_t FromBaseNNum(std::string_view InEncoded, std::string_view InCharSet);

	/**
	 * @brief Converts a number to any base using a custom character set
	 *
	 * @param InData Put any memory, get expected Charset characters only but size may be different
	 * @param InCharSet String containing characters to use (defines the base)
	 * @return String representation in the specified base
	 */
	static std::string ToBaseN(std::string_view InData, std::string_view InCharSet);

	/** Does not have leading zeros but also does not allow reverse */
	static std::string ToBaseN_Irreversible(std::string_view InData, std::string_view InCharSet);

	/**
	 * @brief Converts a number to any base using a custom character set
	 *
	 * @param InNumber Put number
	 * @param InCharSet String containing characters to use (defines the base)
	 * @return String representation in the specified base
	 *
	 * Examples:
	 *   ToBaseN(255, "01") -> "11111111" (binary)
	 *   ToBaseN(255, "01234567") -> "377" (octal)
	 *   ToBaseN(255, "0123456789ABCDEF") -> "FF" (hex)
	 *   ToBaseN(1234, "abc") -> "bbbacb" (base-3 with custom chars)
	 */
	static std::string ToBaseNNum(uintmax_t InNumber, std::string_view InCharSet);

	/**
	 * @brief Advanced encryption with input validation
	 *
	 * @param InString String to encrypt/decrypt
	 * @param InCharSet Character set defining valid characters
	 * @param InKey Encryption key
	 *	*	MINIMUM:  8 characters  - Basic security (casual obfuscation)
		**	GOOD:     16 characters - Moderate security (recommended minimum)
		**	BETTER:   32 characters - Strong security
		***	BEST:     64+ characters - Maximum security for this algorithm
	 *
	 * @param bEncrypt True to encrypt, false to decrypt
	 * @param bValidateInput If true, throws exception for invalid characters
	 * @return Encrypted/decrypted string
	 * @throws std::invalid_argument If input contains invalid characters (when validation enabled)
	 */
	static std::string EncryptCustomBaseValidated(std::string_view InString, std::string_view InCharSet, std::string_view InKey, bool bEncrypt = true, bool bValidateInput = false);

	static std::string AddPKCS7Padding(const std::string & text);
	static std::string RemovePKCS7Padding(const std::string& text);

	static char NormalizeChar(int32 InChar);
};
