#include "Animation.h"

//TODO: current assumes sprites are facing right. May not always be the case.
Animation::Animation(std::shared_ptr<sf::Texture> texture, int row, int width, int height, int frameStart,
	int frameEnd, float frameSpeed, bool loop) :
	m_row(row),
	m_width(width),
	m_height(height),
	m_frameCountStart(frameStart),
	m_frameCountEnd(frameEnd),
	m_frameSpeed(frameSpeed),
	m_currentFrame(m_frameCountStart),
	m_timeDelta(0.f),
	m_initial(0),
	m_loop(loop),
	m_shouldAnimate(true),
	m_widthDir(1)
{
	m_sprite.setTexture(*texture);

	m_sprite.setOrigin(m_width / 2.f, m_height / 2.f);

	Reset();
}

void Animation::SetPosition(const sf::Vector2f& pos)
{
	m_sprite.setPosition(pos);
}

void Animation::Draw(sf::RenderWindow &window, 
	float timeDelta)
{
	if (m_shouldAnimate)
	{
		// add the elapsed time since the last draw call to the aggregate
		m_timeDelta += timeDelta;

		// check if the frame should be updated
		if (m_timeDelta >= m_frameSpeed)
		{
			NextFrame();
			m_timeDelta = 0;
		}
	}

	window.draw(m_sprite);
}

void Animation::NextFrame()
{
	// check if we reached the last frame
	if (m_currentFrame == (m_frameCountEnd - 1))
	{
		m_currentFrame = m_frameCountStart;

		if (!m_loop)
		{
			m_shouldAnimate = false;
		}
	}
	else
	{
		m_currentFrame++;
	}

	// update the texture rect
	m_sprite.setTextureRect(sf::IntRect(
		m_width * m_currentFrame + m_initial, 
		(m_row * m_height), 
		m_width * m_widthDir, 
		m_height));

	if (m_shouldAnimate)
	{
		auto actionKey = m_actions.find(m_currentFrame);

		if (actionKey != m_actions.end())
		{
			actionKey->second();
		}
	}
}

void Animation::Flip()
{
	m_initial = (m_widthDir > 0) ? m_width : 0;

	m_widthDir *= -1;
}

void Animation::Reset()
{
	m_shouldAnimate = true;
	m_timeDelta = 0.f;
	m_currentFrame = m_frameCountStart;
	m_sprite.setTextureRect(sf::IntRect(
		m_initial, 
		m_row * m_height, 
		m_width * m_widthDir,
		m_height
	));
}

const sf::Sprite& Animation::GetSprite() const
{
	return m_sprite;
}

void Animation::SetFrameAction(int frame, std::function<void(void)> action)
{
	if (frame >= m_frameCountStart && frame <= m_frameCountEnd)
	{
		auto actionKey = m_actions.find(frame);

		if (actionKey == m_actions.end())
		{
			m_actions.insert(std::make_pair(frame, action));
		}
		else
		{
			actionKey->second = action;
		}
	}
}
