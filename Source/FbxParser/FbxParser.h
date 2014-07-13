#pragma once
#include "ISingleton.h"
#include "ShadeEngine.h"

namespace Fbx {

	class FrameNode;
	class FbxParser : public ISingleton<FbxParser>
	{
	private:
		bool ProcessNode(FrameNode*, FbxNode*);
		void ProcessingByNodeType(FrameNode*, FbxNode*);
		void ProcessMeshNode(FrameNode*, FbxNode*);
		void ProcessSkeletonNode(FrameNode*, FbxNode*);
		void ProcessMarkerNode(FrameNode*, FbxNode*);

	public:
		FbxParser()					= default;
		FbxParser(const FbxParser&) = delete;
		~FbxParser()				= default;

		void	Initialize();
		bool	ReadFBX(const std::string&, FrameNode*);

	private:
		FbxManager*				fbxManager;
		FbxScene*				fbxScene;
		FbxImporter*			fbxImporter;
	};

} /*Fbx*/