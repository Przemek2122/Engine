// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkClientBase.h"

FNetworkClientBase::FNetworkClientBase(FNetworkManager* InNetworkManager)
	: FNetworkClassBase(InNetworkManager)
{
}

#endif