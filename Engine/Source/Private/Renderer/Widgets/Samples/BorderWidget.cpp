// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Renderer/Widgets/Samples/BorderWidget.h"

FBorderWidget::FBorderWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName, const int32 InWidgetOrder)
	: FWidget(InWidgetManagementInterface, InWidgetName, InWidgetOrder)
	, CurrentBorderDisplayMethod(EBorderDisplayMethod::BorderOutline)
{
}

void FBorderWidget::Render()
{
	Super::Render();

	FRenderer* Renderer = GetRenderer();

	switch (CurrentBorderDisplayMethod)
	{
		case EBorderDisplayMethod::BorderOutline:
		{
			Renderer->DrawRectangleOutline(GetWidgetLocation(), GetWidgetSize(), CurrentWidgetColor, false);

			break;
		}
		case EBorderDisplayMethod::BorderWithFill:
		{
			Renderer->DrawRectangle(GetWidgetLocation(), GetWidgetSize(), CurrentWidgetColor, false);

			break;
		}
		default:
		{
			LOG_WARN("Missing enum value.");
		}
	}
}

void FBorderWidget::SetBorderDisplayMethod(const EBorderDisplayMethod NewBorderDisplayMethod)
{
	CurrentBorderDisplayMethod = NewBorderDisplayMethod;
}

void FBorderWidget::SetColor(const FColorRGBA& NewColor)
{
	CurrentWidgetColor = NewColor;
}

EBorderDisplayMethod FBorderWidget::GetBorderDisplayMethod() const
{
	return CurrentBorderDisplayMethod;
}

FColorRGBA FBorderWidget::GetColor() const
{
	return CurrentWidgetColor;
}
