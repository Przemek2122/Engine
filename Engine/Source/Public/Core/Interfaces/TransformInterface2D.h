// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "CoreMinimal.h"

/** Make as def to be able to change in single place in case of need */
typedef FVector2D<int32> FTransform2DLocation;
typedef int32 FTransform2DRotation;
typedef FVector2D<int32> FTransform2DSize;

enum class ETransformFollowMethod
{
	Default,
	RotateAroundParent
};

/** Structure with location and rotation in 2D */
struct FTransform2D
{
	FTransform2D();

	FTransform2DLocation	Location;
	FTransform2DRotation	Rotation;
	FTransform2DSize		Size;
};

/**
 * Interface for managing location and rotation
 * Note:
 * Location and rotation are absolute to world (0, 0) if not attached
 * and Location and rotation are relative to parent if attached
 */
class FTransform2DInterface
{
public:
	FTransform2DInterface();
	virtual ~FTransform2DInterface() = default;

	const FTransform2D& GetAbsoluteTransform() const { return AbsoluteTransform2D; }
	const FTransform2DLocation& GetAbsoluteLocation() const { return AbsoluteTransform2D.Location; }
	FTransform2DRotation GetAbsoluteRotation() const { return AbsoluteTransform2D.Rotation; }

	const FTransform2D& GetTransform() const { return RelativeTransform2D; }
	const FTransform2DLocation& GetLocation() const { return RelativeTransform2D.Location; }
	FTransform2DRotation GetRotation() const { return RelativeTransform2D.Rotation; }
	virtual FTransform2DSize GetSize() const { return RelativeTransform2D.Size; }

	void SetTransform(const FTransform2D& NewTransform2D);
	void SetLocation(const FTransform2DLocation& NewLocation);
	void SetRotation(const FTransform2DRotation NewRotation);
	void SetSize(const FTransform2DSize& NewSize);

	ETransformFollowMethod GetTransformFollowMethod() const;
	void SetTransformFollowMethod(const ETransformFollowMethod InTransformFollowMethod);

	CArray<FTransform2DInterface*>& GetChildrenInterfaces() { return ChildrenInterfaces; }

	void AddUpdatedComponent(FTransform2DInterface* InTransform2DInterface);
	void RemoveUpdatedComponent(FTransform2DInterface* InTransform2DInterface);

	void OnParentLocationChanged(const FTransform2DLocation& NewLocation);
	void OnParentRotationChanged(const FTransform2DRotation NewRotation);

	virtual void OnLocationChanged();
	virtual void OnRotationChanged();
	virtual void OnSizeChanged();

	/** Called when location changed */
	FDelegate<void, const FTransform2DLocation&> OnLocationChangedDelegate;

	/** Called when rotation is changed */
	FDelegate<void, FTransform2DRotation> OnRotationChangedDelegate;

	/** Called when size is changed */
	FDelegate<void, FTransform2DSize> OnSizeChangedDelegate;

private:
	/** Child array of interfaces */
	CArray<FTransform2DInterface*> ChildrenInterfaces;

	/** 2D Location and rotation in world space */
	FTransform2D AbsoluteTransform2D;

	/** 2D Location and rotation got from parent on each parent change */
	FTransform2D ParentTransform2D;

	/** 2D Location and rotation relative to parent, or world (0, 0) for master interface */
	FTransform2D RelativeTransform2D;

	/** Special for ETransformFollowMethod::RotateAroundParent */
	FTransform2DLocation RotateAroundParentLocation;

	ETransformFollowMethod TransformFollowMethod;

};
