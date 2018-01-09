#include "AnimationFactory.h"

std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> AnimationFactory::FromFile(SharedContext& context, const std::string& filePath)
{
	std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> animations;

	//TODO: cache this.
	std::map<std::string, ANIMATION_STATE> animationStateLookup;
	animationStateLookup.emplace(std::make_pair("ATTACK", ANIMATION_STATE::ATTACK));
	animationStateLookup.emplace(std::make_pair("DEATH", ANIMATION_STATE::DEATH));
	animationStateLookup.emplace(std::make_pair("HURT", ANIMATION_STATE::HURT));
	animationStateLookup.emplace(std::make_pair("IDLE", ANIMATION_STATE::IDLE));
	animationStateLookup.emplace(std::make_pair("JUMP", ANIMATION_STATE::JUMP));
	animationStateLookup.emplace(std::make_pair("WALK", ANIMATION_STATE::WALK));

	std::ifstream animationDataFile;
	animationDataFile.open(filePath);
	if (!animationDataFile.is_open())
	{
		Debug::LogError("Cannot find animation data file");
		return animations;
	}

	int xSize = -1;
	int ySize = -1;
	int xScale = 1;
	int yScale = 1;
	int textureIndex = -1;

	std::string line;
	while (std::getline(animationDataFile, line))
	{
		if (line[0] == '|') { continue; }

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Texture")
		{
			std::string textureName;
			keystream >> textureName;

			textureIndex = context.m_textureManager->Add("../resources/spritesheets/" + textureName);
		}
		else if (type == "Size")
		{
			keystream >> xSize >> ySize;
		}
		else if (type == "Scale")
		{
			//TODO: not implementing scale.
			keystream >> xScale >> yScale;
		}
		else if (type == "Animation")
		{
			if (xSize < 0 || ySize < 0)
			{
				Debug::LogError("Cannot load animation as sprite size not set");
				return animations;
			}

			if (textureIndex < -1)
			{
				Debug::LogError("Cannot load animation as no texture set");
				return animations;
			}

			std::string animationName;
			int startFrame;
			int endFrame;
			int row;
			float frameTime;
			int loop;

			keystream >> animationName >> startFrame >> endFrame >> row >> frameTime >> loop;

			// Get animation state from snimation name string.

			if (animationStateLookup.find(animationName) == animationStateLookup.end())
			{
				Debug::LogError("No animation state found for animation name: " + animationName);
				return animations;
			}

			ANIMATION_STATE animState = animationStateLookup[animationName];

			std::shared_ptr<Animation> a = std::make_shared<Animation>(context.m_textureManager->Get(textureIndex), row, xSize, ySize, startFrame, endFrame, frameTime, loop);

			if (animations.find(animState) == animations.end())
			{
				std::shared_ptr<AnimationGroup> g = std::make_shared<AnimationGroup>();
				//TODO: need to get movement direction from data file.
				g->AddAnimation(MOVEMENT_DIRECTION::RIGHT, a);
				animations.emplace(std::make_pair(animState, g));
			}
			else
			{
				animations[animState]->AddAnimation(MOVEMENT_DIRECTION::RIGHT, a);
			}


		}
	}

	return animations;
}
