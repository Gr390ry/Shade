#pragma once
#include "IGameObject.h"

namespace GameObject {

	class Prop : public IGameObject
	{
	public:
		Prop();
		~Prop();

		void Initialize();
		void Release();
		void Update(float);
		
		const bool GetActvate() { return mbActivated; }
	};

}; /*GameObject*/