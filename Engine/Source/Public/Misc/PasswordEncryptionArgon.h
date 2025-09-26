// Created by Przemys³aw Wiewióra 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "PasswordEncryptionBase.h"

/** Argon encryption */
class ENGINE_API FPasswordEncryptionArgon : public FPasswordEncryptionBase
{
public:
	std::string HashPassword(const std::string& InputString) override;
	bool VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash) override;

private:
	std::string GenerateSecureSalt(size_t Length) const;

};
