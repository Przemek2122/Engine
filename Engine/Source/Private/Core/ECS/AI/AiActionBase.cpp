// Created by Przemys³aw Wiewióra 2024

#include "CoreEngine.h"
#include "Core/ECS/AI/AIActionBase.h"

FAIActionBase::FAIActionBase(FAITree* InAiTree)
	: AiTree(InAiTree)
	, bIsActionRunning(false)
{
}

bool FAIActionBase::TryStartAction()
{
	if (AiTree != nullptr)
	{
		return AiTree->ActivateAction(this);
	}

	return false;
}

void FAIActionBase::Initialize()
{
}

void FAIActionBase::Tick()
{
}

void FAIActionBase::Start()
{
	StartAction();

	bIsActionRunning = true;
}

void FAIActionBase::End()
{
	EndAction();

	bIsActionRunning = false;
}

FAITree* FAIActionBase::GetTree() const
{
	return AiTree;
}

EEntity* FAIActionBase::GetOwnerEntity() const
{
	return AiTree->GetOwnerEntity();
}

void FAIActionBase::StartAction()
{
}

void FAIActionBase::EndAction()
{
}
