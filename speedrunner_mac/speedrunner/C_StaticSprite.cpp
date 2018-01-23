#include "C_StaticSprite.h"
#include "Object.h"

//TODO: shouldn't need to set unique on both drawable and updateable
C_StaticSprite::C_StaticSprite(Object* owner) : Component(owner)
{
}

void C_StaticSprite::LateUpdate(float deltaTime)
{
	m_sprite.setPosition(m_owner->m_transform->GetPosition());
}

// Draws the object to the given render window.
void C_StaticSprite::Draw(sf::RenderWindow &window, float timeDelta)
{
	window.draw(m_sprite);
}

// Gives the object the given sprite.
void C_StaticSprite::SetSprite(sf::Texture& texture)
{
	// Create a sprite from the loaded texture.
	m_sprite.setTexture(texture);

	// Calculate frame dimensions.
	sf::Vector2u texSize = m_sprite.getTexture()->getSize();
	unsigned int width = texSize.x;
	unsigned int height = texSize.y;

	// Set the origin of the sprite.
	m_sprite.setOrigin(width / 2.f, height / 2.f);
    
    m_sprite.setPosition(m_owner->m_transform->GetPosition());
}

void C_StaticSprite::SetSprite(sf::Texture& texture, sf::IntRect rect)
{
	m_sprite.setTexture(texture);
	m_sprite.setTextureRect(rect);
	m_sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
    m_sprite.setPosition(m_owner->m_transform->GetPosition());
}

void C_StaticSprite::SetSprite(int id, sf::IntRect rect, float scaleX, float scaleY)
{
	std::shared_ptr<sf::Texture> texture = m_owner->m_context.m_textureManager->Get(id);
	if (texture)
	{
		m_sprite.setTexture(*texture);
		m_sprite.setTextureRect(rect);
		m_sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
		m_sprite.setScale(scaleX, scaleY);
        m_sprite.setPosition(m_owner->m_transform->GetPosition());
	}
}

//TODO: remove this function and provide functionality through different methods.
const sf::Sprite& C_StaticSprite::GetSprite() const
{
	return m_sprite;
}

void C_StaticSprite::SetColour(const sf::Color& colour)
{
	m_sprite.setColor(colour);
}

void C_StaticSprite::SetScale(sf::Vector2f& scale)
{
	m_sprite.setScale(scale);
}

void C_StaticSprite::Flip()
{
	const sf::IntRect& rect = m_sprite.getTextureRect();
	m_sprite.setTextureRect(sf::IntRect(
		rect.left + rect.width,
		rect.top,
		-rect.width,
		rect.height
	));
}



