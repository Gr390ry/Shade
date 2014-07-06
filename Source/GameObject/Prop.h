#pragma once
#include "IGameObject.h"

namespace GameObject {

	class Prop : public IGameObject
	{
	public:
		Prop();
		~Prop();

		void				Initialize()	override;
		void				Release()		override;
		void				Update(float)	override;		
		const bool			GetActvate()	override { return mbActivated; }
	};

}; /*GameObject*/