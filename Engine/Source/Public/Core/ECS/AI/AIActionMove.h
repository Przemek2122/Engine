// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "AIActionBase.h"

class UMoveComponent;
class FAITree;

/**
 * AI actions base class
 * It is not set as not to run automatically, you should use TryStartAction
 * If you wish to start automatically, change HasAutomaticStart to return true.
 */
class FAIActionMove : public FAIActionBase
{
public:
	FAIActionMove(FAITree* InAiTree);
	virtual ~FAIActionMove() override = default;

	/** Begin FAIActionBase */
	void Initialize() override;
	bool HasAutomaticStart() const override { return false; }
	bool ShouldFinishAction() const override;
	bool IsActionReady() const override { return true; }

	void StartAction() override;
	void EndAction() override;
	/** End FAIActionBase */

	/** Default implementation, will walk into given point */
	virtual void SetTargetLocation(const FVector2D<int32>& InLocation);

	void OnStoppedMovement();

protected:
	UMoveComponent* CurrentMoveComponent;

};
