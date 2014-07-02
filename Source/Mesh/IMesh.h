#pragma once
#include "../ShadeEngine.h"

class IMesh
{
public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

public:
	virtual bool			Initialize(const char*) = 0;
	virtual void			Release() = 0;
	virtual const Vertex*	GetVertices() = 0;
	virtual const int*		GetIndices() = 0;
	virtual const int&		GetNumVertices() = 0;
	virtual const int&		GetNumIndices() = 0;
};