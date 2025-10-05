#include "CoreEngine.h"
#include "Threads/ThreadData.h"

FThreadData::FThreadData(FThreadsManager* InThreadsManager, const std::string& InNewThreadName)
	: ThreadInputData(new FThreadInputData(InThreadsManager, InNewThreadName))
	, Thread(nullptr)
{
}

FThreadWorkerData::FThreadWorkerData(FThreadsManager* InThreadsManager, const std::string& InNewThreadName)
	: FThreadData(InThreadsManager, InNewThreadName)
	, ThreadNumber(-1)
{
}
