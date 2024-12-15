#ifndef SNAKE_GAME_SNAKE_DIRECTION_DIRECTION_H_
#define SNAKE_GAME_SNAKE_DIRECTION_DIRECTION_H_

#include <SFML/Window/Keyboard.hpp>

class Direction
{
public:
	enum Type
	{
		None,
		Up,
		Right,
		Down,
		Left
	};

	static Direction::Type fromSfmlKey(sf::Keyboard::Key key);
	
	static Direction::Type opposite(Direction::Type dir);

private:
	Direction() = delete;
};

Direction::Type operator-(Direction::Type dir, int num);

Direction::Type operator-(int num, Direction::Type dir);

Direction::Type operator+(Direction::Type dir, int num);

Direction::Type operator+(int num, Direction::Type dir);

#endif
