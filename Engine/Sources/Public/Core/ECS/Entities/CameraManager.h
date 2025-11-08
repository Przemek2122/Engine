// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "CoreMinimal.h"
#include "ECS/Entity.h"

enum class EInputState;

class ENGINE_API ECameraManager : public EEntity
{
public:
	ECameraManager(FEntityManager* InEntityManager);

	/** Begin EEntity */
	void BeginPlay() override;
	void EndPlay() override;
	void Tick(float DeltaTime) override;
	/** End EEntity */

	void RegisterInput();
	void UnregisterInput();

	bool OnMouseMove(FVector2D<int> CurrentMouseLocation, EInputState);
	bool OnMouseRightClick(FVector2D<int> CurrentMouseLocation, EInputState InputState);

protected:
	/** MapManager - Sending map location */
	FMapManager* WindowMapManager;

	FVector2D<int> LastMouseLocation;

	/** Set in OnMouseRightClick and used in OnMouseMove to move map when holding right-click */
	bool bIsRightMouseButtonPressed;

}; 
