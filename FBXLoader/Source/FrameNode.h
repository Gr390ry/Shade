#pragma once

class FrameNode
{
public:
	FrameNode();
	~FrameNode();

private:
	FrameNode* solbingNode;
	FrameNode* childNode;
};