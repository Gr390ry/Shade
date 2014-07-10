#include "StaticMesh.h"
#include "../Management/RenderDevice.h"
#include <Common.h>

void StaticMesh::Release()
{
	mVertices.clear();
	mIndices.clear();
}

bool StaticMesh::Initialize(const char* filename)
{
	typedef fbxsdk_2015_1::FbxImporter IMPORTER;
	typedef FbxScene SCENE;

	fbxsdk_2015_1::FbxManager* manager = RenderDevice::Get()->GetFbxManager();
	IMPORTER *	import = IMPORTER::Create(manager, "Model");
	SCENE *		scene = SCENE::Create(manager, "Model");

	if (!import->Initialize(filename, -1, manager->GetIOSettings()))
	{
		assert(NULL);
		return false;
	}

	import->Import(scene);
	import->Destroy();

	FbxNode* root = scene->GetRootNode();

	if (!root)
	{
		assert(NULL);
		return false;
	}

	mVertices.clear();
	mIndices.clear();

	for (int i = 0; i < root->GetChildCount(); ++i)
	{
		FbxNode* child = root->GetChild(i);
		GetFBXInfo(child);
	}

	Console::Get()->print("Mesh[%s] Load Complete Vertex Count=%d\n", filename, mVertices.size());
	return true;
}

void StaticMesh::GetFBXInfo(FbxNode* pNode)
{
	if (pNode == nullptr || pNode->GetNodeAttribute() == nullptr)
		return;

	FbxNodeAttribute::EType attributeType = pNode->GetNodeAttribute()->GetAttributeType();

	if (attributeType != FbxNodeAttribute::eMesh)
		return;

	FbxMesh*	mesh		= static_cast<FbxMesh*>(pNode->GetNodeAttribute());
	FbxVector4* vertices	= mesh->GetControlPoints();
	FbxVector4	normal		= FbxVector4(0, 0, 0, 1);
	FbxVector2	uv			= FbxVector2(0, 0);
	int			PolygonCount = mesh->GetPolygonCount();
	int			idx			= 0;
	char*		uvSetName	= "UVChannel_1";
	bool		bUnMapped	= false;

	{
		FbxStringList uvSetList;
		mesh->GetUVSetNames(uvSetList);
		//uvSetName = uvSetList[0].Buffer();
	}

	//Get Vertex, Index, UV, Normal Info
	for (int polygonidx = 0; polygonidx < PolygonCount; ++polygonidx)
	{
		int numVertices = mesh->GetPolygonSize(polygonidx);
		assert(numVertices == 3);

		for (int vertexidx = 0; vertexidx < numVertices; ++vertexidx)
		{
			int controlPointIdx = mesh->GetPolygonVertex(polygonidx, vertexidx);

			Vertex vertex;

			idx = polygonidx * numVertices + vertexidx;

			if (mesh->GetPolygonVertexNormal(polygonidx, vertexidx, normal))
			{
				vertex.normal.x = static_cast<float>(normal.mData[0]);
				vertex.normal.y = static_cast<float>(normal.mData[1]);
				vertex.normal.z = static_cast<float>(normal.mData[2]);
			}
			if (mesh->GetPolygonVertexUV(polygonidx, vertexidx, uvSetName, uv, bUnMapped))
			{
				vertex.uv.x = static_cast<float>(uv.mData[0]);
				vertex.uv.y = static_cast<float>(uv.mData[1]) -1;
			}

			vertex.position.x = static_cast<float>(vertices[controlPointIdx].mData[0]);
			vertex.position.y = static_cast<float>(vertices[controlPointIdx].mData[1]);
			vertex.position.z = static_cast<float>(vertices[controlPointIdx].mData[2]);
			
			mVertices.push_back(vertex);
			mIndices.push_back(controlPointIdx);
		}
	}

	const char* diffuse = fbxsdk_2015_1::FbxSurfaceMaterial::sDiffuse;

	int nMaterialCount = pNode->GetMaterialCount();
	for (int i = 0; i < nMaterialCount; ++i)
	{
		fbxsdk_2015_1::FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		if (pMaterial == nullptr) continue;
	}

	//자식 노드 검사 한다.
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		GetFBXInfo(pNode->GetChild(i));
	}
}

const IMesh::Vertex* StaticMesh::GetVertices()
{
	if (mVertices.size() == 0) return nullptr;
	return &mVertices[0];
}

const int* StaticMesh::GetIndices()
{
	if (mIndices.size() == 0) return nullptr;
	return &mIndices[0];
}

const int StaticMesh::GetNumVertices()
{
	return mVertices.size();
}

const int StaticMesh::GetNumIndices()
{
	return mIndices.size();
}