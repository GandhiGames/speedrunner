#pragma once

#include <array>
#include <iostream>
#include <functional>
#include <SFML\Graphics.hpp>

#include "Raycast.h"

class Debug
{
	friend class SharedContext;
public:
	static void Initialise(SharedContext& context);

	static void Draw(sf::RenderWindow& window);

	static void DrawLine(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour = sf::Color::White);
	static void DrawRay(const sf::Vector2f& from, const sf::Vector2f& to, sf::Color colour = sf::Color::White);
	static void DrawRect(const sf::Vector2f& pos, const sf::Vector2f size, sf::Color colour = sf::Color::White);

	static void Log(const sf::Vector2f& v);
	static void Log(const std::string& msg);
	static void LogWarning(const std::string& msg);
	static void LogError(const std::string& msg);

	static void ToggleGridCoordinates(bool draw);

	static void Debug::TimeFunction(std::function<void()> func);

	//TODO: implement function timer for performance measurement see math_engine

private:
	static std::vector<std::array<sf::Vertex, 2>> m_lines;
	static std::vector<sf::RectangleShape> m_rects;

	static bool m_drawGridCoordinates;

	static sf::Font m_debugFont;

	static std::vector<sf::Text> m_coordinateTexts;
};

