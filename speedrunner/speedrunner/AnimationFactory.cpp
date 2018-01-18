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
	animationStateLookup.emplace(std::make_pair("AttackGround", ANIMATION_STATE::ATTACK_ON_GROUND));
	animationStateLookup.emplace(std::make_pair("Walk", ANIMATION_STATE::WALK));
	animationStateLookup.emplace(std::make_pair("WallJump", ANIMATION_STATE::WALL_JUMP));

	//TODO: cache this.
	std::map<std::string, MOVEMENT_DIRECTION> moveDirLookup;
	moveDirLookup.emplace(std::make_pair("Right", MOVEMENT_DIRECTION::RIGHT));
	moveDirLookup.emplace(std::make_pair("Left", MOVEMENT_DIRECTION::LEFT));

	MOVEMENT_DIRECTION spriteDir = MOVEMENT_DIRECTION::COUNT;

	char* cstr = new char[filePath.size() + 1];
	strcpy_s(cstr, filePath.size() + 1, filePath.c_str());
	rapidxml::file<> xmlFile(cstr);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<>* rootNode = doc.first_node("TextureAtlas");

	const std::string imagePath = "../resources/spritesheets/" + std::string(rootNode->first_attribute("imagePath")->value());
	int imageWidth = std::atoi(rootNode->first_attribute("width")->value());
	int imageHeight = std::atoi(rootNode->first_attribute("height")->value());

	xml_node<>* dirNode = rootNode->first_node("direction");
	if (dirNode)
	{
		spriteDir = moveDirLookup[dirNode->value()];
	}
	if (spriteDir == MOVEMENT_DIRECTION::COUNT)
	{
		Debug::LogError("Cannot load animations as no direction set");
		return animations;
	}

	int textureId = context.m_textureManager->Add(imagePath);
	if (textureId == -1)
	{
		Debug::LogError("Texture not found");
	}


	//TODO: add error checking - what if an attribute does not exist?
	for (xml_node<> * node = rootNode->first_node("animation"); node; node = node->next_sibling())
	{
		const std::string animationName = node->first_attribute("name")->value();
		if (animationStateLookup.find(animationName) == animationStateLookup.end())
		{
			Debug::LogError("No animation state found for animation name: " + animationName);
			return animations;
		}

		ANIMATION_STATE animState = animationStateLookup[animationName];
		float frameTime = std::atof(node->first_attribute("frameTime")->value());
		bool loop = std::atoi(node->first_attribute("loop")->value());

		//(std::shared_ptr<sf::Texture> texture, float frameSpeed, bool loop, MOVEMENT_DIRECTION facingDir)
		std::shared_ptr<Animation> anim = 
			std::make_shared<Animation>(context.m_textureManager->Get(textureId), frameTime, loop, spriteDir);
		std::shared_ptr<AnimationGroup> animationGroup = std::make_shared<AnimationGroup>();
		animationGroup->AddAnimation(anim);

		xml_attribute<>* goToState = node->first_attribute("goToState");
		if (goToState)
		{
			ANIMATION_STATE state = animationStateLookup[goToState->value()];
			animationGroup->SetNextState(state);
		}

		for (xml_node<> * spriteNode = node->first_node("sprite"); spriteNode; spriteNode = spriteNode->next_sibling())
		{
			int x = std::atoi(spriteNode->first_attribute("x")->value());
			int y = std::atoi(spriteNode->first_attribute("y")->value());
			int width = std::atoi(spriteNode->first_attribute("w")->value());;
			int height = std::atoi(spriteNode->first_attribute("h")->value());;

			//TODO: implement sprite pivot point
			//float pivotX = std::atof(spriteNode->first_attribute("pX")->value());;
			//float pivotY = std::atof(spriteNode->first_attribute("pY")->value());

			anim->AddFrame(x, y, width, height);
		}

		//TODO: should check if animations already contains animation group with the same animation state
		animations.emplace(std::make_pair(animState, animationGroup));
	}

	return animations;




	/*
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

	*/

	return animations;
}
