#include "BoxMesh.h"

void BoxMesh::Release()
{
}

bool BoxMesh::Initialize(const char* filepath = "")
{
	_vertices.clear();
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(-1, -1, -1), XMFLOAT3(-1, -1, -1), XMFLOAT2(0, 1)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(-1, 1, -1), XMFLOAT3(-1, 1, -1), XMFLOAT2(0, 0)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(1, 1, -1), XMFLOAT3(1, 1, -1), XMFLOAT2(1, 0)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(1, -1, -1), XMFLOAT3(1, -1, -1), XMFLOAT2(1, 1)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(-1, -1, 1), XMFLOAT3(-1, -1, 1), XMFLOAT2(1, 1)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(-1, 1, 1), XMFLOAT3(-1, 1, 1), XMFLOAT2(1, 0)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(1, 1, 1), XMFLOAT3(1, 1, 1), XMFLOAT2(0, 0)));
	_vertices.emplace_back(GENERIC::Vertex(XMFLOAT3(1, -1, 1), XMFLOAT3(1, -1, 1), XMFLOAT2(0, 1)));

	_indices.clear();
	//front
	_indices.emplace_back(0); _indices.emplace_back(1); _indices.emplace_back(2);
	_indices.emplace_back(0); _indices.emplace_back(2); _indices.emplace_back(3);
	//back
	_indices.emplace_back(4); _indices.emplace_back(6); _indices.emplace_back(5);
	_indices.emplace_back(4); _indices.emplace_back(7); _indices.emplace_back(6);
	//left
	_indices.emplace_back(4); _indices.emplace_back(5); _indices.emplace_back(1);
	_indices.emplace_back(4); _indices.emplace_back(1); _indices.emplace_back(0);
	//right
	_indices.emplace_back(3); _indices.emplace_back(2); _indices.emplace_back(6);
	_indices.emplace_back(3); _indices.emplace_back(6); _indices.emplace_back(7);
	//top
	_indices.emplace_back(1); _indices.emplace_back(5); _indices.emplace_back(6);
	_indices.emplace_back(1); _indices.emplace_back(6); _indices.emplace_back(2);
	//bottom
	_indices.emplace_back(4); _indices.emplace_back(0); _indices.emplace_back(3);
	_indices.emplace_back(4); _indices.emplace_back(3); _indices.emplace_back(7);

	CreateVertexBuffer();
	CreateIndexBuffer();

	Console::Get()->print("Load>Mesh>Box\n");
	return true;
}

const GENERIC::Vertex* BoxMesh::GetVertices()
{
	if (_vertices.size() == 0) return nullptr;
	return &_vertices[0];
}

const int* BoxMesh::GetIndices()
{
	if (_indices.size() == 0) return nullptr;
	return &_indices[0];
}

const int BoxMesh::GetNumVertices()
{
	return this->_vertices.size();
}

const int BoxMesh::GetNumIndices()
{
	return this->_indices.size();
}