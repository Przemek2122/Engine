// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkClientBase.h"

class FNetworkTCPClient : public FNetworkClientBase
{
public:
	FNetworkTCPClient(FNetworkManager* InNetworkManager);

	void Initialize() override;
	void DeInitialize() override;
	void AsyncWork() override;

protected:
	uv_tcp_t* TCP_Socket;
	uv_connect_t* Connection;
	sockaddr_in* DestinationSocket;

};

#endif