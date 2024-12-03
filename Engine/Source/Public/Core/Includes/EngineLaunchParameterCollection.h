// Created by Przemysław Wiewióra 2024

#pragma once
#include "Misc/StringHelpers.h"

/**
 * Engine launch parameter available on Engine class inside of LaunchParameters array
 * There is raw 'AsString' as well as 'AsBool' and 'AsInt' functions
 */
struct FEngineLaunchParameter
{
	std::string Name;
	std::string Value;

	/** Can be used to determine if paramter was found. */
	bool IsValid() const { return !Name.empty(); }

	bool AsBool() const { return FStringHelpers::ToBoolValue(Value); }
	int32 AsInt() const { return SDL_atoi(Value.c_str()); }
	const std::string& AsString() const { return Value; }
};

/** Namespace with predefined names of input parameters */
namespace FEngineLaunchParameterCollection
{
	extern const std::string IsServer;
};
