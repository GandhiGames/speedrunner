#include "C_AnimatedSprite.h"
#include "Object.h"

C_AnimatedSprite::C_AnimatedSprite(Object* owner) : Component(owner),
m_animated(false),
m_curState(ANIMATION_STATE::COUNT)
{
}


void C_AnimatedSprite::Update(float deltaTime)
{
	if (m_curAnimation->HasNextState() && m_curAnimation->IsFinished())
	{
		SetAnimationState(m_curAnimation->GetNextState());
	}
}

void C_AnimatedSprite::LateUpdate(float deltaTime)
{
	if (m_curAnimation)
	{
		m_curAnimation->SetPosition(m_owner->m_transform->GetPosition());
	}
}

// Draws the object to the given render window.
void C_AnimatedSprite::Draw(sf::RenderWindow &window, float timeDelta)
{
	if (m_animated && m_curAnimation)
	{
		m_curAnimation->Draw(window, timeDelta);
	}
}

void C_AnimatedSprite::AddAnimation(ANIMATION_STATE state, std::shared_ptr<Animation> animation)
{
	auto inserted = m_animations.insert(std::make_pair(state, animation));

	if (!m_curAnimation)
	{
        SetAnimationState(state);
		m_animated = true;
	}
}

void C_AnimatedSprite::SetAnimationState(ANIMATION_STATE state)
{
	if (m_curState == state)
	{
		return;
	}

	m_curState = state;
	auto animation = m_animations.find(state);
	if (animation != m_animations.end())
	{
		// Get current flip scale.
		MOVEMENT_DIRECTION curDir = MOVEMENT_DIRECTION::COUNT;
		if (m_curAnimation)
		{
			curDir = m_curAnimation->GetFacingFirection();
		}

		m_curAnimation = animation->second;

		if (curDir != MOVEMENT_DIRECTION::COUNT)
		{
			m_curAnimation->SetFacingDirection(curDir);
		}

		m_curAnimation->Reset();
        m_curAnimation->OnStart();
	}
}

void C_AnimatedSprite::SetScale(float x, float y)
{
    for(auto a : m_animations)
    {
        a.second->SetScale(x, y);
    }
}

ANIMATION_STATE C_AnimatedSprite::GetAnimationState()
{
	return m_curState;
}

std::shared_ptr<Animation> C_AnimatedSprite::GetAnimation(ANIMATION_STATE state)
{
	auto animation = m_animations.find(state);
	if (animation != m_animations.end())
	{
		return animation->second;
	}

	return nullptr;
}

void C_AnimatedSprite::SetAnimated(bool animated)
{
	if (animated && !m_animated && m_curAnimation) // if we want to be animated but we are not currently animated.
	{
		m_curAnimation->Reset();
	}

	m_animated = animated;
}

bool C_AnimatedSprite::IsAnimated()
{
	return m_animated;
}

void C_AnimatedSprite::SetDirection(MOVEMENT_DIRECTION dir)
{
    if(m_curAnimation && dir != m_curAnimation->GetFacingFirection())
    {
        m_curAnimation->SetFacingDirection(dir);
        m_curAnimation->Reset();
    }
}
