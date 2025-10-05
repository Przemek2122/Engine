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
	void Tick(float DeltaTime);
	void TickManual();
	void End();

	/** Pause animation */
	void Stop();

	/** Continue animation */
	void Resume();

	/** Cancel and restore state */
	void Abort();

	/** Should tick be called every frame when it's active or should tick be called manually */
	void SetAutoUpdate(bool bInAutoUpdate);

	/** If true animation instance will be removed after end */
	void SetAutoDelete(bool bInAutoDelete);

	/** Called when animation ends */
	FDelegate<void> OnAnimationFinished;

	UComponent* GetComponentToAnimate() const { return ComponentToAnimate; }
	bool IsAnimationRunning() const { return bIsAnimationRunning; }

protected:
	/** Called on begin of animation */
	virtual void BeginAnimation() = 0;

	/** Called every frame during animation */
	virtual void TickAnimation(float DeltaTime) = 0;

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

	/** True if should call TickAnimation from entity */
	bool bAutoUpdate;

	/** If true the animation will be auto deleted */
	bool bAutoDelete;

};

/**
 * Class for animating Entity components using delegates
 * Does not require creating separate class for whole animation
 */
class ENGINE_API FComponentAnimationInline : public FComponentAnimation
{
public:
	FComponentAnimationInline(UComponent* InComponentToAnimate);

protected:
	void BeginAnimation() override;
	void TickAnimation(float DeltaTime) override;
	void EndAnimation() override;

public:
	FDelegate<void, UComponent*> OnBegin;
	FDelegate<void, UComponent*, float> OnUpdate;
	FDelegate<void, UComponent*> OnEnd;

};