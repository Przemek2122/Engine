// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

/**
 * Singleton manager for engine class.
 */
class FEngineManager
{
public:
	FEngineManager();
	~FEngineManager();

public:

	/** Call once before doing anything with engine. */
	template<class TEngineClass = FEngine>
	void Start(const int InArgc, char* InArgv[])
	{
		std::cout << "Game engine initializing ..." << std::endl;

		GEngine = new TEngineClass();
		
		Init(InArgc, InArgv);

		MainLoop();

		Exit();
	}

	static FEngine* Get();
	
	template<typename TEngineClass>
	static TEngineClass* Get()
	{
		return static_cast<TEngineClass>(GEngine);
	}
	
protected:
	void Init(const int Argc, char* Argv[]); 
	
	void MainLoop();
	
	void Exit();
	
};
