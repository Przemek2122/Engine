// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkClientBase.h"
#include "Network/NetworkServerBase.h"

enum class ENetworkConnectionMethod : uint8
{
	TCP
};

struct FNetworkConnectorServerData
{
	FClassStorage<FNetworkServerBase, FNetworkManager*> NetworkServerClass;
};

struct FNetworkConnectorClientData
{
	FClassStorage<FNetworkClientBase, FNetworkManager*> NetworkServerClass;
};

struct FNetworkConnectionData
{
	FNetworkConnectionData(const std::string& InNetworkAddressIP, const int32 InNetworkPort, const int32 InNetworkDefaultBackLog);

	void SetNetworkAddressIP(const std::string& NewNetworkAddressIP);
	void SetNetworkPort(const int32 NewPort);
	void SetNetworkDefaultBackLog(const int32 NewBackLogSize);

	const std::string& GetNetworkAddressIP() const { return NetworkAddressIP; }
	int32 GetNetworkPort() const { return NetworkPort; }
	int32 GetNetworkDefaultBackLog() const { return NetworkDefaultBackLog; }

protected:
	/** Network address */
	std::string NetworkAddressIP;

	/** Network port */
	int32 NetworkPort;

	/** This defines the maximum number of pending connections that the operating system will queue for the listening socket. */
	int32 NetworkDefaultBackLog;

};

class FNetworkThread;

class FNetworkManager
{
	friend FEngine;
	friend FNetworkThread;

private:
	FNetworkManager(FEngine* InEngine, const bool bInIsServer);
	virtual ~FNetworkManager() = default;

public:
	/** @returns true - it's server, false - client */
	bool IsServer() const { return bIsServer; }

	const FNetworkConnectionData& GetDefaultNetworkConnectionData() { return DefaultNetworkConnectionData; }

	void OnNewClientConnectToTCP(uv_stream_t* InUVServer);

protected:
	virtual void Initialize();
	virtual void DeInitialize();

	virtual void LoadConfig();

	virtual void InitializeServer();
	virtual void InitializeClient();

protected:
	/** Pointer back to the engine */
	FEngine* Engine;

	/** This bool defines if it's server instance, if true - it's server, false - client */
	bool bIsServer;

	/** Network connection data */
	FNetworkConnectionData DefaultNetworkConnectionData;

	/** Map with server classes. */
	CMap<ENetworkConnectionMethod, FNetworkConnectorServerData> NetworkServerConnectorsMap;

	/** Map with client classes. */
	CMap<ENetworkConnectionMethod, FNetworkConnectorClientData> NetworkClientConnectorsMap;

	/** Instances of servers connections currently running */
	CArray<FNetworkServerBase*> NetworkServerInstances;

	/** Instances of client connections currently running */
	CArray<FNetworkClientBase*> NetworkClientInstances;

};

#endif
