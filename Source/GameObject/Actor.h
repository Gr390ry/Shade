#pragma once
#include "IGameObject.h"
//#include <vector>

namespace Component {
	class Render;
	class Transform;
}

namespace GameObject {
	
	class Actor : public IGameObject
	{
	public:
		Actor();
		~Actor();

		virtual void		Initialize()	override;
		void				Update(float)	override;
		void				Release()		override;
		const bool			GetActvate()	override;

		const Component::Transform* GetTransform();
		const Component::Render*	GetRender();
	};

};