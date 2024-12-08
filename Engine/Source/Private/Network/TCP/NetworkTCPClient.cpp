// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/TCP/NetworkTCPClient.h"

#include "Network/NetworkManager.h"

FNetworkTCPClient::FNetworkTCPClient(FNetworkManager* InNetworkManager)
	: FNetworkClientBase(InNetworkManager)
{
}

void FNetworkTCPClient::Initialize()
{
	FNetworkClientBase::Initialize();

	const FNetworkConnectionData& DefaultNetworkConnectionData = NetworkManager->GetDefaultNetworkConnectionData();


}

void FNetworkTCPClient::DeInitialize()
{

	FNetworkClientBase::DeInitialize();
}

void FNetworkTCPClient::AsyncWork()
{
	FNetworkClientBase::AsyncWork();


}

#endif