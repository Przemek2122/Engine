// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"
#include "Renderer/Widgets/WidgetsPositionInterface.h"

#include "Misc/Math.h"

IWidgetPositionInterface::IWidgetPositionInterface(IWidgetManagementInterface* InWidgetManagementInterface)
	: WidgetSizeType(EWidgetSizeType::Pixels)
	, DefaultAnchor(EAnchor::Center)
	, CurrentAnchor(EAnchor::None)
	, ClippingMethodInterface(EClipping::Cut)
	, bShouldChangeSizeToFitChildren(true)
{
}

void IWidgetPositionInterface::GenerateWidgetGeometry(FWidgetGeometry& InWidgetGeometry)
{
	GenerateChildWidgetGeometry();

	const FWidgetMargin& CurrentPadding = GetWidgetMargin();
	InWidgetGeometry.Size = GetWidgetSize() + CurrentPadding.Get();

	Super::GenerateWidgetGeometry(InWidgetGeometry);
}

void IWidgetPositionInterface::GenerateDesiredWidgetGeometry()
{
	// Default alignment is like VerticalBox
	const CArray<FWidgetGeometry>& ChildrenGeometryRef = GetChildrenGeometry();
	for (const FWidgetGeometry& Geometry : ChildrenGeometryRef)
	{
		DesiredWidgetGeometry.Size.Y += Geometry.Size.Y;

		if (Geometry.Size.X > DesiredWidgetGeometry.Size.X)
		{
			DesiredWidgetGeometry.Size.X = Geometry.Size.X;
		}
	}
}

void IWidgetPositionInterface::RebuildWidget()
{
	UpdateWidgetSize(true);
	UpdateWidgetLocation();
	UpdateAnchor(true);

	GenerateChildWidgetGeometry();

	Super::RebuildWidget();
}

bool IWidgetPositionInterface::IsLocationInsideWidget(const FVector2D<int32>& TestLocation) const
{
	const FVector2D<int32> WidgetLocation = GetWidgetLocation(EWidgetOrientation::Absolute);
	const FVector2D<int32> WidgetSize = GetWidgetSize();

	bool bIsInWidget = false;

	if (	TestLocation.X > WidgetLocation.X
		&&	TestLocation.X < WidgetLocation.X + WidgetSize.X
		&&	TestLocation.Y > WidgetLocation.Y
		&&	TestLocation.Y < WidgetLocation.Y + WidgetSize.Y	)
	{
		bIsInWidget = true;
	}

	return bIsInWidget;
}

FVector2D<int32> IWidgetPositionInterface::GetWidgetLocation(const EWidgetOrientation WidgetOrientation) const
{
	switch (WidgetOrientation)
	{
		case EWidgetOrientation::Absolute:
		{
			return WidgetLocationInterface;
		}
		case EWidgetOrientation::Relative:
		{
			return WidgetLocationInterface - GetWidgetManagerOffset();
		}
	}

	return { };
}

void IWidgetPositionInterface::SetWidgetLocation(const FVector2D<int32> InWidgetLocation, const EWidgetOrientation WidgetOrientation, const bool bSetNoneAnchor, const bool bWasSentFromRebuild)
{
	switch (WidgetOrientation)
	{
		case EWidgetOrientation::Absolute:
		{
			WidgetLocationInterface = InWidgetLocation;

			break;
		}
		case EWidgetOrientation::Relative:
		{
			WidgetLocationInterface = GetParent()->GetWidgetManagerOffset() + InWidgetLocation;

			break;
		}
	}

	if (bSetNoneAnchor)
	{
		SetAnchor(EAnchor::None);
	}

	if (!bWasSentFromRebuild)
	{
		RequestWidgetRebuild();
	}

	UpdateWidgetLocation();
}

const FVector2D<int32>& IWidgetPositionInterface::GetWidgetSize() const
{
	return WidgetSizeInPixelsInterface;
}

void IWidgetPositionInterface::SetWidgetSize(const FVector2D<int32> InWidgetSize, const bool bWasSentFromRebuild)
{
	WidgetSizeType = EWidgetSizeType::Pixels;
	WidgetSizeInPixelsInterface = InWidgetSize;

	bShouldChangeSizeToFitChildren = false;

	UpdateAnchor(bWasSentFromRebuild);
	UpdateWidgetSize(bWasSentFromRebuild);

	if (HasParent())
	{
		GetParent()->OnChildSizeChanged();
	}

	if (!bWasSentFromRebuild)
	{
		RequestWidgetRebuild();
	}
}

void IWidgetPositionInterface::SetWidgetSizePercent(const FVector2D<float> InScreenPercentage)
{
	WidgetSizeType = EWidgetSizeType::ParentPercentage;

	WidgetSizeInScreenPercentInterface = InScreenPercentage;

	UpdateSizeInPercent(false);

	UpdateAnchor();

	UpdateWidgetSize(false);

	if (HasParent())
	{
		GetParent()->OnChildSizeChanged();
	}
}

void IWidgetPositionInterface::UpdateWidget()
{
	RequestWidgetRebuild();
}

void IWidgetPositionInterface::UpdateWidgetLocation()
{
}

void IWidgetPositionInterface::UpdateWidgetSize(const bool bWasSentFromRebuild)
{
	switch (WidgetSizeType)
	{
		case EWidgetSizeType::Pixels:
		{
			UpdateWidgetSizePixels(bWasSentFromRebuild);

			break;
		}
		case EWidgetSizeType::ParentPercentage:
		{
			UpdateSizeInPercent(bWasSentFromRebuild);
			break;
		}
	}
}

void IWidgetPositionInterface::UpdateAnchor(const bool bIsFromRebuild)
{
	switch (CurrentAnchor)
	{
		case EAnchor::None:
		{
			// This option means user do not want to use Anchor system.
			// Due to no anchor we add only margin
			//FVector2D<int> RelativeCenter = { WidgetMargin.GetLeft(), WidgetMargin.GetTop() };
			//SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);

			break;
		}
		
		case EAnchor::Center:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
			
			FVector2D<int> RelativeCenter;

			// Calculate position
			RelativeCenter.X = (ParentSize.X - ThisWidgetSize.X) / 2;
			RelativeCenter.Y = (ParentSize.Y - ThisWidgetSize.Y) / 2;
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::LeftTop:
		{
			FVector2D<int32> MarginHalf = { WidgetMargin.GetLeft(), WidgetMargin.GetTop() };

			SetWidgetLocation(MarginHalf, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::LeftBottom:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = 0;
			RelativeCenter.Y = ParentSize.Y - ThisWidgetSize.Y;

			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::RightTop:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = ParentSize.X - ThisWidgetSize.X - WidgetMargin.GetRight();
			RelativeCenter.Y = WidgetMargin.GetTop();
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::RightBottom:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = ParentSize.X - ThisWidgetSize.X;
			RelativeCenter.Y = ParentSize.Y - ThisWidgetSize.Y;
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::TopCenter:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = (ParentSize.X - ThisWidgetSize.X) / 2;
			RelativeCenter.Y = 0;
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::LeftCenter:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = 0;
			RelativeCenter.Y = (ParentSize.Y - ThisWidgetSize.Y) / 2;

			// Add Margin
			RelativeCenter.X += WidgetMargin.GetLeft();
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::BottomCenter:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = (ParentSize.X - ThisWidgetSize.X) / 2;
			RelativeCenter.Y = ParentSize.Y - ThisWidgetSize.Y;
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
		
		case EAnchor::RightCenter:
		{
			const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();
			const FVector2D<int> ThisWidgetSize = GetWidgetSize();
		
			FVector2D<int> RelativeCenter;
			
			RelativeCenter.X = ParentSize.X - ThisWidgetSize.X;
			RelativeCenter.Y = (ParentSize.Y - ThisWidgetSize.Y) / 2;
			
			SetWidgetLocation(RelativeCenter, EWidgetOrientation::Relative, false, bIsFromRebuild);
			break;
		}
	}

	UpdateAnchorForChildren(bIsFromRebuild);
}

void IWidgetPositionInterface::SetDefaultAnchor(const EAnchor NewAnchor)
{
	if (DefaultAnchor != NewAnchor)
	{
		DefaultAnchor = NewAnchor;
	}
}

void IWidgetPositionInterface::SetAnchor(const EAnchor NewAnchor)
{
	if (CurrentAnchor != NewAnchor)
	{
		CurrentAnchor = NewAnchor;

		UpdateAnchor();
	}
}

EAnchor IWidgetPositionInterface::GetAnchor() const
{
	return CurrentAnchor;
}

void IWidgetPositionInterface::SetClippingMethod(const EClipping NewClippingMethod)
{
	if (ClippingMethodInterface != NewClippingMethod)
	{
		OnClippingMethodChanged(NewClippingMethod);

		ClippingMethodInterface = NewClippingMethod;
	}
}

EClipping IWidgetPositionInterface::GetClippingMethod() const
{
	return ClippingMethodInterface;
}

void IWidgetPositionInterface::SetWidgetMargin(const FWidgetMargin& InWidgetMargin)
{
	WidgetMargin = InWidgetMargin;
}

std::string IWidgetPositionInterface::GetName() const
{
	static const std::string DefaultWidgetName = "DefaultWidgetNameInInterface";

	return DefaultWidgetName;
}

void IWidgetPositionInterface::OnClippingMethodChanged(EClipping NewClippingMethod)
{
}

void IWidgetPositionInterface::UpdateAnchorForChildren(const bool bIsFromRebuild)
{
	for (FWidget* Widget : ManagedWidgets)
	{
		Widget->UpdateAnchor(bIsFromRebuild);
	}
}

void IWidgetPositionInterface::UpdateLocation()
{
	UpdateAnchor();
}

void IWidgetPositionInterface::UpdateSizeInPercent(const bool bWasSentFromRebuild)
{
	if (WidgetSizeType == EWidgetSizeType::ParentPercentage)
	{
		const FVector2D<int> ParentSize = GetParent()->GetWidgetManagerSize();

		WidgetSizeInPixelsInterface.X = FMath::RoundToInt(static_cast<float>(ParentSize.X) * WidgetSizeInScreenPercentInterface.X);
		WidgetSizeInPixelsInterface.Y = FMath::RoundToInt(static_cast<float>(ParentSize.Y) * WidgetSizeInScreenPercentInterface.Y);
	}
}

void IWidgetPositionInterface::UpdateWidgetSizePixels(const bool bWasSentFromRebuild)
{
	switch (ClippingMethodInterface)
	{
		case EClipping::None:
		{
			break;
		}

		case EClipping::Resize:
		{
			const FVector2D<int> ParentSize = GetWidgetManagerSize();

			if (ParentSize.X < WidgetSizeInPixelsInterface.X)
			{
				WidgetSizeInPixelsInterface.X = ParentSize.X;
			}

			if (ParentSize.Y < WidgetSizeInPixelsInterface.Y)
			{
				WidgetSizeInPixelsInterface.Y = ParentSize.Y;
			}

			break;
		}
		case EClipping::Cut:
		{
			const FVector2D<int> ParentSize = GetWidgetManagerSize();

			if (ParentSize.X < WidgetSizeInPixelsInterface.X)
			{
				WidgetSizeInPixelsInterface.X = ParentSize.X;
			}

			if (ParentSize.Y < WidgetSizeInPixelsInterface.Y)
			{
				WidgetSizeInPixelsInterface.Y = ParentSize.Y;
			}

			break;
		}
	}
}

void IWidgetPositionInterface::GenerateChildWidgetGeometry()
{
	const bool bAnyChanged = RebuildChildren();
	if (bAnyChanged)
	{
		ChildrenGeometry.Clear();

		for (FWidget* ManagedWidget : ManagedWidgets)
		{
			FWidgetGeometry ChildWidgetGeometry;
			ManagedWidget->GenerateWidgetGeometry(ChildWidgetGeometry);

			ChildrenGeometry.Push(ChildWidgetGeometry);
		}

		DesiredWidgetGeometry = FWidgetGeometry();

		// When we have children size we can create desired geometry
		GenerateDesiredWidgetGeometry();
	}
}

bool IWidgetPositionInterface::RebuildChildren()
{
	bool bAnyChildChanged = false;

	for (FWidget* ManagedWidget : ManagedWidgets)
	{
		if (ManagedWidget->NeedsWidgetRebuild())
		{
			ManagedWidget->RebuildWidget();

			bAnyChildChanged = true;
		}
	}

	return bAnyChildChanged;
}
