// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Logger.h"

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <functional>
#include "KLS_Bone.h"
#include "KLS_AssimpHelpers.h"

#include "KLS_Bone.h"

namespace KLS
{
	typedef class KLS_AnimatedMesh KLS_AnimatedMesh;

	struct KLS_API KLS_AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<KLS_AssimpNodeData> children;
	};

	class KLS_API KLS_Animation
	{
	public:
		KLS_Animation() = default;

		KLS_Animation(const std::string& animationPath, KLS_AnimatedMesh* model);

		~KLS_Animation();

		KLS_Bone* FindBone(const std::string& name);


		inline float GetTicksPerSecond() { return (float)m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration; }
		inline const KLS_AssimpNodeData& GetRootNode() { return m_RootNode; }
		inline const std::map<std::string, KLS_BoneInfo>& GetBoneIDMap()
		{
			return m_BoneInfoMap;
		}

		void ReadMissingBones(const aiAnimation* animation, KLS_AnimatedMesh& model);

	private:
		void ReadHierarchyData(KLS_AssimpNodeData& dest, const aiNode* src);

		float m_Duration;
		int m_TicksPerSecond;
		std::vector<KLS_Bone> m_Bones;
		KLS_AssimpNodeData m_RootNode;
		std::map<std::string, KLS_BoneInfo> m_BoneInfoMap;
		KLS_VARIABLE_GET(int, NumberOfFrames, 0);
	};

} // end namespace
