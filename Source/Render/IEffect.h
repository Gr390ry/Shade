#pragma once
#include "ShadeEngine.h"
#include "ISingleton.h"


/*
�� ����Ʈ�� ����Ʈ ���Ϻ��� �ϳ��� ����Ʈ ��ü�� �����ؾ� �Ѵ�.
�ƿ� �ν��Ͻ̶����� ������ �ٽ� ������ ����� �ϴٴ� -_-...
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