// Created by Przemys³aw Wiewióra 2024

#pragma once

#include "CoreMinimal.h"
#include "Types/Mutex/Mutex.h"
#include "Threads/Thread.h"

typedef FDelegateSafe<void> FRenderDelegate;

enum class ERenderOrder : Uint8
{
	Pre,
	Default,
	Post
};

/**
 * Object for rendering data
 * All data used by render object should be copied inside of this object
 */
class FRenderableObject
{
public:
	virtual void Render() = 0;

};

struct FRenderableObjectsCollection
{
	CArray<std::shared_ptr<FRenderableObject>> Collection;
};

class FRenderCommandsWithScopeLock
{
public:
	FRenderCommandsWithScopeLock(FRenderThread* InRenderThread);
	~FRenderCommandsWithScopeLock();

	void GetRenderDelegate(const std::shared_ptr<FRenderableObject>& InRenderableObject, const ERenderOrder RenderOrder = ERenderOrder::Default) const;

private:
	FRenderThread* RenderThread;
	
};

/**
 * @TODO Currently abandoned implementation
 * SDL2 does not really support rendering from any other thread than main thread
 */
class FRenderThread : public FThread
{
	friend FEngine;
	friend FRenderCommandsWithScopeLock;

public:
	FRenderThread(FThreadInputData* InThreadInputData, FThreadData* InThreadData);
	~FRenderThread() override;

	void StartThread() override;
	void StopThread() override;

	FRenderCommandsWithScopeLock GetRenderCommands();

	bool IsRenderingFrameFinished() const { return bIsRenderingFrameFinished; }

protected:
	void TickThread() override;

	void AllowRenderNextFrame();

	void InitializeMapWithDelegates();

protected:
	/** Render commands collected during frame */
	CMap<ERenderOrder, FRenderableObjectsCollection> RenderCommands;

	/** Render commands to actually render */
	CMap<ERenderOrder, FRenderableObjectsCollection> RenderCommandsCopy;

	/** True if thread has finished work for this engine tick */
	std::atomic_bool bIsRenderingFrameFinished;

	/** Will be set from engine when tick is finished. */
	std::atomic_bool bIsRenderingNextFrameAllowed;

	/** Mutex for RenderCommands */
	FMutex RenderCommandsMutex;

};
