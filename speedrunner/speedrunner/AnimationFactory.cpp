#include "AnimationFactory.h"
#include "SharedContext.h"
#include "ResourceManager.h"

std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> AnimationFactory::FromFile(SharedContext& context, const std::string& filePath)
{
	std::map<ANIMATION_STATE, std::shared_ptr<AnimationGroup>> animations;

	//TODO: cache this.
	std::map<std::string, ANIMATION_STATE> animationStateLookup;
	animationStateLookup.emplace(std::make_pair("Hurt", ANIMATION_STATE::HURT));
	animationStateLookup.emplace(std::make_pair("Idle", ANIMATION_STATE::IDLE));
	animationStateLookup.emplace(std::make_pair("JumpDown", ANIMATION_STATE::JUMP_DOWN));
	animationStateLookup.emplace(std::make_pair("JumpUp", ANIMATION_STATE::JUMP_UP));
	animationStateLookup.emplace(std::make_pair("AttackAir", ANIMATION_STATE::ATTACK_IN_AIR));
	animationStateLookup.emplace(std::make_pair("AttackAir", ANIMATION_STATE::ATTACK_ON_GROUND));
	animationStateLookup.emplace(std::make_pair("Walk", ANIMATION_STATE::WALK));
	animationStateLookup.emplace(std::make_pair("WallJump", ANIMATION_STATE::WALL_JUMP));

	//TODO: cache this.
	std::map<std::string, MOVEMENT_DIRECTION> moveDirLookup;
	moveDirLookup.emplace(std::make_pair("Right", MOVEMENT_DIRECTION::RIGHT));
	moveDirLookup.emplace(std::make_pair("Left", MOVEMENT_DIRECTION::LEFT));




	return;

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
	MOVEMENT_DIRECTION spriteDir = MOVEMENT_DIRECTION::COUNT;

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
		else if (type == "Direction")
		{
			std::string dir;
			keystream >> dir;
			
			if (moveDirLookup.find(dir) == moveDirLookup.end())
			{
				Debug::LogError("Sprite dir not found in lookup, cannot build animation for: " + dir);
				return animations;
			}

			spriteDir = moveDirLookup[dir];
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

			if (spriteDir == MOVEMENT_DIRECTION::COUNT)
			{
				Debug::LogError("Cannot load animations as no direction set");
				return animations;
			}

			std::string animationName;
			int startFrame;
			int endFrame;
			int row;
			float frameTime;
			int loop;

			keystream >> animationName >> startFrame >> endFrame >> row >> frameTime >> loop;

			// Get animation state from animation name string.

			if (animationStateLookup.find(animationName) == animationStateLookup.end())
			{
				Debug::LogError("No animation state found for animation name: " + animationName);
				return animations;
			}

			ANIMATION_STATE animState = animationStateLookup[animationName];

			std::shared_ptr<Animation> a = 
				std::make_shared<Animation>(context.m_textureManager->Get(textureIndex), row, 
					xSize, ySize, startFrame, endFrame, frameTime, 
					loop, spriteDir);

			if (animations.find(animState) == animations.end())
			{
				std::shared_ptr<AnimationGroup> g = std::make_shared<AnimationGroup>();
				//TODO: need to get movement direction from data file.
				g->AddAnimation(a);
				animations.emplace(std::make_pair(animState, g));
			}
			else
			{
				animations[animState]->AddAnimation(a);
			}


		}
	}

	return animations;
}
