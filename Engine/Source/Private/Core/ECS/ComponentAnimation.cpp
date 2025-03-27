#include "CoreEngine.h"
#include "ECS/ComponentAnimation.h"

FComponentAnimation::FComponentAnimation(UComponent* InComponentToAnimate)
	: ComponentToAnimate(InComponentToAnimate)
	, Entity(nullptr)
	, bIsAnimationRunning(false)
	, bAutoUpdate(true)
	, bAutoDelete(true)
{
#if DEBUG
	if (ComponentToAnimate == nullptr)
	{
		LOG_ERROR("ComponentToAnimate == nullptr");
	}
#endif
}

void FComponentAnimation::Init(EEntity* InEntity)
{
#if DEBUG
	if (!InEntity->HasComponent(ComponentToAnimate))
	{
		LOG_ERROR("Entity should be owner of component it is animating.");
	}
#endif

	Entity = InEntity;
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
	if (bIsAnimationRunning && bAutoUpdate)
	{
		TickAnimation(DeltaTime);
	}
}

void FComponentAnimation::TickManual()
{
	if (bIsAnimationRunning)
	{
		TickAnimation(FGlobalDefines::GEngine->GetDeltaTime());
	}
}

void FComponentAnimation::End()
{
	if (bIsAnimationRunning)
	{
		bIsAnimationRunning = false;

		EndAnimation();

		OnAnimationFinished.Execute();

		if (bAutoDelete)
		{
			if (Entity != nullptr)
			{
				Entity->DestroyComponentAnimation(this);
			}
			else
			{
				LOG_ERROR("Unable to auto delete, Entity is missing.");
			}
		}
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

void FComponentAnimation::SetAutoUpdate(const bool bInAutoUpdate)
{
	bAutoUpdate = bInAutoUpdate;
}

void FComponentAnimation::SetAutoDelete(const bool bInAutoDelete)
{
	bAutoDelete = bInAutoDelete;
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

FComponentAnimationInline::FComponentAnimationInline(UComponent* InComponentToAnimate)
	: FComponentAnimation(InComponentToAnimate)
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
