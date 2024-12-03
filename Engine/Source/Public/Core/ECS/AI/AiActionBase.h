// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"

class FAITree;

/**
 * AI actions base class
 */
class FAIActionBase
{
public:
	FAIActionBase(FAITree* InAiTree);
	virtual ~FAIActionBase() = default;

	bool TryStartAction();

	virtual void Initialize();

	/** Ticks only when action is active */
	virtual void Tick();

	/** Called when parent tree chooses this action. */
	void Start();

	/** Call to end action */
	void End();

	/** used by AiTree to check if action should be finished */
	virtual bool ShouldFinishAction() const { return false; }

	/** Used by AiTree to decide if this action is available. */
	virtual bool IsActionReady() const { return true; }

	/** Used by AiTree to decide if this action should start automatically. */
	virtual bool HasAutomaticStart() const { return true; }

	/** You can override if you want more complicated logic for determinig if action is running but, it would be nice for performance to do not touch */
	virtual bool IsActionRunning() const { return bIsActionRunning; }

	/** @return owner AI tree */
	FAITree* GetTree() const;

	/** @return Entity owner for this action */
	EEntity* GetOwnerEntity() const;

protected:
	virtual void StartAction();
	virtual void EndAction();

private:
	FAITree* AiTree;

	bool bIsActionRunning;

};
