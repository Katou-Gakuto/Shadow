#include <future>
#include <iostream>
#include <vector>

#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
}

// I—¹
void ThreadManager::Finalize()
{
	for (int i = 0; i < mfuAsyncThread.size(); i++)
	{
        if (mfuAsyncThread[i].valid())
        {
            mfuAsyncThread[i].get();
        }
	}
    mfuAsyncThread.clear();
}

// I—¹‚µ‚½”ñ“¯Šú‚ğíœ
void ThreadManager::CleanupReadyTasks()
{
    auto threadIt = mfuAsyncThread.begin();

    while (threadIt != mfuAsyncThread.end())
    {
        std::future_status threadStatus = threadIt->wait_for(std::chrono::seconds(0));

        if (threadStatus == std::future_status::ready)
        {
            threadIt->get(); // —áŠO‰ñû
            threadIt = mfuAsyncThread.erase(threadIt);
        }
        else
        {
            ++threadIt;
        }
    }
}