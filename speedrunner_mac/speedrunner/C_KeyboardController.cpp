#include "C_KeyboardController.h"
#include "Object.h"
#include "SharedContext.h"
#include "Map.h"

C_KeyboardController::C_KeyboardController(Object* owner) : Component(owner),
m_speed(150),
m_jumpForce(150.f, 300.f),
m_fallMultiplier(2.5f),
m_lowJumpMultiplier(2.f),
m_jumping(false),
m_currentJumpCount(0),
m_jumpCount(2),
m_grounded(false)
{
}

void C_KeyboardController::Awake()
{
	m_movement = m_owner->GetComponent<C_Velocity>();
	m_mapCollision = m_owner->GetComponent<C_MapCollision>();
    m_events = m_owner->GetComponent<C_EntityEvents>();
}

void C_KeyboardController::Start()
{
	float gravity = m_owner->m_context.m_map->GetGravity();
	m_movement->SetAcceleration(0.f, gravity);
    
    //TODO: this should be moved to OnEnable (need to write remove listener) and add the to Ondisable (both methods also need implementing)
    m_events->m_eventSystem.AddListener<EventCollidingBelow>(std::bind(&C_KeyboardController::OnCollidingBelow, this, std::placeholders::_1));
}

void C_KeyboardController::OnCollidingBelow(const Event& e)
{
    m_grounded = true;
    
    if(m_jumping)
    {
        m_movement->SetVelocity(0.f, 0.f);
        m_jumping = false;
        m_currentJumpCount = 0;
    }
}

void C_KeyboardController::OnNotCollidingBelow(const Event& e)
{
    m_grounded = false;
}

//TODO: keyboard controller should not have to calculate friction/gravity etc.
//Move to seperate physics class.
void C_KeyboardController::Update(float timeDelta)
{
	float gravity = m_owner->m_context.m_map->GetGravity();

    if(!m_jumping)
    {
        CalculateMovementForce(timeDelta, gravity);
    }
    
    if(m_grounded)
    {
        CalculateJumpForce(timeDelta, gravity);
    }
}

void C_KeyboardController::CalculateMovementForce(float timeDelta, float gravity)
{
	sf::Vector2f velocity = m_movement->GetVelocity();
	velocity.x = 0.f;
	if (Input::IsKeyPressed(Input::KEY::LEFT))
	{
		velocity.x = -m_speed;
	}
	else if (Input::IsKeyPressed(Input::KEY::RIGHT))
	{
		velocity.x = m_speed;
	}

	m_movement->SetVelocity(velocity);

	
	

	//TODO: implement tile and air friction
	/*
	if (m_jumping)
	{
		//TODO: load air friction in from map data file.
		const sf::Vector2f airXFriction(600.f * timeDelta, 0.f);
		m_movement->ApplyFriction(airXFriction.x, 0.f);
	}
	else
	{
		sf::Vector2f groundFriction;
		if (standingOnTile)
		{
			groundFriction = standingOnTile->m_friction * timeDelta;
		}
		else
		{
			groundFriction = m_owner->m_context.m_map->GetDefaultTile().m_friction * timeDelta;
		}

		m_movement->ApplyFriction(groundFriction.x, groundFriction.y);
	}
	*/
	//const sf::Vector2f friction = (standingOnTile) ? standingOnTile->m_friction * timeDelta : airXFriction * timeDelta;

}


//TODO: give player small amount time after walking off ledge to still be able to jump.
void C_KeyboardController::CalculateJumpForce(float timeDelta, float gravity)
{    
    if (Input::IsKeyDown(Input::KEY::UP) && m_currentJumpCount < m_jumpCount)
	{
		m_jumping = true;
        m_currentJumpCount++;
        
        float xVel = 0.f;
        if(Input::IsKeyPressed(Input::KEY::RIGHT))
        {
            xVel = m_jumpForce.x;
        }
        else if(Input::IsKeyPressed(Input::KEY::LEFT))
        {
            xVel = -m_jumpForce.x;
        }
        
        
		m_movement->SetVelocity(xVel, -m_jumpForce.y);
	}


    //Enables variable height jumps
    /*
    sf::Vector2f velocity = m_movement->GetVelocity();
	if (velocity.y > 0.f && !isGrounded)
	{
		velocity.y += gravity * (m_fallMultiplier - 1) * timeDelta;
		m_movement->SetVelocity(velocity.x, velocity.y);
	}
	else if (velocity.y < 0.f && !isGrounded && !Input::IsKeyPressed(Input::KEY::KEY_UP))
	{
		velocity.y += gravity * (m_lowJumpMultiplier - 1) * timeDelta;
		m_movement->SetVelocity(velocity.x, velocity.y);
	}
     */
}

void C_KeyboardController::SetMovementSpeed(int speed)
{
	m_speed = speed;
}
