#pragma once

#include <fstream>
#include <sstream>

#include "Debug.h"
#include "AnimationGroup.h"
#include "SharedContext.h"

class AnimationFactory
{
public:
	static std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> FromFile(SharedContext& context, const std::string& filePath);
};

