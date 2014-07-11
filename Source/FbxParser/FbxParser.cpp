#include "FbxParser.h"

void FrameNode::Initialize()
{
	firstNode				= nullptr;
	siblingNode				= nullptr;
	translationMatrix		= XMMatrixIdentity();
	siblingTranslationMatrix= XMMatrixIdentity();
	previousSiblingTranslationMatrix	= XMMatrixIdentity();
	name					= "";
}


FbxParser::FbxParser()
{
}
FbxParser::~FbxParser()
{
}

void FbxParser::Initialize()
{
	fbxManager = FbxManager::Create();

	assert(fbxManager == nullptr);

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	//Create Scene
	fbxScene		= FbxScene::Create(fbxManager, "");
	assert(fbxScene == nullptr);
	
	//Create Impoter
	fbxImporter		= FbxImporter::Create(fbxManager, "");
	assert(fbxImporter == nullptr);

	if (fbxImporter->IsFBX())
	{
		FbxIOSettings * ioSettings = fbxManager->GetIOSettings();

		ioSettings->SetBoolProp(IMP_FBX_CONSTRAINT, true);
		ioSettings->SetBoolProp(IMP_FBX_CONSTRAINT_COUNT, true);
		ioSettings->SetBoolProp(IMP_FBX_MATERIAL, true);
		ioSettings->SetBoolProp(IMP_FBX_TEXTURE, true);
		ioSettings->SetBoolProp(IMP_FBX_LINK, true);
		ioSettings->SetBoolProp(IMP_FBX_SHAPE, true);
		ioSettings->SetBoolProp(IMP_FBX_GOBO, false);
		ioSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
		ioSettings->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
}

bool FbxParser::ReadFBX(const std::string& filepath, FrameNode* out)
{
	if (fbxManager == nullptr) return false;

	FbxNode* rootNode = fbxScene->GetRootNode();
	assert(rootNode == nullptr);

	FrameNode* rootFrame = new FrameNode;

	if (!ProcessNode(rootFrame, rootNode))
	{
		Console::Get()->print("Err>ReadFBX>Node Processing failed!\n");
		return false;
	}

	out = rootFrame;

	return true;
}

bool FbxParser::ProcessNode(FrameNode* parentFrameNode, FbxNode* parentFbxNode)
{
	ProcessingByNodeType(parentFrameNode, parentFbxNode);

	std::string nodeName = parentFbxNode->GetName();
	FrameNode* currNode = new FrameNode;
	currNode->name = nodeName;
	currNode->firstNode = nullptr;
	currNode->siblingNode = nullptr;

	for (int i = 0; i < parentFbxNode->GetChildCount(); ++i)	
	{
		FbxNode* childNode = parentFbxNode->GetChild(i);
		FrameNode* childFrame = parentFrameNode->firstNode;

		if (childFrame == nullptr)
		{
			parentFrameNode->firstNode = currNode;
		}
		else// if (parentFrameNode->firstNode->siblingNode)
		{
			while (childFrame->siblingNode)
			{
				childFrame = childFrame->siblingNode;
			}
			childFrame->siblingNode = currNode;
		}

		ProcessNode(currNode, childNode);
	}

	return true;
}

void FbxParser::ProcessingByNodeType(FrameNode* frameNode, FbxNode* fbxNode)
{
	assert(frameNode == nullptr || fbxNode == nullptr);

	FbxNodeAttribute* nodeAttribute = fbxNode->GetNodeAttribute();
	FbxNodeAttribute::EType attributeType = FbxNodeAttribute::eNull;

	if (nodeAttribute)
	{
		attributeType = nodeAttribute->GetAttributeType();
	}

	if (attributeType == FbxNodeAttribute::eMarker)
	{
		ProcessMarkerNode(frameNode, fbxNode);
	}
	else if (attributeType == FbxNodeAttribute::eSkeleton)
	{
		ProcessSkeletonNode(frameNode, fbxNode);
	}
	else if (attributeType == FbxNodeAttribute::eMesh)
	{
		ProcessMeshNode(frameNode, fbxNode);
	}
}

void FbxParser::ProcessMarkerNode(FrameNode* frameNode, FbxNode* fbxNode)
{
}

void FbxParser::ProcessSkeletonNode(FrameNode* frameNode, FbxNode* fbxNode)
{
}

void FbxParser::ProcessMeshNode(FrameNode* frameNode, FbxNode* fbxNode)
{
}