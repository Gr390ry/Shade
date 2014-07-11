#pragma once
//#include <xnamath.h>
#include "../../Source/ShadeEngine.h"


class MeshInfo;
class FrameNode
{
public:
	FrameNode() { Initialize(); }
	~FrameNode() {}

	void Initialize()
	{
		solbingNode		= nullptr;
		FirstNode		= nullptr;
		meshInfo		= nullptr;
	}

	static void* operator new (size_t size)
	{
		void* p = _aligned_malloc(sizeof(FrameNode), 16);
		return p;
	}
	static void operator delete (void* p)
	{
		static_cast<FrameNode*>(p)->~FrameNode();
		_aligned_free(p);
		p = nullptr;
	}

private:
	FrameNode*	solbingNode;
	FrameNode*	FirstNode;

	XMMATRIX	translationMatrix;
	XMMATRIX	SolbingTranslationMatrix;
	XMMATRIX	PreviusTranslationMatrix;

	MeshInfo*	meshInfo;
};