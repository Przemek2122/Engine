// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkServerBase.h"

FNetworkServerBase::FNetworkServerBase(FNetworkManager* InNetworkManager)
	: FNetworkClassBase(InNetworkManager)
{
}

#endif