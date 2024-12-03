// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Threads/Thread.h"

struct FThreadInputData;
class FNetworkClassBase;

/**
 * Thread for 
 */
class FNetworkThread : public FThread
{
	friend FEngine;

public:
	FNetworkThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData);
	~FNetworkThread() override;

	void SetNetworkManager(FNetworkClassBase* InNetworkObject);
	void SetIsNetworkReady(const bool bInReady);

protected:
	void TickThread() override;

protected:
	FNetworkClassBase* NetworkObject;
	bool bIsNetworkReady;

};

#endif