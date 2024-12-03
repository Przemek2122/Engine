// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "BaseComponent.h"
#include "Interfaces/TransformInterface2D.h"

class EEntity;

/**
 * It's UBaseComponent but with transform
 */
class UComponent : public UBaseComponent, public FTransform2DInterface
{
public:
	UComponent(IComponentManagerInterface* InComponentManagerInterface);

	void BeginPlay() override;

	void OnComponentCreated(const std::string& ComponentName, UBaseComponent* NewComponent) override;
	void OnComponentDestroy(const std::string& ComponentName, UBaseComponent* OldComponent) override;

	FVector2D<int> GetLocationCenter() const;

};
