// Created by https://www.linkedin.com/in/przemek2122/ 2020-2025

#include "CoreEngine.h"
#include "Misc/PasswordEncryptionBase.h"

std::string FPasswordEncryptionBase::HashPassword(const std::string& InputString)
{
	LOG_ERROR("Empty implementation");

	return std::string();
}

bool FPasswordEncryptionBase::VerifyPassword(const std::string& StringWithHash, const std::string& StringWithoutHash)
{
	LOG_ERROR("Empty implementation");

	return false;
}
