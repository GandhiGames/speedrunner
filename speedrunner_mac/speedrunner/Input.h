#pragma once

#include <SFML/Graphics.hpp>

#include "Bitmask.h"

class Input
{
public:

	/**
	* An enum denoting all possible input keys. int values used for input bitmask.
	*/
	enum class KEY
	{
		LEFT = 1,
		RIGHT = 2,
		UP = 3,
		DOWN = 4,
		ATTACK = 5,
		ESC = 6,
		RETURN = 7
	};

	static void Update();

	static bool IsKeyPressed(KEY keycode);
	static bool IsKeyDown(KEY keycode);
	static bool IsKeyUp(KEY keycode);

private:
	static void SetPressedKeys();
	static Bitmask m_thisFrame;
	static Bitmask m_lastFrame;
};

