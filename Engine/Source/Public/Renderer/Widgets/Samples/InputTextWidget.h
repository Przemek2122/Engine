// Created by Przemys³aw Wiewióra 2020

#pragma once

#include "../Widget.h"

class FInputTextWidget : public FWidget
{
public:
	FInputTextWidget(IWidgetManagementInterface* InWidgetManagementInterface, const std::string& InWidgetName);

	void SetText(const std::string& Text) { CurrentText = Text; };
	_NODISCARD const std::string& GetText() const { return CurrentText; }

protected:
	std::string CurrentText;
};
