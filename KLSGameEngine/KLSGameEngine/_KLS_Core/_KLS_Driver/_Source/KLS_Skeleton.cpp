#include "KLS_Skeleton.h"
#include "KLS_Animation.h"
#include "KLS_Animator.h"

namespace KLS
{
	KLS_Skeleton::KLS_Skeleton()
	{
	}

	KLS_Skeleton::~KLS_Skeleton()
	{
		destroyAnimations();
		m_Mesh = nullptr;
	}

	std::vector<glm::mat4> KLS_Skeleton::updateAnimation(const double& elapsedtime)
	{
		if (m_Animator)
		{
			m_Animator->UpdateAnimation((float)elapsedtime);
			return m_Animator->GetFinalBoneMatrices();
		}
		return std::vector<glm::mat4>();
	}

	void KLS_Skeleton::setAnimationEnd(KLS_AnimationEnd* animationend)
	{
		m_AnimationEnd = animationend;
	}

	void KLS_Skeleton::setSkeletonAnimatedMesh(KLS_AnimatedMesh* mesh)
	{
		destroyAnimations();
		m_Mesh = mesh;
	}

	float KLS_Skeleton::getCurrentAnimationTime()
	{
		if (m_Animator) return m_Animator->getCurrentTime();
		return 0;
	}

	void KLS_Skeleton::setAnimationTagTime(float time)
	{
		if (m_Animator) m_Animator->setAnimationTagTime(time);
	}

	void KLS_Skeleton::setAnimationSpeed(float multiplier)
	{
		if (m_Animator) m_Animator->setAnimationSpeed(multiplier);
	}

	void KLS_Skeleton::onAnimationEnd()
	{
		if (m_AnimationEnd) m_AnimationEnd->onAnimationEnd();
	}

	void KLS_Skeleton::onAnimationChanged()
	{
		if (m_AnimationEnd) m_AnimationEnd->onAnimationChanged();
	}

	void KLS_Skeleton::onAnimationTag()
	{
		if (m_AnimationEnd) m_AnimationEnd->onAnimationTag();
	}

	void KLS_Skeleton::destroyAnimations()
	{
		for (auto& pair : m_Animations) 
		{
			delete pair.second;
		}
		m_Animations.clear();
	}

	void KLS_Skeleton::addAnimation(std::string name, std::string filename)
	{
		if (m_Mesh)
		{
			m_Animations.insert(std::make_pair(name, new KLS_Animation(filename, m_Mesh)));
		}
	}

	KLS_Animation* KLS_Skeleton::getAnimation(std::string name)
	{
		KLS_Animation* anim = m_Animations[name];
		if (anim) return anim;
		return nullptr;
	}

	void KLS_Skeleton::setAnimation(std::string name, bool loop)
	{
		KLS_Animation* anim = m_Animations[name];
		if (anim)
		{
			if (!m_Animator) m_Animator = new KLS_Animator(anim, this, loop);
			else
			{
				m_Animator->setLoop(loop);
				m_Animator->PlayAnimation(anim);
			}
		}
	}

} // end namespace
