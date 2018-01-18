#pragma once

#include <memory>

#include "Animation.h"
#include "Util.h"

//TODO: is this class still necessary?
class AnimationGroup
{
public:
	AnimationGroup();

	void AddAnimation(std::shared_ptr<Animation> animation);
	void Draw(sf::RenderWindow &window, float timeDelta);
	void Update(const sf::Vector2f pos);
	void Reset();

	void SetFacingDirection(MOVEMENT_DIRECTION dir);
	MOVEMENT_DIRECTION GetFacingFirection();

	std::vector<std::shared_ptr<Animation>>& GetAnimations();

	bool IsFinished();
	bool HasNextState();

	ANIMATION_STATE GetNextState();
	void SetNextState(ANIMATION_STATE state);

private:
	//TODO: get rid of the outdated m_ notation!
	std::vector<std::shared_ptr<Animation>> m_animations;
	ANIMATION_STATE m_goToState;
};

