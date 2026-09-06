#pragma once
#include <future>
#include <iostream>
#include <vector>

class ThreadManager
{
private:
	// 非同期スレッド
	std::vector<std::future<void>> mfuAsyncThread;

public:
	ThreadManager();
	~ThreadManager();

	/// <summary>終了</summary>
	void Finalize();

    /// <summary>終了した非同期を削除</summary>
	void CleanupReadyTasks();

	void AddThread(std::future<void>&& future) { mfuAsyncThread.emplace_back(std::move(future)); }
};