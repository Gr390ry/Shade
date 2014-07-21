#include "SphereMesh.h"

void SphereMesh::Release()
{
}

bool SphereMesh::Initialize(const char* filepath = "")
{
	_vertices.clear();
	_indices.clear();

	UINT	nSliceCount		= 24;
	float	fRadius			= 2;	
	float	phiStep			= XM_PI / nSliceCount;
	float	thetaStep		= 2.0f * XM_PI / nSliceCount;

	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(0, fRadius, 0), XMFLOAT3(0, 1, 0), XMFLOAT2(0, 0)));

	for (UINT i = 1; i <= nSliceCount - 1; ++i)
	{
		float phi			= i * phiStep;

		for (UINT j = 0; j <= nSliceCount; ++j)
		{
			float theta		= j*thetaStep;

			GENERIC::Vertex v;
			v.position.x	= fRadius * sinf(phi) * cosf(theta);
			v.position.y	= fRadius * cosf(phi);
			v.position.z	= fRadius * sinf(phi) * sinf(theta);

			XMVECTOR p		= XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.uv.x			= theta / XM_2PI;
			v.uv.y			= phi / XM_PI;

			_vertices.emplace_back(v);
		}
	}

	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(0, -fRadius, 0), XMFLOAT3(0, -1, 0), XMFLOAT2(1, 0)));

	for (UINT i = 1; i <= nSliceCount; ++i)
	{
		_indices.emplace_back(0);
		_indices.emplace_back(i + 1);
		_indices.emplace_back(i);
	}

	UINT baseIndex			= 1;
	UINT ringVertexCount	= nSliceCount + 1;

	for (UINT i = 0; i < nSliceCount - 2; ++i)
	{
		for (UINT j = 0; j < nSliceCount; ++j)
		{
			_indices.emplace_back(baseIndex + i*ringVertexCount + j);
			_indices.emplace_back(baseIndex + i*ringVertexCount + j + 1);
			_indices.emplace_back(baseIndex + (i + 1)*ringVertexCount + j);

			_indices.emplace_back(baseIndex + (i + 1)*ringVertexCount + j);
			_indices.emplace_back(baseIndex + i*ringVertexCount + j + 1);
			_indices.emplace_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = (UINT)_vertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < nSliceCount; ++i)
	{
		_indices.emplace_back(southPoleIndex);
		_indices.emplace_back(baseIndex + i);
		_indices.emplace_back(baseIndex + i + 1);
	}

	Console::Get()->print("Load>Mesh>Sphere\n");
	return true;
}

const GENERIC::Vertex* SphereMesh::GetVertices()
{
	if (_vertices.size() == 0) return nullptr;
	return &_vertices[0];
}

const int* SphereMesh::GetIndices()
{
	if (_indices.size() == 0) return nullptr;
	return &_indices[0];
}

const int SphereMesh::GetNumVertices()
{
	return this->_vertices.size();
}

const int SphereMesh::GetNumIndices()
{
	return this->_indices.size();
}