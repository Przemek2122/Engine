#include "CoreEngine.h"
#include "ECS/ComponentAnimation.h"

FComponentAnimation::FComponentAnimation(UComponent* InComponentToAnimate)
	: ComponentToAnimate(InComponentToAnimate)
	, bIsAnimationRunning(false)
{
}

void FComponentAnimation::Begin()
{
	if (!bIsAnimationRunning)
	{
		bIsAnimationRunning = true;

		BeginAnimation();
	}
}

void FComponentAnimation::Tick(const float DeltaTime)
{
	if (bIsAnimationRunning)
	{
		TickAnimation(DeltaTime);
	}
}

void FComponentAnimation::End()
{
	if (bIsAnimationRunning)
	{
		bIsAnimationRunning = false;

		EndAnimation();
	}
}

void FComponentAnimation::Stop()
{
	if (bIsAnimationRunning)
	{
		bIsAnimationRunning = false;

		OnPauseAnimation();
	}
}

void FComponentAnimation::Resume()
{
	if (!bIsAnimationRunning)
	{
		bIsAnimationRunning = true;

		OnResumeAnimation();
	}
}

void FComponentAnimation::Abort()
{
	if (bIsAnimationRunning)
	{
		bIsAnimationRunning = false;

		EndAnimation();

		OnRestoreState();
	}
}

void FComponentAnimation::OnPauseAnimation()
{
}

void FComponentAnimation::OnResumeAnimation()
{
}

void FComponentAnimation::OnRestoreState()
{
}

void FComponentAnimationInline::BeginAnimation()
{
	OnBegin.Execute(GetComponentToAnimate());
}

void FComponentAnimationInline::TickAnimation(const float DeltaTime)
{
	OnUpdate.Execute(GetComponentToAnimate(), DeltaTime);
}

void FComponentAnimationInline::EndAnimation()
{
	OnEnd.Execute(GetComponentToAnimate());
}
