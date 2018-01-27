#include "Input.h"

Bitmask Input::m_thisFrame;
Bitmask Input::m_lastFrame;

void Input::Update()
{
    m_lastFrame.SetMask(m_thisFrame.GetMask());
	SetPressedKeys();
}

bool Input::IsKeyDown(KEY keycode)
{
	if (!IsKeyPressed(keycode))
	{
		return false;
	}

    return !m_lastFrame.GetBit((int)keycode);
}

bool Input::IsKeyPressed(KEY keycode)
{
    return m_thisFrame.GetBit((int)keycode);
}

bool Input::IsKeyUp(KEY keycode)
{
	if (IsKeyPressed(keycode))
	{
		return false;
	}

    return m_lastFrame.GetBit((int) keycode);
}

//TODO: simplify this mess.
void Input::SetPressedKeys()
{
	if((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -40))
    {
        m_thisFrame.TurnOnBit((int)KEY::LEFT);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::LEFT);
    }
    
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 40))
    {
        m_thisFrame.TurnOnBit((int)KEY::RIGHT);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::RIGHT);
    }
        

	 if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -40))
     {
         m_thisFrame.TurnOnBit((int)KEY::UP);
     }
    else
    {
        m_thisFrame.ClearBit((int)KEY::UP);
    }
    
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) || (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 40))
    {
        m_thisFrame.TurnOnBit((int)KEY::DOWN);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::DOWN);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        m_thisFrame.TurnOnBit((int)KEY::ATTACK);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::ATTACK);
    }

    
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        m_thisFrame.TurnOnBit((int)KEY::ESC);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::ESC);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        m_thisFrame.TurnOnBit((int)KEY::RETURN);
    }
    else
    {
        m_thisFrame.ClearBit((int)KEY::RETURN);
    }
}
