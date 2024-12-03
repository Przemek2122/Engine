// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkServerBase.h"

class FNetworkTCPServer : public FNetworkServerBase
{
public:
	FNetworkTCPServer(FNetworkManager* InNetworkManager);
	~FNetworkTCPServer() override;

	void Initialize() override;
	void DeInitialize() override;
	void AsyncWork() override;

	void OnNewClientConnected(uv_stream_t* InUVServer);

private:
	uv_stream_t* UVServer;

};

#endif