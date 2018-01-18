#include "AnimationGroup.h"

AnimationGroup::AnimationGroup() : m_goToState(ANIMATION_STATE::COUNT) {}

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

std::vector<std::shared_ptr<Animation>>& AnimationGroup::GetAnimations()
{
	return m_animations;
}

bool AnimationGroup::IsFinished()
{
	for (auto& a : m_animations)
	{
		if (!a->IsFinished()) { return false; }
	}

	return true;
}

bool AnimationGroup::HasNextState() 
{
	return m_goToState != ANIMATION_STATE::COUNT;
}

ANIMATION_STATE AnimationGroup::GetNextState()
{
	return m_goToState;
}

void AnimationGroup::SetNextState(ANIMATION_STATE state)
{
	m_goToState = state;
}