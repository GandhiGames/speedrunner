#include "C_MeleeAttack.h"
#include "Object.h"
#include "Debug.h"

C_MeleeAttack::C_MeleeAttack(Object * owner) : Component(owner),
projectileForce(600.f),
dmgAmount(1),
m_projTextureID(-1),
m_projTextureRect(0, 0, 0, 0),
m_projOffset(5.f, 14.f)
{
}

void C_MeleeAttack::Awake()
{
    m_moveDir = m_owner->GetComponent<C_Direction>();
    m_animation = m_owner->GetComponent<C_AnimatedSprite>();
    m_mapCollision = m_owner->GetComponent<C_MapCollision>();
    m_velocity = m_owner->GetComponent<C_Velocity>();
}

void C_MeleeAttack::Start()
{
    m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::LEFT, sf::Vector2f(-projectileForce, 0.f)));
    m_hitDirections.insert(std::make_pair(MOVEMENT_DIRECTION::RIGHT, sf::Vector2f(projectileForce, 0.f)));
    
    
    std::shared_ptr<Animation> groundAttackAnim = m_animation->GetAnimation(ANIMATION_STATE::ATTACK_ON_GROUND);
    if (groundAttackAnim)
    {
        groundAttackAnim->SetFrameAction(0, std::bind(&C_MeleeAttack::DoAttack, this));
        
        int maxFrame = groundAttackAnim->GetFrameCount();
        groundAttackAnim->SetFrameAction(maxFrame + 1, std::bind(&C_MeleeAttack::AllowMovement, this));
    }
    
    std::shared_ptr<Animation> airAttackAnim = m_animation->GetAnimation(ANIMATION_STATE::ATTACK_IN_AIR);
    if (airAttackAnim)
    {
        airAttackAnim->SetFrameAction(0, std::bind(&C_MeleeAttack::DoAttack, this));
        
        int maxFrame = airAttackAnim->GetFrameCount();
        airAttackAnim->SetFrameAction(maxFrame + 1, std::bind(&C_MeleeAttack::AllowMovement, this));
    }
    
    
    m_projTextureID = m_owner->m_context.m_textureManager->Add(resourcePath() + "spritesheets/weapons/hand_swords.png");
    if (m_projTextureID == -1)
    {
        Debug::LogError("Weapon sprite not found");
    }
    m_projTextureRect = sf::IntRect(0, 284, 320, 47);
}

void C_MeleeAttack::Update(float deltaTime)
{
    if (Input::IsKeyDown(Input::KEY::KEY_ATTACK))
    {
        ANIMATION_STATE curState = m_animation->GetAnimationState();
        
        if(curState != ANIMATION_STATE::ATTACK_IN_AIR && curState != ANIMATION_STATE::ATTACK_ON_GROUND)
        {
            bool isGrounded = m_mapCollision->IsGrounded();
            
            ANIMATION_STATE attackState = isGrounded ? ANIMATION_STATE::ATTACK_ON_GROUND : ANIMATION_STATE::ATTACK_IN_AIR;
            
            m_animation->SetAnimationState(attackState);
            
            if(isGrounded)
            {
                const sf::Vector2f vel = m_velocity->GetVelocity();
                m_velocity->SetVelocity(0.f, vel.y);
                m_velocity->BlockX();
            }
        }
    }
}

//TODO: create generic object pool, use for projectiles.
void C_MeleeAttack::DoAttack()
{
    std::shared_ptr<Object> projectile = std::make_shared<Object>(m_owner->m_context);
    Object::Add(projectile);
    
    //TODO: if character not gounded we want projectile to match y position for n mseconds.
    projectile->m_transform->SetPosition(m_owner->m_transform->GetPosition() + m_projOffset);
    
    std::shared_ptr<C_Velocity> projVel = projectile->AddComponent<C_Velocity>();
    const MOVEMENT_DIRECTION moveDir = m_moveDir->Get();
    const sf::Vector2f& vel = m_hitDirections.at(moveDir);
    projVel->SetMaxVelocity(abs(vel.x), abs(vel.y));
    projVel->SetVelocity(vel);
    
    std::shared_ptr<C_StaticSprite> projSprite = projectile->AddComponent<C_StaticSprite>();
    projSprite->SetSprite(m_projTextureID, m_projTextureRect, 0.1f, 0.1f);
    if (moveDir == MOVEMENT_DIRECTION::LEFT)
    {
        projSprite->Flip();
    }
    projSprite->SetSortOrder(2000); //TODO: setup sort layers.
    
    
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

void C_MeleeAttack::AllowMovement()
{
    m_velocity->AllowX();
}
