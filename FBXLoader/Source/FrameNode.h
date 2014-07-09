#pragma once
#include "Common.h"

class FrameNode
{
public:
	FrameNode();
	~FrameNode();

private:
	FrameNode*	solbingNode;
	FrameNode*	childNode;
	XMMATRIX	translationMatrix;
	XMMATRIX	SolbingTranslationMatrix;
	XMMATRIX	PreviusTranslationMatrix;
};