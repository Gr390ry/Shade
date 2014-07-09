#pragma once
#include "Common.h"

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