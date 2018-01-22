#pragma once

#include <fstream>
#include <sstream>

#include "Debug.h"
#include "AnimationGroup.h"
#include "SharedContext.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

class AnimationFactory
{
public:
	static std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> FromFile(SharedContext& context, const std::string& filePath);
};

