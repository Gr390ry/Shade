#pragma once
#include <ShadeEngine.h>

class IMesh
{
	typedef std::vector<GENERIC::Vertex>	VEC_VERTEX;
	typedef std::vector<UINT>				VEC_INDEX;
	//typedef std::vector<GENERIC::InstancedData> VEC_INSTANCED;
	//typedef std::vector<XMMATRIX*>			VEC_LPMATRIX;

protected:
	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();
	//virtual void CreateInstancedBuffer();

public:
	virtual bool			Initialize(const char*) = 0;
	virtual void			Release() = 0;
	virtual const GENERIC::Vertex*	GetVertices() = 0;
	virtual const UINT*		GetIndices() = 0;
	virtual const int		GetNumVertices() = 0;
	virtual const int		GetNumIndices() = 0;

	ID3D11Buffer*			GetVB();
	ID3D11Buffer*			GetIB();

protected:
	ID3D11Buffer*			_vertexBuffer;
	ID3D11Buffer*			_indicesBuffer;
	
	VEC_VERTEX				_vertices;
	VEC_INDEX				_indices;
};

