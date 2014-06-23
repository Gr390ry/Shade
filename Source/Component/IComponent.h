#pragma once

namespace Component {

	class IComponent
	{
	public:
		virtual void ResetComponent() = 0;
		virtual void Start() = 0;
		virtual void Update(float) = 0;
		virtual void End() = 0;
	};
	
} /*Component*/