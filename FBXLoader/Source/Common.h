#pragma once
//#include "FrameNode.h"
//#include "MeshInfo.h"
//#include <string>
//#include <fbxsdk.h>
#include <ShadeEngine.h>

class FrameNode;
class MeshInfo;

namespace FBXLoader {

	FrameNode* LoadFBX(const std::string& filepath);
	bool ProcessNode(FbxNode* pNode);
};