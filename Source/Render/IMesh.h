#pragma once
#include "../ShadeEngine.h"

class IMesh
{
	typedef std::vector<GENERIC::Vertex>	VEC_VERTEX;
	typedef std::vector<int>				VEC_INDEX;

protected:
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();

public:
	virtual bool			Initialize(const char*) = 0;
	virtual void			Release() = 0;
	virtual const GENERIC::Vertex*	GetVertices() = 0;
	virtual const int*		GetIndices() = 0;
	virtual const int		GetNumVertices() = 0;
	virtual const int		GetNumIndices() = 0;


	const ID3D11Buffer*		GetVB();
	const ID3D11Buffer*		GetIB();

protected:
	ID3D11Buffer*			_vertexBuffer;
	ID3D11Buffer*			_indicesBuffer;
	VEC_VERTEX				_vertices;
	VEC_INDEX				_indices;
};

