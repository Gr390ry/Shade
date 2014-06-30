#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../Management/RenderDevice.h"

namespace Component
{
	ImplementRTTI(Render, IRenderable);

	Render::Render()
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
		SAFE_RELEASE(pVB);
		SAFE_RELEASE(pIB);
		//SAFE_RELEASE(pInputLayout);
		//SAFE_DELETE_ARRAY(attributes);
		RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}

	//void Render::DrawOnScreen(const LPD3DXEFFECT _shader)
	//{
	//	Transform* transform = pOwner->GetComponent<Transform>();

	//	if (transform)
	//	{
	//		_shader->SetMatrix("WorldMatrix", &transform->GetWorldMatrix());
	//	}		
	//	_shader->CommitChanges();
	//	pMesh->DrawSubset(0);
	//}
	
	void Render::DrawFbxMesh()
	{		
	}

	void Render::LoadFbxModel(const char* filepath)
	{
		typedef fbxsdk_2015_1::FbxImporter IMPORTER;
		typedef FbxScene SCENE;

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

		this->mVertices.clear();
		this->mIndices.clear();

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

					this->mVertices.push_back(vertex);
					this->mIndices.push_back(controlPointIdx);
				}
			}
		}
		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	void Render::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex)* this->mVertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &mVertices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &pVB)))
		{
			assert(NULL);
		}
	}

	void Render::CreateIndexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(int)* mIndices.size();
		vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &mIndices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &pIB)))
		{
			assert(NULL);
		}
	}
};