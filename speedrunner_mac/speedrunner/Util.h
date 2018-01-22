#pragma once

#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>

enum class ANIMATION_STATE 
{
	WALK,
	IDLE,
	JUMP_DOWN,
	JUMP_UP,
	ATTACK_IN_AIR,
	ATTACK_ON_GROUND,
	HURT,
	DEATH,
	WALL_JUMP,
	COUNT
};

enum class MOVEMENT_DIRECTION
{
	LEFT,
	RIGHT,
	COUNT
};

const std::string PLAYER_TAG = "Player";
const std::string FOLLOWER_TAG = "Follower";
const std::string ENEMY_TAG = "Follower";

const std::string BODY_PARTS_ROOT_FOLDER = "../resources/characters/body_parts/";

class Util
{
public:

	static inline bool IsInteger(const std::string & s)
	{
		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

		char * p;
		strtol(s.c_str(), &p, 10);

		return (*p == 0);
	}
};

//TODO: move to own header
class Mathf
{
public:
	/*
	* Math Helper Functions
	*/
	static inline sf::Vector2f normalize(sf::Vector2f v)
	{
		float l = magnitude(v);

		if (l != 0)
		{
			return v / l;
		}

		return v;
	}

	static inline sf::Vector2f normalize(sf::Vector2f v, float l)
	{
		if (l > 0)
		{
			return v / l;
		}

		return v;
	}

	static inline void swap(float& a, float& b)
	{
		float c = a;
		a = b;
		b = c;
	}

	static inline float sqrMagnitude(sf::Vector2f& v)
	{
		return v.x*v.x + v.y*v.y;
	}

	// Length
	static inline float magnitude(const sf::Vector2f& a)
	{
		return sqrt((a.x * a.x) + (a.y * a.y));
	}

	static inline float distance(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return (abs(sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)))));
	}

	static inline float distanceSqr(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return abs((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	}

	static inline void truncate(sf::Vector2f& v, float max)
	{
		float sqrLength = sqrMagnitude(v);
		if (sqrLength > (max * max))
		{
			v = normalize(v);
			v *= max;
		}
	}

	// Dot product
	static inline float dotProduct(const sf::Vector2f v1, const sf::Vector2f v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}

	static inline float invert(sf::Vector2f& v)
	{
		v.x = -v.x;
		v.y = -v.y;
	}

	static inline sf::Vector2f componentProduct(const sf::Vector2f v1, const sf::Vector2f v2)
	{
		return sf::Vector2f(v1.x * v2.x, v1.y * v2.y);
	}

	static inline int to1DIndex(int x, int y, int width)
	{
		//return y * width + x;
		return (x * width) + y;
	}

	static inline float RandomValue()
	{
		return (float)rand() / (RAND_MAX);
	}
};

