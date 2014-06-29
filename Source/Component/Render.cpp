#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../Management/RenderDevice.h"

namespace Component
{
	ImplementRTTI(Render, IRenderable);

	Render::Attribute::Attribute() : vertices(0, 0, 0), normal(0, 0, 0), tangent(0, 0, 0), binormal(0, 0, 0), uv(0, 0), indices(0)
	{
	}

	Render::Render() : pMesh(nullptr), pShader(nullptr), pDiffuseMap(nullptr), pNormalMap(nullptr), pSpecularMap(nullptr), attributes(nullptr)
	{
	}

	Render::~Render()
	{
	}

	void Render::Initialize()
	{
		RenderDevice::Get()->AddListener(this);
	}

	void Render::Release()
	{
		SAFE_DELETE_ARRAY(attributes);
		RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}

	void Render::DrawOnScreen(const LPD3DXEFFECT _shader)
	{
		Transform* transform = pOwner->GetComponent<Transform>();

		if (transform)
		{
			_shader->SetMatrix("WorldMatrix", &transform->GetWorldMatrix());
		}		
		_shader->CommitChanges();
		pMesh->DrawSubset(0);
	}
	
	void Render::DrawFbxMesh()
	{
		if (attributes == nullptr) return;
	}

	void Render::LoadFbxModel(const char* filepath)
	{
		typedef fbxsdk_2015_1::FbxImporter IMPORTER;
		typedef FbxScene SCENE;
		typedef Render::Attribute MeshAttribute;

		FbxManager* manager = RenderDevice::Get()->GetFbxManager();
		IMPORTER *	import	= IMPORTER::Create(manager, "Model");
		SCENE *		scene	= SCENE::Create(manager, "Model");

		if (!import->Initialize(filepath, -1, manager->GetIOSettings()))
		{
			assert(NULL);
			return;
		}

		import->Import(scene);
		import->Destroy();

		FbxNode* root = scene->GetRootNode();

		if (!root)
		{
			assert(NULL);
			return;
		}

		for (int i = 0; i < root->GetChildCount(); ++i)
		{
			FbxNode* child = root->GetChild(i);

			if (child->GetNodeAttribute() == nullptr)
				continue;

			FbxNodeAttribute::EType attributeType = child->GetNodeAttribute()->GetAttributeType();

			if (attributeType != FbxNodeAttribute::eMesh) 
				continue;

			FbxMesh* mesh = static_cast<FbxMesh*>(child->GetNodeAttribute());
			FbxVector4* vertices = mesh->GetControlPoints();

			int PolygonCount = mesh->GetPolygonCount();
			attributes = new Render::Attribute[PolygonCount * 3];

			FbxVector4 normal(0, 0, 0, 1);
			FbxVector2 uv(0, 0);
			int idx = 0;
			char* uvSetName = "UVChannel_1";//{ 0, };
			bool bUnMapped;
			
			for (int polygonidx = 0; polygonidx < PolygonCount; ++polygonidx)
			{
				int numVertices = mesh->GetPolygonSize(polygonidx);
				assert(numVertices == 3);

				for (int vertexidx = 0; vertexidx < numVertices; ++vertexidx)
				{
					int controlPointIdx = mesh->GetPolygonVertex(polygonidx, vertexidx);
					idx = polygonidx * numVertices + vertexidx;

					if (mesh->GetPolygonVertexNormal(polygonidx, vertexidx, normal))
					{
						attributes[idx].normal.x = static_cast<float>(normal.mData[0]);
						attributes[idx].normal.y = static_cast<float>(normal.mData[1]);
						attributes[idx].normal.z = static_cast<float>(normal.mData[2]);
					}					
					if (mesh->GetPolygonVertexUV(polygonidx, vertexidx, uvSetName, uv, bUnMapped))
					{
						attributes[idx].uv.x = static_cast<float>(uv.mData[0]);
						attributes[idx].uv.y = static_cast<float>(uv.mData[1]);
					}

					attributes[idx].vertices.x = static_cast<float>(vertices[controlPointIdx].mData[0]);
					attributes[idx].vertices.y = static_cast<float>(vertices[controlPointIdx].mData[1]);
					attributes[idx].vertices.z = static_cast<float>(vertices[controlPointIdx].mData[2]);
					attributes[idx].indices = controlPointIdx;
				}

			}
		}
	}
};