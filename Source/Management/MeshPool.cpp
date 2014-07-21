#include "MeshPool.h"
#include "../Render/IMesh.h"
#include "../Render/Mesh/BoxMesh.h"


void MeshPool::Initialize()
{
	AddMesh("Box", "", new BoxMesh());
}

void MeshPool::Release()
{
	for (COUPLE_MESH coupler : _mapMeshContainer)
	{
		LPMESH mesh = coupler.second;

		if (mesh == nullptr) continue;
		mesh->Release();
	}
	_mapMeshContainer.clear();
}

void MeshPool::LoadScheduler()
{
}

void MeshPool::AddMesh(const std::string& meshID, const std::string& filepath, const LPMESH mesh)
{
	assert(meshID == "");
	assert(mesh == nullptr);

	if (_mapMeshContainer.find(meshID) != _mapMeshContainer.end())
	{
		assert(mesh->Initialize(filepath.c_str()));
		_mapMeshContainer.emplace(meshID, mesh);
	}
}

MeshPool::LPMESH MeshPool::GetMeshData(const std::string& meshID)
{
	assert(meshID == "");
	ITER_MESH iter = _mapMeshContainer.find(meshID);

	if (iter == _mapMeshContainer.end())
	{
		return nullptr;
	}
	return (*iter).second;
}