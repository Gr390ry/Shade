#pragma once
#include "../ShadeEngine.h"

class IMesh
{
	typedef std::vector<GENERIC::Vertex>	VEC_VERTEX;
	typedef std::vector<int>				VEC_INDEX;

private:
	virtual void CreateBuffers();

public:
	virtual bool			Initialize(const char*) = 0;
	virtual void			Release() = 0;
	virtual const GENERIC::Vertex*	GetVertices() = 0;
	virtual const int*		GetIndices() = 0;
	virtual const int		GetNumVertices() = 0;
	virtual const int		GetNumIndices() = 0;


	const ID3D11Buffer*		GetVB()
	{
		return	_vertexBuffer;
	}
	const ID3D11Buffer*		GetIB()
	{
		return	_indicesBuffer;
	}

private:
	ID3D11Buffer*			_vertexBuffer;
	ID3D11Buffer*			_indicesBuffer;
	VEC_VERTEX				_vertices;
	VEC_INDEX				_indices;
};

void IMesh::CreateBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth			= sizeof(GENERIC::Vertex) * _vertices.size();
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &_vertices;

	if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &_vertexBuffer)))
	{
		assert(NULL);
		Console::Get()->print("Err>Create Vertex Buffer Failed!\n");
		return;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage				= D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth			= sizeof(int) * _indices.size();
	ibd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags		= 0;
	ibd.MiscFlags			= 0;
	ibd.StructureByteStride = 0;
	 
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &_indices;

	if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&ibd, &initData, &_indicesBuffer)))
	{
		assert(NULL);
	}
}