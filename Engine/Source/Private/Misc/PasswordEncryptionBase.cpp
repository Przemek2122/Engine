// Created by Przemys�aw Wiewi�ra 2020-2025

#include "CoreEngine.h"
#include "Misc/PasswordEncryptionBase.h"

FPasswordEncryptionBase::FPasswordEncryptionBase(const EPasswordEncryptionAlgorithm InPasswordEncryptionAlgorithm)
	: PasswordEncryptionAlgorithm(InPasswordEncryptionAlgorithm)
{
}
