// Created by https://www.linkedin.com/in/przemek2122/ 2020-2025

#pragma once

#include "CoreMinimal.h"

enum class ENGINE_API EPasswordEncryptionAlgorithm : Uint8
{
	Argon2
};

/** Each encryption type should inherit from this class */
class ENGINE_API FPasswordEncryptionBase
{
public:
	virtual std::string HashPassword(const std::string& InputString);
	virtual bool VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash);

};
