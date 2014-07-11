#pragma once
#include "../../Source/ShadeEngine.h"

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

class MeshInfo
{
public:
	MeshInfo();
	~MeshInfo();

private:
	std::vector<Vertex> vertices;
	std::vector<int>	indices;
	std::string			diffuseMap;
	std::string			normalMap;
	std::string			specularMap;
};