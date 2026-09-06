#pragma once
#include <string>

#include "ResourceBase.h"

class ResourceMovie : public ResourceBase<int>
{
public:
	ResourceMovie();
	~ResourceMovie();

	void Initilize();
	void Finalize() override;

	void PlayMovie(int handle);
	void StopMovie(int handle);
	void MovieReset(int handle);
	void MovieLoop(int handle);

private:
    /// <summary>リソース本体作成</summary>
    int CreateResource(const std::string& fileName, int* plusData = nullptr) override;
    /// <summary>リソース複製</summary>
    int ResourceDuplication(const std::string& fileName, int* plusData = nullptr) override;
    /// <summary>リソース削除</summary>
    void ResourceDelete(const std::vector<int>& handles) override;
};
