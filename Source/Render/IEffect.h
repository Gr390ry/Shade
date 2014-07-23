#pragma once
#include "ShadeEngine.h"
#include "ISingleton.h"


/*
각 이펙트는 이펙트 파일별로 하나의 이펙트 객체만 생성해야 한다.
아오 인스턴싱때문에 구조를 다시 뒤집어 엎어야 하다니 -_-...
*/

namespace Render { namespace Effect {

	class IEffect
	{
	private:
		virtual void SetInputLayout() = 0;

	public:
		IEffect();
		IEffect(IEffect& ref) = delete;
		virtual ~IEffect();

		
		virtual void Initialize() = 0;
		virtual void Release() = 0;

		bool LoadEffect(const std::string&);
		const LPD3D11EFFECT GetFx();
	protected:
		LPD3D11EFFECT	_fx;
	};

} /*Effect*/ } /*Render*/