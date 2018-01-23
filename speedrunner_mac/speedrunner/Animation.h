#pragma once

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

#include "Debug.h"
#include "Util.h"

struct FrameData
{
	int x;
	int y;
	int w;
	int h;
};

class Animation
{
public:
	Animation(std::shared_ptr<sf::Texture> texture, float frameSpeed, bool loop,
		MOVEMENT_DIRECTION facingDir);

    void OnStart();
    
	void RunAction();

	void SetPosition(const sf::Vector2f& pos);

	void Draw(sf::RenderWindow &window, float timeDelta);

	void SetFrameAction(int frame, std::function<void(void)> action);

	const sf::Sprite& GetSprite() const;

	void SetFacingDirection(MOVEMENT_DIRECTION dir);

	MOVEMENT_DIRECTION GetFacingFirection();

	void AddFrame(int x, int y, int w, int h);

	void Reset();

	bool IsFinished();

    bool IsLooped();
    
    int GetFrameCount() const;

private:
	void UpdateSpriteRect();
	void NextFrame();

private:
	sf::Sprite m_sprite;

	std::vector<FrameData> m_frames; //TODO: can increase performance by using fixed-size array
	float m_timeDelta;
	int m_initialSpriteOffset;
	float m_frameSpeed;
	int m_currentFrameIndex;
	int m_scale;
	bool m_loop;
	bool m_shouldAnimate;
	MOVEMENT_DIRECTION m_initialFacingDir;
	MOVEMENT_DIRECTION m_curFacingDir;

	std::map<int, std::function<void(void)>> m_actions;
};

