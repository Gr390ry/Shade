#include "Actor.h"
#include "ConstructHelper.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"
#include "../Render/Mesh/BoxMesh.h"
#include "../Render/Mesh/SphereMesh.h"

namespace GameObject {

	Actor::Actor()
	{
	}

	Actor::~Actor()
	{
		Release();
	}

	void Actor::Initialize()
	{
		this->AddComponent<Component::Render>();
		this->AddComponent<Component::Transform>();		
		/*Component::Transform* transform = GetComponent<Component::Transform>();
		Component::Render* render = GetComponent<Component::Render>();*/
		//render->LoadFbxModel("Contents/Model/darkSider2/DarSider.fbx");
		//BoxMesh* pBox = new BoxMesh;
		/*SphereMesh *pSphere = new SphereMesh;
		pSphere->Initialize("");
		render->SetMeshData(pSphere);*/
	}

	void Actor::Update(float pDelta)
	{
		Component::Transform* transform = GetComponent<Component::Transform>();

		ITER_COMPONENT iter = ContainComponents.begin();
		int nSize = ContainComponents.size();
		concurrency::parallel_for(0, nSize, [&](int n)
		{
			iter->second->Update(pDelta);
			++iter;
		});
	}

	void Actor::Release()
	{
		RemoveAllComponents();
	}

	const bool Actor::GetActvate()
	{
		return mbActivated;
	}
	const Component::Transform* Actor::GetTransform()
	{
		return GetComponent<Component::Transform>();
	}
	const Component::Render* Actor::GetRender()
	{
		return GetComponent<Component::Render>();
	}
}