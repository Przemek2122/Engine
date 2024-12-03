#include "CoreEngine.h"
#include "Threads/RenderThread.h"

FRenderCommandsWithScopeLock::FRenderCommandsWithScopeLock(FRenderThread* InRenderThread)
	: RenderThread(InRenderThread)
{
	THREAD_WAIT_FOR_MUTEX_LOCK(RenderThread->RenderCommandsMutex);
}

FRenderCommandsWithScopeLock::~FRenderCommandsWithScopeLock()
{
#if _DEBUG
	// Lock released before it should be released
	ENSURE_VALID(RenderThread->RenderCommandsMutex.IsLocked());
#endif

	RenderThread->RenderCommandsMutex.Unlock();
}

void FRenderCommandsWithScopeLock::GetRenderDelegate(const std::shared_ptr<FRenderableObject>& InRenderableObject, const ERenderOrder RenderOrder) const
{
	if (RenderThread->RenderCommands.IsValidKey(RenderOrder))
	{
		RenderThread->RenderCommands[RenderOrder].Collection.Push(InRenderableObject);
	}
}

FRenderThread::FRenderThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData)
	: FThread(InThreadInputData, InThreadData)
{
}

FRenderThread::~FRenderThread()
{
	RenderCommands.Clear();
}

void FRenderThread::StartThread()
{
	InitializeMapWithDelegates();

	FThread::StartThread();
}

void FRenderThread::StopThread()
{
	FThread::StopThread();
}

FRenderCommandsWithScopeLock FRenderThread::GetRenderCommands()
{
	return FRenderCommandsWithScopeLock(this);
}

void FRenderThread::TickThread()
{
	bIsRenderingFrameFinished = false;

	// Lock RenderCommandsMutex for copy
	THREAD_WAIT_FOR_MUTEX_LOCK(RenderCommandsMutex);

	// Copy data for render
	RenderCommandsCopy = std::move(RenderCommands);

	// After move populate map with required data
	InitializeMapWithDelegates();

	RenderCommandsMutex.Unlock();

	// Execute
	for (std::pair<const ERenderOrder, FRenderableObjectsCollection>& RenderCommand : RenderCommandsCopy)
	{
		for (std::shared_ptr<FRenderableObject>& RenderableObject : RenderCommand.second.Collection)
		{
			RenderableObject->Render();
		}
	}

	bIsRenderingFrameFinished = true;

	while (!bIsRenderingNextFrameAllowed)
	{
		THREAD_WAIT_SHORT_TIME;
	}

	bIsRenderingNextFrameAllowed = false;
}

void FRenderThread::AllowRenderNextFrame()
{
	bIsRenderingNextFrameAllowed = true;
}

void FRenderThread::InitializeMapWithDelegates()
{
	RenderCommands.Emplace(ERenderOrder::Pre, FRenderableObjectsCollection());
	RenderCommands.Emplace(ERenderOrder::Default, FRenderableObjectsCollection());
	RenderCommands.Emplace(ERenderOrder::Post, FRenderableObjectsCollection());
}
