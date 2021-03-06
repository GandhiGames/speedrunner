#include "Animation.h"

//TODO: current assumes sprites are facing right. May not always be the case.
Animation::Animation(std::shared_ptr<sf::Texture> texture, float frameSpeed, bool loop, MOVEMENT_DIRECTION facingDir) :
	m_frameSpeed(frameSpeed),
	m_currentFrameIndex(0),
	m_timeDelta(0.f),
	m_initialSpriteOffset(0),
	m_loop(loop),
	m_shouldAnimate(true),
	m_initialFacingDir(facingDir),
	m_curFacingDir(facingDir)
{
	m_sprite.setTexture(*texture);

	if (facingDir == MOVEMENT_DIRECTION::RIGHT)
	{
		m_scale = 1;
	}
	else
	{
		m_scale = -1;
	}
}

void Animation::RunAction()
{
	if (m_shouldAnimate)
	{
		if (m_actions.size() > 0)
		{
			//TODO: can use bit mask as quick way of checking if frame has action.
			auto actionKey = m_actions.find(m_currentFrameIndex);

			if (actionKey != m_actions.end())
			{
				actionKey->second();
			}
		}
	}
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
		// check if the frame should be updated
		if (m_timeDelta >= m_frameSpeed)
		{
			NextFrame();
			m_timeDelta = 0;
		}

		m_timeDelta += timeDelta;
	}

	window.draw(m_sprite);
}

void Animation::NextFrame()
{
	// check if we reached the last frame
	if (m_currentFrameIndex == (m_frames.size() - 1))
	{
		m_currentFrameIndex = 0;

		if (!m_loop)
		{
			m_shouldAnimate = false;
		}
	}
	else
	{
		m_currentFrameIndex++;
	}

	UpdateSpriteRect();
	RunAction();
}

void Animation::SetFacingDirection(MOVEMENT_DIRECTION dir)
{
	//TODO: minimise this check in the flip sprite pipeline
	if (m_curFacingDir == dir)
	{
		return;
	}

	m_curFacingDir = dir;

	if (m_initialFacingDir == dir) 	// Need to undo any scalling.
	{
		m_initialSpriteOffset =  0;
		m_scale = 1;
	}
	else
	{
		//TODO: this assumes all sprites have same width. This should be re-caculated when reseting/drawing new frame.
		m_initialSpriteOffset = m_frames.at(m_currentFrameIndex).w; 
		m_scale = -1;
	}
}

MOVEMENT_DIRECTION Animation::GetFacingFirection()
{
	return m_curFacingDir;
}

void Animation::AddFrame(int x, int y, int w, int h)
{
	FrameData d;
	d.x = x;
	d.y = y;
	d.w = w;
	d.h = h;

	m_frames.emplace_back(d);

	if (m_frames.size() == 1)
	{
		UpdateSpriteRect();
	}
}

void Animation::Reset()
{
	m_shouldAnimate = true;
	m_timeDelta = 0.f;
	m_currentFrameIndex = 0;
	UpdateSpriteRect();
	RunAction();
}

const sf::Sprite& Animation::GetSprite() const
{
	return m_sprite;
}

void Animation::SetFrameAction(int frame, std::function<void(void)> action)
{
	if (frame >= 0 && frame <= m_frames.size() - 1)
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

void Animation::UpdateSpriteRect()
{
	FrameData* currentFrame = &m_frames.at(m_currentFrameIndex);
	m_sprite.setTextureRect(sf::IntRect(
		currentFrame->x + m_initialSpriteOffset,
		currentFrame->y,
		currentFrame->w * m_scale,
		currentFrame->h));
}

bool Animation::IsFinished()
{
	return !m_loop && !m_shouldAnimate;
}
