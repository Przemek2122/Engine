#include "CoreEngine.h"
#include "ECS/Components/WidgetAttachmentComponent.h"

UWidgetAttachmentComponent::UWidgetAttachmentComponent(IComponentManagerInterface* InComponentManagerInterface)
	: UComponent(InComponentManagerInterface)
	, Widget(nullptr)
{
}

void UWidgetAttachmentComponent::SetWidget(FWidget* NewWidget)
{
	if (NewWidget != nullptr)
	{
		Widget = NewWidget;

		NewWidget->SetWidgetLocation(GetAbsoluteLocation());
		NewWidget->SetWidgetSize(GetSize());
	}
}

bool UWidgetAttachmentComponent::IsWidgetVisible() const
{
	return (Widget != nullptr && Widget->IsVisible());
}

void UWidgetAttachmentComponent::OnLocationChanged()
{
	UComponent::OnLocationChanged();

	if (Widget != nullptr)
	{
		Widget->SetWidgetLocation(GetAbsoluteLocation());
	}
}

void UWidgetAttachmentComponent::OnSizeChanged()
{
	UComponent::OnSizeChanged();

	if (Widget != nullptr)
	{
		Widget->SetWidgetSize(GetSize());
	}
}
