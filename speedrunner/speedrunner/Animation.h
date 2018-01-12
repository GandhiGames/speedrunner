#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <map>

#include "Util.h"

class Animation
{
public:
	Animation(std::shared_ptr<sf::Texture> texture, int row, int width,
		int height, int frameStart, int frameEnd, float frameSpeed, bool loop,
		MOVEMENT_DIRECTION facingDir);

	void SetPosition(const sf::Vector2f& pos);

	void Draw(sf::RenderWindow &window, float timeDelta);

	void NextFrame();

	void Reset();

	void SetFrameAction(int frame, std::function<void(void)> action);

	const sf::Sprite& GetSprite() const;

	void SetFacingDirection(MOVEMENT_DIRECTION dir);

	MOVEMENT_DIRECTION GetFacingFirection();

private:
	sf::Sprite m_sprite;

	float m_timeDelta;
	int m_row;
	int m_width;
	int m_height;
	int m_frameCountStart;
	int m_frameCountEnd;
	int m_initialSpriteOffset;
	float m_frameSpeed;
	int m_scale;
	int m_currentFrame;
	bool m_loop;
	bool m_shouldAnimate;
	MOVEMENT_DIRECTION m_initialFacingDir;
	MOVEMENT_DIRECTION m_curFacingDir;

	std::map<int, std::function<void(void)>> m_actions;
};

