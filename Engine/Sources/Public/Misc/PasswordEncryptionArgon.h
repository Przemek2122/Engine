// Created by Przemys³aw Wiewióra 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "PasswordEncryptionBase.h"

struct FArgonSettings
{
    FArgonSettings(Uint32 t_cost = 2, Uint32 m_cost = 16 * 1024, Uint32 parallelism = 1, Uint32 hash_len = 32, Uint32 salt_len = 16)
        : t_cost(t_cost)
        , m_cost(m_cost)
        , parallelism(parallelism)
        , hash_len(hash_len)
        , salt_len(salt_len)
    {
    }

    Uint32 t_cost;
    Uint32 m_cost;
    Uint32 parallelism;
    Uint32 hash_len;
    Uint32 salt_len;
};

/** Argon encryption */
class ENGINE_API FPasswordEncryptionArgon : public FPasswordEncryptionBase
{
public:
	std::string HashPasswordCustom(const std::string& InputString, const FArgonSettings& ArgonSettings);
	std::string HashPassword(const std::string& InputString) override;
	bool VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash) override;

};
