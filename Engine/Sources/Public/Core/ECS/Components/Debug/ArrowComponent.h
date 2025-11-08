// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "ECS/Component.h"

class UParentComponent;

/**
 * Component for displaying simple arrow
 */
class ENGINE_API UArrowComponent : public UComponent
{
public:
	UArrowComponent(IComponentManagerInterface* InComponentManagerInterface);
	virtual ~UArrowComponent() override = default;

	/** Begin UBaseComponent */
	void BeginPlay() override;
	void EndPlay() override;
	void Render() override;
	/** End UBaseComponent */

	void SetOnlyVisibleInDebug(const bool bInOnlyVisibleInDebug);
	bool IsOnlyVisibleInDebug() const;

	void SetArrowColor(const FColorRGBA& InColor);

protected:
	bool IsDebug() const;

	void RenderArrow();

	void SetArrow(FTextureAsset* NewTextureAsset);

private:
	bool bOnlyVisibleInDebug;

	FColorRGBA ArrowColor;

	/** Root component */
	UParentComponent* RootComponent;

	FTextureAsset* ArrowTextureAsset;

	FVector2D<int> ArrowRenderSize;

};