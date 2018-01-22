#pragma once

#include "Component.h"
#include "C_Updateable.h"
#include "C_Direction.h"
#include "C_AnimatedSprite.h"
#include "C_MapCollision.h"
#include "Input.h"
#include "Raycast.h"

//TODO: rename class to attack? rangedattack? playerattack?
class C_MeleeAttack : public Component, public C_Updateable
{
public:
	C_MeleeAttack(Object * owner);

	void Awake() override;
	void Start() override;

	void DoAttack();

	void Update(float deltaTime) override;

private:
	std::shared_ptr<C_Direction> m_moveDir;
	std::shared_ptr<C_AnimatedSprite> m_animation;
	std::shared_ptr<C_MapCollision> m_mapCollision;
	std::map<MOVEMENT_DIRECTION, sf::Vector2f> m_hitDirections;
	float projectileForce;
	int dmgAmount;
	int m_projTextureID;
	sf::IntRect m_projTextureRect;
	sf::Vector2f m_projOffset;
};

