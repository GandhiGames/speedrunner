#pragma once

#include "C_Drawable.h"
#include "C_Updateable.h"
#include "C_Transform.h"

//TODO: create animation component
class C_StaticSprite : public Component, public C_Drawable, public C_Updateable
{
public:
	C_StaticSprite(Object* owner);
	
	void Update(float deltaTime) override;

	void Draw(sf::RenderWindow &window, float timeDelta) override;

	void SetSprite(sf::Texture& texture);
	void SetSprite(sf::Texture& texture, sf::IntRect rect);
	void SetSprite(int id, sf::IntRect rect);

	const sf::Sprite& GetSprite() const;

	void SetColour(const sf::Color& colour);

	void SetScale(sf::Vector2f& scale);

private:
	sf::Sprite m_sprite;
};

