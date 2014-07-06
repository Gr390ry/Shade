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

		void				ResetComponent()		override;
		void				Initialize()			override;
		void				Release()				override;
		void				Update(float)			override;
		const char*			GetID()					override { return "LightPoint"; };
		const EFamily::_	GetFamilyID()			override { return EFamily::System; };
		void				SetOwner(GameObject::IGameObject* owner) override { pOwner = owner; }

		const XMFLOAT3&		GetPosition() { return mPosition; }
		const XMFLOAT4&		GetColor() { return mColor; }
		const float			GetBrightness() { return mBrightness; }
		void				SetPosition(const XMFLOAT3& position) { mPosition = position; }
		void				SetColor(const XMFLOAT4& color) { mColor = color; }
		void				SetBrightness(const float& brightness) { mBrightness = brightness; }
	private:
		XMFLOAT3 mPosition;
		XMFLOAT4 mColor;
		float mBrightness;
	};

}; /*Component*/