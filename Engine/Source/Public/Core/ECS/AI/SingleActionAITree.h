// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "AIActionBase.h"
#include "CoreMinimal.h"

class FAIActionBase;

enum class ESingleActionChooseMethod
{
	/** Choose random action */
	Random,
	/** Choose first added and first available action */
	FirstAvailable
};

/**
 * AI Trees are used for decision making on which action should AI Choose
 * This specific tree always chooses only one action at a time
 */
class FSingleActionAITree : public FAITree
{
public:
	void ChooseAction() override;

protected:
	ESingleActionChooseMethod SingleActionChooseMethod;

};
