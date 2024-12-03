// Created by Przemys³aw Wiewióra 2020-2024

#pragma once

#include "ParentComponent.h"
#include "ECS/Component.h"

enum class ERenderType : uint8
{
	Center,
	AbsoluteLocation,
};

enum class ERenderCenterType : uint8
{
	RotateAround,
	AtPivotPoint
};

struct FAssetCollectionItem;

/**
 * Component for handling transform of the entity
 */
class URenderComponent : public UComponent
{
public:
	URenderComponent(IComponentManagerInterface* InComponentManagerInterface);

	/** Begin UBaseComponent */
	void BeginPlay() override;
	void EndPlay() override;
	void Render() override;
	/** End UBaseComponent */

	/** Set image from AssetCollectionItem. */
	void SetImage(const FAssetCollectionItem& AssetCollectionItem);

	/** Set image from given name. It will try to find asset. If it was not created it will try to use OptionalPath to load it. */
	void SetImage(const std::string& InImageName, const std::string& OptionalPath = "");

	/** Set image from given asset. */
	void SetImage(FTextureAsset* InAsset);

	/** Set size. @Note SetImage sets always size of image, so call it after SetImage */
	void SetImageSize(const FVector2D<int>& InSize);

	void SetRenderLocationType(const ERenderType InRenderType);
	void SetRenderCenterType(const ERenderCenterType InRenderCenterType);

protected:
	/** Image to render */
	FTextureAsset* TextureAsset;

	ERenderCenterType CurrentRenderCenterType;
	ERenderType CurrentRenderType;

};
