#pragma once
#include "IMesh.h"

#ifndef _SCENE_CACHE_H
#define _SCENE_CACHE_H

class StaticMesh : public IMesh
{
private:
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};

private:
	void GetFBXInfo(FbxNode*);

public:	
	bool Initialize(const char*);
	void Release();
	const Vertex* GetVertices() { return mVertices; }
	const int* GetIndices() { return mIndices; }
	const int&		GetNumVertices() { return mNumVertices; };
	const int&		GetNumIndices() { return mNumIndices; };
private:
	Vertex*	mVertices;
	int* mIndices;
	int mNumVertices;
	int mNumIndices;

	std::vector<Vertex> mContainVertices;
	std::vector<int> mContainIndices;
};

#endif