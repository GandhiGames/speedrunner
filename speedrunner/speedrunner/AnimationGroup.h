#pragma once

#include <memory>

#include "Animation.h"
#include "Util.h"

//TODO: is this class still necessary?
class AnimationGroup
{
public:
	void AddAnimation(std::shared_ptr<Animation> animation);
	void Draw(sf::RenderWindow &window, float timeDelta);
	void Update(const sf::Vector2f pos);
	void Reset();

	void SetFacingDirection(MOVEMENT_DIRECTION dir);
	MOVEMENT_DIRECTION GetFacingFirection();
	//std::vector<std::shared_ptr<Animation>> GetAnimations(MOVEMENT_DIRECTION dir);
	//std::vector<std::shared_ptr<Animation>> GetAnimations(SPRITE_TYPE animationType);

private:
	//TODO: get rid of the outdated m_ notation!
	std::vector<std::shared_ptr<Animation>> m_animations;
};

