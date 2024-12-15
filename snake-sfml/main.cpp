#include <cassert>

#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
	sf::VideoMode videoMode(500, 500);
	std::string windowTitle = "Snake";
	auto windowStyles = sf::Style::Titlebar | sf::Style::Close;

	sf::RenderWindow window(videoMode, windowTitle, windowStyles);
	window.setKeyRepeatEnabled(false);

	Game game(&window);
	game.loop();

	return 0;
}
