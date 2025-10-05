// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

/**
 * Singleton manager for engine class.
 */
class ENGINE_API FEngineManager
{
public:
	/**
	 * Call once before doing anything with engine.
	 * When using SDL main, follow the sample instead of using this.
	 */
	void Start(const int InArgc, char* InArgv[]);
	void MainLoop();

	void Init(const int Argc, char* Argv[]);
	void LoopIterate();
	void Exit();

	static FEngine* Get();

	template<typename TEngineClass>
	static TEngineClass* Get()
	{
		return static_cast<TEngineClass>(FGlobalDefines::GEngine);
	}

	FClassStorage<FEngine> EngineClass;
	
};
