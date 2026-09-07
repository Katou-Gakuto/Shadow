#pragma once
#include <string>
#include <vector>

#include "ResourceData.h"

#include "ResourceBase.h"

class ResourceGraph : public ResourceBase<int>
{
public:
	ResourceGraph();
	~ResourceGraph();

	void Initilize();
	void Finalize() override;

private:
    /// <summary>リソース本体作成</summary>
    int CreateResource(const std::string& fileName, int* plusData = nullptr) override;
    /// <summary>リソース複製</summary>
    int ResourceDuplication(const std::string& fileName, int* plusData = nullptr) override;
    /// <summary>リソース削除</summary>
    void ResourceDelete(const std::vector<int>& handles) override;
};

class ResourceDivGraph : public ResourceBase<DIV_GRAPH_DATA, std::string, DIV_GRAPH_DATA>
{
public:
	ResourceDivGraph();
	~ResourceDivGraph();

	void Initilize();
	void Finalize() override;

private:
    /// <summary>リソース本体作成</summary>
    DIV_GRAPH_DATA CreateResource(const std::string& fileName, DIV_GRAPH_DATA* plusData = nullptr) override;
    /// <summary>リソース複製</summary>
    DIV_GRAPH_DATA ResourceDuplication(const std::string& fileName, DIV_GRAPH_DATA* plusData = nullptr) override;
    /// <summary>リソース削除</summary>
    void ResourceDelete(const std::vector<DIV_GRAPH_DATA>& handles) override;
};