// Created by https://www.linkedin.com/in/przemek2122/ 2020

#pragma once

#include "../Widget.h"

class ENGINE_API FInputTextWidget : public FWidget
{
public:
	FInputTextWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName);

	void SetText(const std::string& Text) { CurrentText = Text; };
	NO_DISCARD const std::string& GetText() const { return CurrentText; }

protected:
	std::string CurrentText;
};
