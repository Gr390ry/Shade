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

		virtual void Initialize();
		void Update(float);
		void Release();
		
		const bool GetActvate();
	};

};