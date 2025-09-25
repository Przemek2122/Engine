// Created by Przemys³aw Wiewióra 2020-2025

#include "CoreEngine.h"
#include "Misc/PasswordEncryptionArgon.h"
#include "argon2.h"

std::string FPasswordEncryptionArgon::HashPassword(const std::string& InputString)
{
	// Argon settings
	const uint32_t t_cost = 3;      // 3 iterations
	const uint32_t m_cost = 65536;  // 64 MB
	const uint32_t parallelism = 1;
	const uint32_t hash_len = 32;
	const uint32_t salt_len = 16;

	std::string salt = GenerateSecureSalt(salt_len);

	const size_t encoded_len = argon2_encodedlen(t_cost, m_cost, parallelism, salt_len, hash_len, Argon2_id);
	std::string encoded(encoded_len, '\0');

	const int Result = argon2id_hash_encoded(
		t_cost, m_cost, parallelism,
		InputString.c_str(), InputString.length(),
		salt.c_str(), salt_len,
		hash_len,
		encoded.data(), encoded_len
	);

	return ( (Result == ARGON2_OK) ? encoded : std::string() );
}

bool FPasswordEncryptionArgon::VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash)
{
	return ( argon2id_verify(StringWithHash.c_str(), StringWithoutHash.c_str(), StringWithoutHash.length()) == ARGON2_OK );
}

std::string FPasswordEncryptionArgon::GenerateSecureSalt(const size_t Length) const
{
	std::random_device rd;  // Hardware entropy
	std::mt19937 gen(rd()); // Seed with hardware randomness
	std::uniform_int_distribution<uint8_t> dis(0, UINT8_MAX);

	std::string salt(Length, '\0');
	for (size_t i = 0; i < Length; ++i) {
		salt[i] = static_cast<char>(dis(gen));
	}

	return salt;
}
