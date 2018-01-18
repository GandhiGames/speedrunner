#include "C_MeleeAttack.h"
#include "Object.h"
#include "Debug.h"

C_MeleeAttack::C_MeleeAttack(Object * owner) : Component(owner),
projectileForce(200.f), 
dmgAmount(1),
m_projTextureID(-1),
m_projTextureRect(0, 0, 0, 0)
{
}

void C_MeleeAttack::Awake()
{
	m_moveDir = m_owner->GetComponent<C_Direction>();
	m_animation = m_owner->GetComponent<C_AnimatedSprite>();
	m_mapCollision = m_owner->GetComponent<C_MapCollision>();
}

void C_MeleeAttack::Start()
{
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, sf::Vector2f(-projectileForce, 0.f)));
	m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, sf::Vector2f(projectileForce, 0.f)));


	std::shared_ptr<AnimationGroup> groundAttackAnim = m_animation->GetAnimation(ANIMATION_STATE::ATTACK_ON_GROUND);

	if (groundAttackAnim)
	{
		auto animations = groundAttackAnim->GetAnimations();

		if(animations.size() > 0) //TODO: needs to be a method of differentiating between animations in a group.
		{
			animations[0]->SetFrameAction(3, std::bind(&C_MeleeAttack::DoAttack, this));
		}
	}

	m_projTextureID = m_owner->m_context.m_textureManager->Add();
	m_projTextureRect = sf::IntRect();
}

void C_MeleeAttack::Update(float deltaTime)
{
	if (Input::IsKeyDown(Input::KEY::KEY_ATTACK))
	{
		bool isGrounded = m_mapCollision->IsGrounded();

		ANIMATION_STATE attackState = isGrounded ? ANIMATION_STATE::ATTACK_ON_GROUND : ANIMATION_STATE::ATTACK_IN_AIR;

		m_animation->SetAnimationState(attackState); 
	}
}

void C_MeleeAttack::DoAttack()
{
	std::shared_ptr<Object> projectile = std::make_shared<Object>(m_owner->m_context);
	Object::Add(projectile);

	projectile->m_transform->SetPosition(m_owner->m_transform->GetPosition());

	std::shared_ptr<C_Velocity> projVel = projectile->AddComponent<C_Velocity>();
	const MOVEMENT_DIRECTION moveDir = m_moveDir->Get();
	projVel->SetAcceleration(m_hitDirections.at(moveDir));

	std::shared_ptr<C_StaticSprite> projSprite = projectile->AddComponent<C_StaticSprite>();
	projSprite->SetSprite();

	/*
	const MOVEMENT_DIRECTION moveDir = m_moveDir->Get();
	const sf::Vector2f& pos = m_owner->m_transform->GetPosition();
	auto inRange = Raycast::CircleCast(pos + m_hitDirections.at(moveDir), hitRadius, FOLLOWER_TAG);

	for (auto i : inRange)
	{
		const sf::Vector2f& heading = i->m_transform->GetPosition() - pos;
		const float distance = Mathf::magnitude(heading);
		const sf::Vector2f& dir = heading / distance;

		auto damageables = i->GetComponents<C_Damageable>();

		for (auto& d : damageables)
		{
			d->DoDamage(dir, dmgAmount);
		}
	}
	*/
}