#pragma once
#include "ShadeEngine.h"

namespace Fbx {

	class FrameNode
	{
	public:
		void Initialize();

		static void* operator new (size_t size)
		{
			void * p = _aligned_malloc(sizeof(FrameNode), 16);
			static_cast<FrameNode*>(p)->Initialize();
			return p;
		}
			static void operator delete (void* p)
		{
				static_cast<FrameNode*>(p)->~FrameNode();
				_aligned_free(p);
				p = nullptr;
		}

	public:
		FrameNode*	firstNode;
		FrameNode*	siblingNode;
		XMMATRIX	translationMatrix;
		XMMATRIX	siblingTranslationMatrix;
		XMMATRIX	previousSiblingTranslationMatrix;
		std::string name;
	};

	void FrameNode::Initialize()
	{
		firstNode = nullptr;
		siblingNode = nullptr;
		translationMatrix = XMMatrixIdentity();
		siblingTranslationMatrix = XMMatrixIdentity();
		previousSiblingTranslationMatrix = XMMatrixIdentity();
		name = "";
	}
}