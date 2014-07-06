#include "SphereMesh.h"

void SphereMesh::Release()
{
}

bool SphereMesh::Initialize(const char* filepath = "")
{
	mVertices.clear();
	mIndices.clear();

	UINT	nSliceCount = 24;
	float	fRadius		= 2;	
	float	phiStep		= XM_PI / nSliceCount;
	float	thetaStep	= 2.0f * XM_PI / nSliceCount;

	mVertices.emplace_back(Vertex(XMFLOAT3(0, fRadius, 0), XMFLOAT3(0, 1, 0), XMFLOAT2(0, 0)));	

	for (UINT i = 1; i <= nSliceCount - 1; ++i)
	{
		float phi		= i * phiStep;

		for (UINT j = 0; j <= nSliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;
			v.position.x = fRadius * sinf(phi) * cosf(theta);
			v.position.y = fRadius * cosf(phi);
			v.position.z = fRadius * sinf(phi) * sinf(theta);

			XMVECTOR p = XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.uv.x = theta / XM_2PI;
			v.uv.y = phi / XM_PI;

			mVertices.emplace_back(v);
		}
	}

	mVertices.emplace_back(Vertex(XMFLOAT3(0, -fRadius, 0), XMFLOAT3(0, -1, 0), XMFLOAT2(1, 0)));

	for (UINT i = 1; i <= nSliceCount; ++i)
	{
		mIndices.push_back(0);
		mIndices.push_back(i + 1);
		mIndices.push_back(i);
	}

	UINT baseIndex			= 1;
	UINT ringVertexCount	= nSliceCount + 1;

	for (UINT i = 0; i < nSliceCount - 2; ++i)
	{
		for (UINT j = 0; j < nSliceCount; ++j)
		{
			mIndices.push_back(baseIndex + i*ringVertexCount + j);
			mIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			mIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			mIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	UINT southPoleIndex = (UINT)mVertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < nSliceCount; ++i)
	{
		mIndices.push_back(southPoleIndex);
		mIndices.push_back(baseIndex + i);
		mIndices.push_back(baseIndex + i + 1);
	}

	Console::Get()->print("Load>Mesh>Sphere\n");
	return true;
}

const IMesh::Vertex* SphereMesh::GetVertices()
{
	if (mVertices.size() == 0) return nullptr;
	return &mVertices[0];
}

const int* SphereMesh::GetIndices()
{
	if (mIndices.size() == 0) return nullptr;
	return &mIndices[0];
}

const int SphereMesh::GetNumVertices()
{
	return this->mVertices.size();
}

const int SphereMesh::GetNumIndices()
{
	return this->mIndices.size();
}