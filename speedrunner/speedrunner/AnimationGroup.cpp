#include "AnimationGroup.h"


void AnimationGroup::AddAnimation(std::shared_ptr<Animation> animation)
{
	m_animations.emplace_back(animation);
}

void AnimationGroup::SetFacingDirection(MOVEMENT_DIRECTION dir)
{
	for (auto& a : m_animations)
	{
		a->SetFacingDirection(dir);
	}
}

//TODO: convert this to facing direction
MOVEMENT_DIRECTION AnimationGroup::GetFacingFirection()
{
	return (m_animations.size() > 0) ? m_animations.at(0)->GetFacingFirection() : MOVEMENT_DIRECTION::COUNT;
}

void AnimationGroup::Draw(sf::RenderWindow &window, float timeDelta)
{
	for (auto& a : m_animations)
	{
		a->Draw(window, timeDelta);
	}
}

void AnimationGroup::Update(const sf::Vector2f pos)
{
	for (auto& a : m_animations)
	{
		a->SetPosition(pos);
	}
}

void AnimationGroup::Reset()
{
	for (auto& a : m_animations)
	{
		a->Reset();
	}
}

/*
std::vector<std::shared_ptr<Animation>> AnimationGroup::GetAnimations(MOVEMENT_DIRECTION dir)
{
	auto animationGroup = m_animations.find(dir);

	if (animationGroup != m_animations.end())
	{
		return animationGroup->second;
	}

	return std::vector<std::shared_ptr<Animation>>();
}

std::vector<std::shared_ptr<Animation>> AnimationGroup::GetAnimations(SPRITE_TYPE animationType)
{
	std::vector<std::shared_ptr<Animation>> animations;

	for (auto& animationsGroup : m_animations)
	{
		for (auto& animation : animationsGroup.second)
		{
			if (animation->GetType() == animationType)
			{
				animations.push_back(animation);
			}
		}
	}

	return animations;
}
*/