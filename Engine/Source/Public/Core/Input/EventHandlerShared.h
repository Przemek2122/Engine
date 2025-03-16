#pragma once

#include "CoreMinimal.h"

// @TODO: EInputState should be uint8
/** This class is used to tell type of input, if it is pressed or released. */
enum class ENGINE_API EInputState
{
	NOT_PRESSED = 0,
	PRESS,
	RELEASE,
	ENUM_MAX
};
