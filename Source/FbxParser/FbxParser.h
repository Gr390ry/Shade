#pragma once
#include "ISingleton.h"
#include "ShadeEngine.h"

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

class SubMesh
{
public:
	void Initialzie();
	void Release();

private:
};

class FbxParser : public ISingleton<FbxParser>
{
private:
	bool ProcessNode(FrameNode*, FbxNode*);
	void ProcessingByNodeType(FrameNode*, FbxNode*);
	void ProcessMeshNode(FrameNode*, FbxNode*);
	void ProcessSkeletonNode(FrameNode*, FbxNode*);
	void ProcessMarkerNode(FrameNode*, FbxNode*);

public:
	FbxParser() = default;
	FbxParser(const FbxParser&) = delete;
	~FbxParser() = default;	

	void	Initialize();
	bool	ReadFBX(const std::string&, FrameNode*);
	
private:
	FbxManager*				fbxManager;
	FbxScene*				fbxScene;
	FbxImporter*			fbxImporter;
};