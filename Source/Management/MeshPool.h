#pragma once
#include <ShadeEngine.h>
#include <ISingleton.h>

class IMesh;
class MeshPool : public ISingleton<MeshPool>
{
	typedef IMesh*							LPMESH;
	typedef std::map<std::string, LPMESH>	MAP_MESH;
	typedef std::pair<std::string, LPMESH>	COUPLE_MESH;
	typedef MAP_MESH::iterator				ITER_MESH;

private:
	void LoadScheduler();
	void AddMesh(const std::string&, const std::string&, const LPMESH);

public:
	void Initialize();
	void Release();

	LPMESH GetMeshData(const std::string&);


private:
	MAP_MESH			_mapMeshContainer;
};