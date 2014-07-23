#include "../IMesh.h"
#include "../Management/RenderDevice.h"

void IMesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth			= sizeof(GENERIC::Vertex) * _vertices.size();
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;
	vbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &_vertices[0];
	
	if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &_vertexBuffer)))
	{
		assert(NULL);
		Console::Get()->print("Err>Create Vertex Buffer Failed!\n");
		return;
	}
}

void IMesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage				= D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth			= sizeof(UINT) * _indices.size();
	ibd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags		= 0;
	ibd.MiscFlags			= 0;
	ibd.StructureByteStride = 0;
		 
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &_indices[0];
	
	if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&ibd, &initData, &_indicesBuffer)))
	{
		assert(NULL);
	}
}

ID3D11Buffer*		IMesh::GetVB()
{
	return	_vertexBuffer;
}
ID3D11Buffer*		IMesh::GetIB()
{
	return	_indicesBuffer;
}