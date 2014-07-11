#pragma once
#include "IMesh.h"

class BoxMesh : public IMesh
{
public:
	bool					Initialize(const char*)	override;
	void					Release()			override;
	const GENERIC::Vertex*	GetVertices()		override;
	const int*				GetIndices()		override;
	const int				GetNumVertices()	override;
	const int				GetNumIndices()		override;

private:
	std::vector<GENERIC::Vertex>	mVertices;
	std::vector<int>		mIndices;
};