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
	void			GetFBXInfo(FbxNode*);

public:	
	bool			Initialize(const char*) override;
	void			Release()				override;
	const Vertex*	GetVertices()			override;
	const int*		GetIndices()			override;
	const int		GetNumVertices()		override;
	const int		GetNumIndices()			override;
private:
	std::vector<Vertex> mVertices;
	std::vector<int>	mIndices;
};

#endif