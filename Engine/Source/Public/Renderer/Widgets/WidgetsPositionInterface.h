// See https://github.com/Przemek2122/GameEngine

#pragma once

#include "CoreMinimal.h"
#include "WidgetsManagementInterface.h"

/**
 * Class responsible for managing widget location and size.
 */
class IWidgetPositionInterface : public IWidgetManagementInterface
{
	friend FWidget;
	
public:
	IWidgetPositionInterface(IWidgetManagementInterface* InWidgetManagementInterface);
	~IWidgetPositionInterface() override = default;

	/** Begin IWidgetManagementInterface */
	void GenerateWidgetGeometry(FWidgetGeometry& InWidgetGeometry) override;
	virtual void GenerateDesiredWidgetGeometry();
	void RebuildWidget() override;
	/** End IWidgetManagementInterface */

	bool IsLocationInsideWidget(const FVector2D<int32>& TestLocation) const;

	_NODISCARD virtual FVector2D<int32> GetWidgetLocation(EWidgetOrientation WidgetOrientation = EWidgetOrientation::Absolute) const;

	const CArray<FWidgetGeometry>& GetChildrenGeometry() const { return ChildrenGeometry; }
	const FWidgetGeometry& GetDesiredWidgetGeometry() const { return DesiredWidgetGeometry; }

	/** Set widget location on screen/n @bSetNoneAnchor set to true if you want to disable Anchors */
	virtual void SetWidgetLocation(const FVector2D<int32> InWidgetLocation, EWidgetOrientation WidgetOrientation = EWidgetOrientation::Absolute, const bool bSetNoneAnchor = true, const bool bWasSentFromRebuild = false);
	
	/** @returns size of this widget. */
	_NODISCARD virtual const FVector2D<int32>& GetWidgetSize() const;

	/** Set size of this widget in pixels. */
	virtual void SetWidgetSize(const FVector2D<int32> InWidgetSize, const bool bWasSentFromRebuild = false);

	/** Set size of this widget in percentage of parent size (0.0 to 1.0) where 1.0 means 100% size. */
	virtual void SetWidgetSizePercent(const FVector2D<float> InScreenPercentage);

	virtual void UpdateWidget();

	/** Called when changed location */
	virtual void UpdateWidgetLocation();

	/** Called when changed size */
	virtual void UpdateWidgetSize(const bool bWasSentFromRebuild);

	/** Called when anchor changed */
	virtual void UpdateAnchor(const bool bIsFromRebuild = false);

	void SetDefaultAnchor(const EAnchor NewAnchor);
	void SetAnchor(const EAnchor NewAnchor);
	_NODISCARD EAnchor GetAnchor() const;

	void SetClippingMethod(const EClipping NewClippingMethod);
	_NODISCARD EClipping GetClippingMethod() const;

	void SetWidgetMargin(const FWidgetMargin& InWidgetMargin);
	const FWidgetMargin& GetWidgetMargin() const { return WidgetMargin; }

	_NODISCARD virtual std::string GetName() const;

	FDelegateSafe<void> OnLocationChanged;
	FDelegateSafe<void> OnSizeChanged;

	/** @Note: Called before setting ClippingMethod with NewClippingMethod */
	virtual void OnClippingMethodChanged(EClipping NewClippingMethod);

protected:
	virtual void UpdateAnchorForChildren(const bool bIsFromRebuild);

	void UpdateLocation();

	void UpdateSizeInPercent(const bool bWasSentFromRebuild);
	void UpdateWidgetSizePixels(const bool bWasSentFromRebuild);

	void GenerateChildWidgetGeometry();

	bool RebuildChildren();

	/** Desired size determined by children */
	FWidgetGeometry DesiredWidgetGeometry;

private:
	/** Current widget location absolute */
	FVector2D<int32> WidgetLocationInterface;

	/** Type of size of widgets. Pixels or Screen percentage */
	EWidgetSizeType WidgetSizeType;

	/** Size of this widget in pixels */
	FVector2D<int32> WidgetSizeInPixelsInterface;

	/** Size of this widget in screen percentage */
	FVector2D<float> WidgetSizeInScreenPercentInterface;

	/** Default anchor when this widget is created.\n @See Init() */
	EAnchor DefaultAnchor;
	/** Anchor of this widget. Used to align to parent. */
	EAnchor CurrentAnchor;

	/** Padding for this widget */
	FWidgetMargin WidgetMargin;

	/** Defines widget behavior if it's too big. */
	EClipping ClippingMethodInterface;

	/** if children does not fit and this property is set to true we will resize widget to fit them */
	bool bShouldChangeSizeToFitChildren;

	/** It should always match children */
	CArray<FWidgetGeometry> ChildrenGeometry;
	
};
