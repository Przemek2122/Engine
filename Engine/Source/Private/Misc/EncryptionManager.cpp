// Created by Przemys³aw Wiewióra 2020-2025

#include "CoreEngine.h"
#include "Misc/EncryptionManager.h"
#include "Misc/PasswordEncryptionBase.h"

std::string FEncryptionManager::HashPassword(const std::string& InputString, const EPasswordEncryptionAlgorithm EncryptionAlgorithm)
{
	std::string HashedString;

	for (std::unique_ptr<FPasswordEncryptionBase>& Encryptor : EncryptorArray)
	{
		if (Encryptor->GetPasswordEncryptionAlgorithm() == EncryptionAlgorithm)
		{
			HashedString = Encryptor->HashPassword(InputString);

			break;
		}
	}

	return HashedString;
}

bool FEncryptionManager::VerifyPassword(const std::string& PasswordWithHash, const std::string& PasswordWithoutHash, const EPasswordEncryptionAlgorithm PasswordEncryptionAlgorithm)
{
	bool bIsEqual = false;

	for (std::unique_ptr<FPasswordEncryptionBase>& Encryptor : EncryptorArray)
	{
		if (Encryptor->GetPasswordEncryptionAlgorithm() == PasswordEncryptionAlgorithm)
		{
			bIsEqual = Encryptor->VerifyPassword(PasswordWithHash, PasswordWithoutHash);

			break;
		}
	}

	return bIsEqual;
}
