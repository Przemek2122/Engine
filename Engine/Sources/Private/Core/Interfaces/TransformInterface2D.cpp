// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Interfaces/TransformInterface2D.h"

const static FTransform2DSize SizeDefault = { 16, 16 };

FTransform2D::FTransform2D()
	: Location(0)
	, Rotation(0)
	, Size(SizeDefault)
{
}

FTransform2DInterface::FTransform2DInterface()
	: AbsoluteTransform2D()
	, ParentTransform2D()
	, RelativeTransform2D()
	, TransformFollowMethod(ETransformFollowMethod::Default)
{
}

void FTransform2DInterface::SetTransform(const FTransform2D& NewTransform2D)
{
	if (RelativeTransform2D.Location != NewTransform2D.Location)
	{
		RelativeTransform2D.Location = NewTransform2D.Location;

		OnLocationChanged();
	}

	if (RelativeTransform2D.Rotation != NewTransform2D.Rotation)
	{
		RelativeTransform2D.Rotation = NewTransform2D.Rotation;

		OnRotationChanged();
	}
}

void FTransform2DInterface::SetLocation(const FTransform2DLocation& NewLocation)
{
	if (RelativeTransform2D.Location != NewLocation)
	{
		RelativeTransform2D.Location = NewLocation;

		OnLocationChanged();
	}
}

void FTransform2DInterface::SetRotation(const FTransform2DRotation NewRotation)
{
	if (RelativeTransform2D.Rotation != NewRotation)
	{
		RelativeTransform2D.Rotation = NewRotation;

		OnRotationChanged();
	}
}

void FTransform2DInterface::SetSize(const FTransform2DSize& NewSize)
{
	if (RelativeTransform2D.Size != NewSize)
	{
		RelativeTransform2D.Size = NewSize;

		OnSizeChanged();
	}
}

ETransformFollowMethod FTransform2DInterface::GetTransformFollowMethod() const
{
	return TransformFollowMethod;
}

void FTransform2DInterface::SetTransformFollowMethod(const ETransformFollowMethod InTransformFollowMethod)
{
	TransformFollowMethod = InTransformFollowMethod;
}

void FTransform2DInterface::AddUpdatedComponent(FTransform2DInterface* InTransform2DInterface)
{
	ChildrenInterfaces.Push(InTransform2DInterface);
}

void FTransform2DInterface::RemoveUpdatedComponent(FTransform2DInterface* InTransform2DInterface)
{
	ChildrenInterfaces.Remove(InTransform2DInterface);
}

void FTransform2DInterface::OnParentLocationChanged(const FTransform2DLocation& NewLocation)
{
	ParentTransform2D.Location = NewLocation;

	OnLocationChanged();
}

void FTransform2DInterface::OnParentRotationChanged(const FTransform2DRotation NewRotation)
{
	ParentTransform2D.Rotation = NewRotation;

	OnRotationChanged();
}

void FTransform2DInterface::OnLocationChanged()
{
	if (TransformFollowMethod == ETransformFollowMethod::RotateAroundParent)
	{
		RotateAroundParentLocation = RelativeTransform2D.Location.Rotate(AbsoluteTransform2D.Rotation);

		AbsoluteTransform2D.Location = ParentTransform2D.Location + RotateAroundParentLocation;
	}
	else
	{
		AbsoluteTransform2D.Location = ParentTransform2D.Location + RelativeTransform2D.Location;
	}

	OnLocationChangedDelegate.Execute(AbsoluteTransform2D.Location);

	for (FTransform2DInterface* ChildrenInterface : ChildrenInterfaces)
	{
		ChildrenInterface->OnParentLocationChanged(AbsoluteTransform2D.Location);
	}
}

void FTransform2DInterface::OnRotationChanged()
{
	AbsoluteTransform2D.Rotation = ParentTransform2D.Rotation + RelativeTransform2D.Rotation;

	if (TransformFollowMethod == ETransformFollowMethod::RotateAroundParent)
	{
		OnLocationChanged();
	}

	OnRotationChangedDelegate.Execute(AbsoluteTransform2D.Rotation);

	for (FTransform2DInterface* ChildrenInterface : ChildrenInterfaces)
	{
		ChildrenInterface->OnParentRotationChanged(AbsoluteTransform2D.Rotation);
	}
}

void FTransform2DInterface::OnSizeChanged()
{
	OnSizeChangedDelegate.Execute(RelativeTransform2D.Size);
}
