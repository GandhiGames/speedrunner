#include "C_Direction.h"
#include "Object.h"
#include "Debug.h"

C_Direction::C_Direction(Object* owner) :
Component(owner),
m_moveDir(MOVEMENT_DIRECTION::COUNT)
{
}

void C_Direction::Awake()
{
    m_movement = m_owner->GetComponent<C_Velocity>();
    
    if (!m_movement)
    {
        Debug::LogError("Requires velocity component to calculate direction");
    }
}

//TODO: rename method to signify that calculations are performed.
MOVEMENT_DIRECTION C_Direction::Get()
{
    const sf::Vector2f& velocity = m_movement->GetVelocity();
    
    if(velocity.x != 0)
    {
        if (velocity.x <= 0)
        {
            m_moveDir = MOVEMENT_DIRECTION::LEFT;
        }
        else
        {
            m_moveDir = MOVEMENT_DIRECTION::RIGHT;
        }
    }
    
    return m_moveDir;
}

