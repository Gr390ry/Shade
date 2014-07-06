#include "BoxMesh.h"

void BoxMesh::Release()
{
}

bool BoxMesh::Initialize(const char* filepath = "")
{
	mVertices.clear();
	mVertices.emplace_back(Vertex(XMFLOAT3(-1, -1, -1), XMFLOAT3(-1, -1, -1), XMFLOAT2(0, 1)));
	mVertices.emplace_back(Vertex(XMFLOAT3(-1, 1, -1), XMFLOAT3(-1, 1, -1), XMFLOAT2(0, 0)));
	mVertices.emplace_back(Vertex(XMFLOAT3(1, 1, -1), XMFLOAT3(1, 1, -1), XMFLOAT2(1, 0)));
	mVertices.emplace_back(Vertex(XMFLOAT3(1, -1, -1), XMFLOAT3(1, -1, -1), XMFLOAT2(1, 1)));
	mVertices.emplace_back(Vertex(XMFLOAT3(-1, -1, 1), XMFLOAT3(-1, -1, 1), XMFLOAT2(1, 1)));
	mVertices.emplace_back(Vertex(XMFLOAT3(-1, 1, 1), XMFLOAT3(-1, 1, 1), XMFLOAT2(1, 0)));
	mVertices.emplace_back(Vertex(XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT2(0, 0)));
	mVertices.emplace_back(Vertex(XMFLOAT3(1, -1, 1), XMFLOAT3(1, -1, 1), XMFLOAT2(0, 1)));

	mIndices.clear();
	//front
	mIndices.emplace_back(0); mIndices.emplace_back(1); mIndices.emplace_back(2);
	mIndices.emplace_back(0); mIndices.emplace_back(2); mIndices.emplace_back(3);
	//back
	mIndices.emplace_back(4); mIndices.emplace_back(6); mIndices.emplace_back(5);
	mIndices.emplace_back(4); mIndices.emplace_back(7); mIndices.emplace_back(6);
	//left
	mIndices.emplace_back(4); mIndices.emplace_back(5); mIndices.emplace_back(1);
	mIndices.emplace_back(4); mIndices.emplace_back(1); mIndices.emplace_back(0);
	//right
	mIndices.emplace_back(3); mIndices.emplace_back(2); mIndices.emplace_back(6);
	mIndices.emplace_back(3); mIndices.emplace_back(6); mIndices.emplace_back(7);
	//top
	mIndices.emplace_back(1); mIndices.emplace_back(5); mIndices.emplace_back(6);
	mIndices.emplace_back(1); mIndices.emplace_back(6); mIndices.emplace_back(2);
	//bottom
	mIndices.emplace_back(4); mIndices.emplace_back(0); mIndices.emplace_back(3);
	mIndices.emplace_back(4); mIndices.emplace_back(3); mIndices.emplace_back(7);

	Console::Get()->print("Load>Mesh>Box\n");
	return true;
}

const IMesh::Vertex* BoxMesh::GetVertices()
{
	if (mVertices.size() == 0) return nullptr;
	return &mVertices[0];
}

const int* BoxMesh::GetIndices()
{
	if (mIndices.size() == 0) return nullptr;
	return &mIndices[0];
}

const int BoxMesh::GetNumVertices()
{
	return this->mVertices.size();
}

const int BoxMesh::GetNumIndices()
{
	return this->mIndices.size();
}