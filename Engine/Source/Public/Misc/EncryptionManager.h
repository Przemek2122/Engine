// Created by Przemysław Wiewióra 2020-2025

#pragma once

#include "CoreMinimal.h"

class FPasswordEncryptionBase;
enum class EPasswordEncryptionAlgorithm : Uint8;

/**
 * Encryption manager
 *
 * To use, create instance and use. (Preferably create one instance per thread)
 */
class FEncryptionManager
{
public:
	std::string HashPassword(const std::string& InputString, const EPasswordEncryptionAlgorithm EncryptionAlgorithm);
	bool VerifyPassword(const std::string& PasswordWithHash, const std::string& PasswordWithoutHash, const EPasswordEncryptionAlgorithm PasswordEncryptionAlgorithm);

protected:
	std::vector<std::unique_ptr<FPasswordEncryptionBase>> EncryptorArray;

};
