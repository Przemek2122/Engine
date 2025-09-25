// Created by Przemys³aw Wiewióra 2020-2025

#pragma once

#include "CoreMinimal.h"

enum class EPasswordEncryptionAlgorithm : Uint8
{
	Argon2
};

/** Each encryption type should inherit from this class */
class FPasswordEncryptionBase
{
public:
	FPasswordEncryptionBase(const EPasswordEncryptionAlgorithm InPasswordEncryptionAlgorithm);

	virtual std::string HashPassword(const std::string& InputString) = 0;
	virtual bool VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash) = 0;

	EPasswordEncryptionAlgorithm GetPasswordEncryptionAlgorithm() const { return PasswordEncryptionAlgorithm; };

protected:
	EPasswordEncryptionAlgorithm PasswordEncryptionAlgorithm;

};
