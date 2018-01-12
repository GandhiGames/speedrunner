#include "C_Camera.h"
#include "Object.h"
#include "SharedContext.h"

C_Camera::C_Camera(Object* owner) : Component(owner)
{
}


C_Camera::~C_Camera()
{
}

void C_Camera::LateUpdate(float deltaTime) 
{
	SharedContext& context = m_owner->m_context;
	sf::RenderWindow* window = context.m_window;
	sf::View view = window->getView();

	const sf::Vector2f& playerPos = m_owner->m_transform->GetPosition();
	//TODO: remove hard-coding
	view.setCenter(playerPos.x, 784);

	//TODO: Needs to take into account level e.g. the camera stops at current level bounds.

	window->setView(view);
}
