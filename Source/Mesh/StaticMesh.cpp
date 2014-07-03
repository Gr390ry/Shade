#include "StaticMesh.h"
#include "../Management/RenderDevice.h"

void StaticMesh::Release()
{
	SAFE_DELETE_ARRAY(mVertices);
	SAFE_DELETE_ARRAY(mIndices);
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

	mContainVertices.clear();
	mContainIndices.clear();

	//FbxMesh*	pMesh = static_cast<FbxMesh*>(root->GetNodeAttribute());
	//FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	//FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	//if (pMesh->GetElementMaterial())
	//{
	//	lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
	//	lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();

	//	if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
	//	{
	//		FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
	//		if (lMaterialIndice->GetCount() == lPolygonCount)
	//		{
	//			// Count the faces of each material
	//			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
	//			{
	//				const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
	//				if (mSubMeshes.GetCount() < lMaterialIndex + 1)
	//				{
	//					mSubMeshes.Resize(lMaterialIndex + 1);
	//				}
	//				if (mSubMeshes[lMaterialIndex] == NULL)
	//				{
	//					mSubMeshes[lMaterialIndex] = new SubMesh;
	//				}
	//				mSubMeshes[lMaterialIndex]->TriangleCount += 1;
	//			}

	//			// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
	//			// if, in the loop above, we resized the mSubMeshes by more than one slot.
	//			for (int i = 0; i < mSubMeshes.GetCount(); i++)
	//			{
	//				if (mSubMeshes[i] == NULL)
	//					mSubMeshes[i] = new SubMesh;
	//			}

	//			// Record the offset (how many vertex)
	//			const int lMaterialCount = mSubMeshes.GetCount();
	//			int lOffset = 0;
	//			for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
	//			{
	//				mSubMeshes[lIndex]->IndexOffset = lOffset;
	//				lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
	//				// This will be used as counter in the following procedures, reset to zero
	//				mSubMeshes[lIndex]->TriangleCount = 0;
	//			}
	//			FBX_ASSERT(lOffset == lPolygonCount * 3);
	//		}
	//	}
	//}



	for (int i = 0; i < root->GetChildCount(); ++i)
	{
		FbxNode* child = root->GetChild(i);
		GetFBXInfo(child);
	}

	mNumVertices	= mContainVertices.size();
	mNumIndices		= mContainIndices.size();
	mVertices		= new Vertex[mNumVertices - 1];
	mIndices		= new int[mNumIndices - 1];

	for (int i = 0; i < mNumVertices; ++i)
	{
		mVertices[i] = mContainVertices[i];
	}
	for (int i = 0; i < mNumIndices; ++i)
	{
		mIndices[i] = mContainIndices[i];
	}

	mContainVertices.clear();
	mContainIndices.clear();
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
		uvSetName = uvSetList[0].Buffer();
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
				vertex.uv.y = static_cast<float>(uv.mData[1]);
			}

			vertex.position.x = static_cast<float>(vertices[controlPointIdx].mData[0]);
			vertex.position.y = static_cast<float>(vertices[controlPointIdx].mData[1]);
			vertex.position.z = static_cast<float>(vertices[controlPointIdx].mData[2]);

			mContainVertices.push_back(vertex);
			mContainIndices.push_back(controlPointIdx);
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