// Created by https://www.linkedin.com/in/przemek2122/ 2020-2025

#pragma once

#include "CoreMinimal.h"

class FPasswordEncryptionBase;
enum class EPasswordEncryptionAlgorithm : Uint8;

/**
 * Encryption manager
 *
 * Helper for using encryption
 */
class ENGINE_API FEncryptionManager
{
public:
	/** Search for encryptor, creates if it does not exist */
	template<typename TEncryptorClass>
	static std::unique_ptr<TEncryptorClass> CreateEncryptorForPassword()
	{
		std::unique_ptr<TEncryptorClass> EncryptorUniquePtr = std::make_unique<TEncryptorClass>();

		return EncryptorUniquePtr;
	}
};
