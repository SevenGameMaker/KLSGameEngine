#include "KLS_Animator.h"
#include "KLS_AnimationEnd.h"

namespace KLS
{
	KLS_Animator::KLS_Animator(KLS_Animation* animation, KLS_AnimationEnd* animationend, bool loop)
	{
		m_Loop = loop;
		m_AnimationEnd = animationend;
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;
		m_AnimationTagTime = 0;
		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

		m_FrameStart = 0;
		m_FrameEnd = animation->getNumberOfFrames();
		m_FrameCurrent = 0;
	}

	void KLS_Animator::setAnimationTagTime(float time)
	{
		if (time > m_CurrentAnimation->GetDuration()) time = m_CurrentAnimation->GetDuration();
		m_AnimationTagTime = time;
	}

	void KLS_Animator::UpdateAnimation(float dt)
	{
		m_DeltaTime = dt * m_AnimationSpeed;

		if (m_CurrentAnimation)
		{
#if(1)
			m_CurrentTime += m_DeltaTime;

			// sanity checks
			if (m_FrameStart < 0) m_FrameStart = 0;
			if (m_FrameEnd > m_CurrentAnimation->getNumberOfFrames()) m_FrameEnd = m_CurrentAnimation->getNumberOfFrames();

			// set the current time to the start frame time if needed
			if (m_FrameCurrent < m_FrameStart)
				m_CurrentTime = ((float)m_FrameStart / (float)m_CurrentAnimation->getNumberOfFrames()) * m_CurrentAnimation->GetDuration();

			// calculate the current frame
			m_FrameCurrent = (int)((m_CurrentTime / m_CurrentAnimation->getNumberOfFrames()) * m_CurrentAnimation->getNumberOfFrames());

			// if we hit the trigger point then broadcast that event
			if ((m_AnimationEnd) && (m_AnimationTagTime > 0) && (m_CurrentTime > m_AnimationTagTime))
			{
				m_AnimationEnd->onAnimationTag();
			}

			// if we went past the endframe 
			if ((m_FrameCurrent >= m_FrameEnd) || (m_CurrentTime >= m_CurrentAnimation->GetDuration()))
			{
				// if we are looping then reset the currenttime back to the startframe time
				if (m_Loop)	
					m_CurrentTime = ((float)m_FrameStart / (float)m_CurrentAnimation->getNumberOfFrames()) * m_CurrentAnimation->GetDuration();
				else
				{
					// otherwise keep the currenttime as it is
					m_CurrentTime -= m_DeltaTime;
					m_AnimationEnd->onAnimationEnd();
				}
			}

			// update the bones
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));

			//KLS_ERROR("framecount = {} CurrentFrame = {}", m_CurrentAnimation->getNumberOfFrames(), m_FrameCurrent);
#else
			m_CurrentTime += dt; // m_CurrentAnimation->GetTicksPerSecond()* dt* m_AnimationSpeed;

			if ((m_TagTime > 0) && (m_CurrentTime > m_TagTime))
			{
				m_TagTime = 0;
				if (m_AnimationEnd) m_AnimationEnd->onAnimationTag();
			}

			if ((m_CurrentTime > m_CurrentAnimation->GetDuration()) && (!m_Loop))
			{
				m_CurrentTime = m_CurrentAnimation->GetDuration();
				if (m_AnimationEnd) m_AnimationEnd->onAnimationEnd();
			}
			else
			{
				m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
				CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
			}
#endif

		}
	}

	void KLS_Animator::PlayAnimation(KLS_Animation* pAnimation)
	{
		if (pAnimation != m_CurrentAnimation)
		{
			if (m_AnimationEnd) m_AnimationEnd->onAnimationChanged();
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
			m_FrameStart = 0;
			m_FrameEnd = pAnimation->getNumberOfFrames();
			m_FrameCurrent = 0;
		}
	}

	void KLS_Animator::CalculateBoneTransform(const KLS_AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		KLS_Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	std::vector<glm::mat4> KLS_Animator::GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

} // end namespace
