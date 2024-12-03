// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "libuv/uv.h"

class FNetworkThread;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

/** Structure added to libuv server to be able to identify instance */
struct FCustomNetworkInstanceData
{
	FCustomNetworkInstanceData(const std::string& InServerThreadName)
		: ServerThreadName(InServerThreadName)
	{
	}

	/** Unique thread name */
	std::string ServerThreadName;
};

struct FNetworkPackageData
{
	FNetworkPackageData();

	std::string Data;

};

class FNetworkClassBase
{
public:
	FNetworkClassBase(FNetworkManager* InNetworkManager);
	virtual ~FNetworkClassBase();

	virtual void Initialize();
	virtual void DeInitialize();

	/** This work will be ticked from async thread every 1ms */
	virtual void AsyncWork() { }

	//virtual void Send();
	//virtual void Receive();

protected:
	/** Network manager */
	FNetworkManager* NetworkManager;

	/** Network thread data */
	FThreadData* NetworkThreadData;

	/** Network thread */
	FNetworkThread* NetworkThread;

	/** Library(libuv) loop */
	uv_loop_t* LoopForUV;

	/** Unique thread name */
	std::string ThreadName;

};

#endif