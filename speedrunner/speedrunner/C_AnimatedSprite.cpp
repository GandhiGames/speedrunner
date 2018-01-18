#include "C_AnimatedSprite.h"
#include "Object.h"

C_AnimatedSprite::C_AnimatedSprite(Object* owner) : Component(owner),
m_animated(false), m_curDirection(MOVEMENT_DIRECTION::COUNT), m_curState(ANIMATION_STATE::COUNT)
{
}

void C_AnimatedSprite::Awake()
{
	m_direction = m_owner->GetComponent<C_Direction>();

	if (!m_direction)
	{
		Debug::LogError("Cannot animate without direction. Adding Direction. To remove this warning add direction to object before this component");

		//TODO: enable ability to add components in awake function:
		//m_direction = m_owner->AddComponent<C_Direction>();
	}
}

void C_AnimatedSprite::Start()
{
	m_curDirection = m_direction->Get();
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
		MOVEMENT_DIRECTION curDir = m_direction->Get();

		if (curDir != m_curDirection)
		{
			m_curDirection = curDir;
			m_curAnimation->SetFacingDirection(curDir);
			m_curAnimation->Reset();
		}

		m_curAnimation->Update(m_owner->m_transform->GetPosition());

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

void C_AnimatedSprite::AddAnimation(ANIMATION_STATE state, std::shared_ptr<AnimationGroup> animationGroup)
{
	auto inserted = m_animations.insert(std::make_pair(state, animationGroup));

	if (!m_curAnimation)
	{
		m_curAnimation = inserted.first->second;

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
	}
}

ANIMATION_STATE C_AnimatedSprite::GetAnimationState()
{
	return m_curState;
}

std::shared_ptr<AnimationGroup> C_AnimatedSprite::GetAnimation(ANIMATION_STATE state)
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

/*
const sf::Sprite* C_AnimatedSprite::GetSprite()
{
	if (m_curAnimation)
	{
		return &m_curAnimation->m_sprite;
	}

	return nullptr;
}
*/
