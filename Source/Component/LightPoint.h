#pragma once
#include "IRenderable.h"
#include "../ShadeEngine.h"

namespace Component {

	class LightPoint : public IRenderable
	{
		DeclareRTTI(LightPoint);

	public:
		LightPoint();
		~LightPoint();

		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);

		const char* GetID() { return "LightPoint"; };
		const EFamily::_ GetFamilyID() { return EFamily::System; };
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }

		const Vector3& GetPosition() { return mPosition; }
		const Vector4& GetColor() { return mColor; }
		const float GetBrightness() { return mBrightness; }
		void SetPosition(const Vector3& position) { mPosition = position; }
		void SetColor(const Vector4& color) { mColor = color; }
		void SetBrightness(const float& brightness) { mBrightness = brightness; }
	private:
		Vector3 mPosition;
		Vector4 mColor;
		float mBrightness;
	};

}; /*Component*/