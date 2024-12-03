// Created by Przemys³aw Wiewióra 2020

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/HorizontalBoxWidget.h"

FHorizontalBoxWidget::FHorizontalBoxWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, HorizontalBoxAlignMethod(EHorizontalBoxAlignMethod::FromTheLeft)
	, bScaleToContent(true)
{
	// This is handled by HorizontalBox so we do not need to do it
	SetShouldChangeSizeOnChildChange(false);
}

void FHorizontalBoxWidget::GenerateDesiredWidgetGeometry()
{
	// Special align for horizontalbox
	const CArray<FWidgetGeometry>& ChildrenGeometryRef = GetChildrenGeometry();
	for (const FWidgetGeometry& Geometry : ChildrenGeometryRef)
	{
		DesiredWidgetGeometry.Size.X += Geometry.Size.X;

		if (Geometry.Size.Y > DesiredWidgetGeometry.Size.Y)
		{
			DesiredWidgetGeometry.Size.Y = Geometry.Size.Y;
		}
	}
}

void FHorizontalBoxWidget::AlignWidgets()
{
	if (bScaleToContent)
	{
		SetWidgetSize(GetDesiredWidgetGeometry().Size, true);
	}

	switch (HorizontalBoxAlignMethod)
	{
		case EHorizontalBoxAlignMethod::FromTheLeft:
		{
			AlignFromTheLeft();

			break;
		}
	}
}

void FHorizontalBoxWidget::SetScaleToContent(const bool bInScaleToContent)
{
	bScaleToContent = bInScaleToContent;
}

void FHorizontalBoxWidget::RebuildWidget()
{
	Super::RebuildWidget();

	AlignWidgets();
}

void FHorizontalBoxWidget::AlignFromTheLeft()
{
	const FVector2D<int> HorizontalBoxLocation = GetWidgetLocation(EWidgetOrientation::Relative);
	const FVector2D<int> HorizontalBoxSize = GetWidgetSize();
	const FVector2D<int> HorizontalBoxMaxBounds = HorizontalBoxLocation + HorizontalBoxSize;
	FVector2D<int> AggregatedChildSizeLast = { 0, 0 };

	for (ContainerInt i = 0; i < ManagedWidgets.Size(); i++)
	{
		FWidget* ChildWidget = ManagedWidgets[i];

		// We must ensure it's only done when it should be visible
		if (ChildWidget->IsVisible())
		{
			FVector2D<int> NewChildLocation = HorizontalBoxLocation;
			NewChildLocation.X += AggregatedChildSizeLast.X;

			FVector2D<int> ChildWidgetSize = ChildWidget->GetWidgetSize();

			const FVector2D<int> ChildBounds = NewChildLocation + ChildWidgetSize;

			// Make sure it fits inside box
			if (ChildBounds.X > HorizontalBoxMaxBounds.X)
			{
				ChildWidgetSize.X = HorizontalBoxMaxBounds.X;
			}
			if (ChildBounds.Y > HorizontalBoxMaxBounds.Y)
			{
				ChildWidgetSize.Y = HorizontalBoxMaxBounds.Y;
			}

			ChildWidget->SetWidgetLocation(NewChildLocation, EWidgetOrientation::Relative, true, true);

			AggregatedChildSizeLast.X += ChildWidgetSize.X;
		}
	}
}
