// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Logger.h"

#include "KLS_Animation.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace KLS
{
	typedef class KLS_AnimationEnd KLS_AnimationEnd;

	class KLS_API KLS_Animator
	{
	public:
		KLS_Animator(KLS_Animation* animation, KLS_AnimationEnd* animationend, bool loop);
		void UpdateAnimation(float dt);
		void PlayAnimation(KLS_Animation* pAnimation);
		void CalculateBoneTransform(const KLS_AssimpNodeData* node, glm::mat4 parentTransform);
		std::vector<glm::mat4> GetFinalBoneMatrices();

		void setAnimationTagTime(float time);

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		KLS_Animation* m_CurrentAnimation;
		KLS_VARIABLE_GET(float, CurrentTime,0);
		float m_DeltaTime;
		KLS_VARIABLE_GET(KLS_AnimationEnd*, AnimationEnd,nullptr);
		KLS_VARIABLE_SETGET(bool,  Loop, true);
		KLS_VARIABLE_SETGET(float, AnimationSpeed, 1);
		KLS_VARIABLE_GET(float, AnimationTagTime, 0);

		KLS_VARIABLE_SETGET(int, FrameStart, 0);
		KLS_VARIABLE_SETGET(int, FrameEnd, 0);
		KLS_VARIABLE_SETGET(int, FrameCurrent, 0);
	};

} // end namespace