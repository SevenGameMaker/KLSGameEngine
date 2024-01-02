// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Logger.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "KLS_AssimpHelpers.h"
#include "KLS_Bone.h"
#include "KLS_AnimationEnd.h"

namespace KLS
{
	typedef class KLS_AnimatedMesh KLS_AnimatedMesh;
	typedef class KLS_Animator KLS_Animato;
	typedef class KLS_Animation KLS_Animation;

	class KLS_API KLS_Skeleton: public KLS_AnimationEnd
	{
	private:
		KLS_VARIABLE_GET(KLS_AnimatedMesh*, Mesh, nullptr);
		KLS_VARIABLE_GET(KLS_Animator*, Animator, nullptr);
		KLS_VARIABLE_GET(KLS_AnimationEnd*, AnimationEnd, nullptr);

		std::map<std::string, KLS_BoneInfo> m_BoneInfoMap;
		int m_BoneCounter = 0;
		std::unordered_map<std::string,KLS_Animation*> m_Animations;
		
	public:
		KLS_Skeleton();
		virtual ~KLS_Skeleton();

		virtual std::vector<glm::mat4> updateAnimation(const double& elapsedtime);

		virtual void setAnimationEnd(KLS_AnimationEnd* animationend);

		auto& GetBoneInfoMap() { return m_BoneInfoMap; }
		int& GetBoneCount() { return m_BoneCounter; }

		virtual void setSkeletonAnimatedMesh(KLS_AnimatedMesh* mesh);

		KLS_Animation* getAnimation(std::string name);
		virtual void destroyAnimations();
		virtual void addAnimation(std::string name, std::string filename);
		virtual void setAnimation(std::string name, bool loop);

		virtual void setAnimationTagTime(float time);
		virtual void setAnimationSpeed(float multiplier);
		virtual float getCurrentAnimationTime();

		// animationend overrides
		virtual void onAnimationEnd();
		virtual void onAnimationChanged();
		virtual void onAnimationTag();

	};

} // end namespace