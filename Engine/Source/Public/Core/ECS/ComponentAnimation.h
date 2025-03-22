#pragma once

#include "CoreMinimal.h"

/**
 * Class for animating Entity components
 */
class ENGINE_API FComponentAnimation
{
public:
	FComponentAnimation(UComponent* InComponentToAnimate);

	void Init(EEntity* InEntity);

	void Begin();
	void Tick(const float DeltaTime);
	void End();

	/** Pause animation */
	void Stop();

	/** Continue animation */
	void Resume();

	/** Cancel and restore state */
	void Abort();

	UComponent* GetComponentToAnimate() const { return ComponentToAnimate; }
	bool IsAnimationRunning() const { return bIsAnimationRunning; }

protected:
	/** Called on begin of animation */
	virtual void BeginAnimation() = 0;

	/** Called every frame during animation */
	virtual void TickAnimation(const float DeltaTime) = 0;

	/** Called on end of animation */
	virtual void EndAnimation() = 0;

	/** Pause animation */
	virtual void OnPauseAnimation();

	/** Resume animation */
	virtual void OnResumeAnimation();

	/** Called with abort to restore state before animation */
	virtual void OnRestoreState();

private:
	/** Component which we animate */
	UComponent* ComponentToAnimate;

	/** Entity owning component and this animation */
	EEntity* Entity;

	/** True if running, false if not OR paused. */
	bool bIsAnimationRunning;

};

/**
 * Class for animating Entity components using delegates
 * Does not require creating separate class for whole animation
 */
class ENGINE_API FComponentAnimationInline : public FComponentAnimation
{
protected:
	void BeginAnimation() override;
	void TickAnimation(float DeltaTime) override;
	void EndAnimation() override;

public:
	FDelegate<void, UComponent*> OnBegin;
	FDelegate<void, UComponent*, float> OnUpdate;
	FDelegate<void, UComponent*> OnEnd;

};