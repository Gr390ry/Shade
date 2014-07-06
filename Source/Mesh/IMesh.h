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

		Vertex() : position(0, 0, 0), normal(0, 0, 0), uv(0, 0)
		{
		}
		Vertex(XMFLOAT3 _position, XMFLOAT3 _normal, XMFLOAT2 _uv) : position(_position), normal(_normal), uv(_uv)
		{
		}
	};

public:
	virtual bool			Initialize(const char*) = 0;
	virtual void			Release() = 0;
	virtual const Vertex*	GetVertices() = 0;
	virtual const int*		GetIndices() = 0;
	virtual const int		GetNumVertices() = 0;
	virtual const int		GetNumIndices() = 0;
};