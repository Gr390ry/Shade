#pragma once
#include "IMesh.h"

class SphereMesh : public IMesh
{
public:
	bool					Initialize(const char*)	override;
	void					Release()			override;
	const Vertex*			GetVertices()		override;
	const int*				GetIndices()		override;
	const int				GetNumVertices()	override;
	const int				GetNumIndices()		override;

private:
	std::vector<Vertex>		mVertices;
	std::vector<int>		mIndices;
};