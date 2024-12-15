#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

#include "Direction.h"
#include "Game.h"

Game::Game(sf::RenderWindow* window)
	: window_(window),
	tileEdgeLength_{ 25.f }
{
	assert(!(window_->getSize().y % (int)tileEdgeLength_) and
		!(window_->getSize().x % (int)tileEdgeLength_));
	window_->setFramerateLimit(60);

	textures_[0] = new sf::Texture();
	textures_[0]->loadFromFile("/Users/kirilli/Documents/snake-sfml/snake-sfml/background.png");

	int normilizedWidth = window_->getSize().x / (int)tileEdgeLength_;
	int normilizedHeight = window_->getSize().y / (int)tileEdgeLength_;

	field_ = new GameField(normilizedWidth, normilizedHeight,(int)tileEdgeLength_, textures_[0]);

	textures_[1] = new sf::Texture();
	textures_[1]->loadFromFile("/Users/kirilli/Documents/snake-sfml/snake-sfml/apple.png");

	food_ = new FoodSpawner(field_, textures_[1]);

	textures_[2] = new sf::Texture();
	textures_[2]->loadFromFile("/Users/kirilli/Documents/snake-sfml/snake-sfml/snake.png");

	snake_ = new Snake(field_, textures_[2]);
	
}

Game::~Game()
{
	delete snake_;
	delete food_;
	delete field_;

	for (auto pointer : textures_)
		delete pointer;
}

void Game::loop()
{
    Direction::Type directionChosen = Direction::None;
    bool gameOver = false;

    while (window_->isOpen())
    {
        sf::Event event;
        while (window_->pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window_->close();
                    break;
                
                case sf::Event::KeyPressed:
                    if (gameOver)
                    {
                        std::cout << "Restarting game" << std::endl;

                        delete snake_;
                        delete food_;

                        snake_ = new Snake(field_, textures_[2]);
                        food_ = new FoodSpawner(field_, textures_[1]);
                        
                        score_ = 0;
                        window_->setTitle(std::string("SnakeGame | Score: 0"));
                        gameOver = false;
                        directionChosen = Direction::None;
                        
                        window_->clear();
                        window_->draw(*field_->getRectToDraw());
                        window_->display();
                        
                        continue;
                    }

                    if (event.key.code >= sf::Keyboard::Left && event.key.code <= sf::Keyboard::Down)
                        directionChosen = Direction::fromSfmlKey(event.key.code);
                    break;
            }
        }

        if (gameOver)
        {
            window_->clear();
            static bool firstGameOver = true;
            if (firstGameOver)
            {
                std::cout << "Game is over" << std::endl;
                firstGameOver = false;
            }
            sf::Font font;
            font.loadFromFile("/Users/kirilli/Documents/snake-sfml/snake-sfml/tuffy.ttf");
            sf::Text text;
            text.setFont(font);
            text.setString("Game is over \npress any key to restart");
            text.setCharacterSize(35);
            text.setFillColor(sf::Color::White);

            window_->draw(text);
            window_->display();
            continue;
        }
        window_->clear();

        snake_->update(directionChosen, food_->currentPos());
        
        if (snake_->isFoodEaten())
        {
            food_->respawn();
            score_++;
            window_->setTitle(std::string("SnakeGame | Score: ") + std::to_string(score_));
            
            if (score_ + 3 == field_->tileSum())
            {
                std::cout << "The end! You won..." << std::endl;
                gameOver = true;
                continue;
            }
        }

        if (snake_->isDead())
        {
            std::cout << "The end! You are dead..." << std::endl;
            gameOver = true;
            continue;
        }

        window_->draw(*field_->getRectToDraw());

        for (auto drawable : snake_->getRectsToDraw())
            window_->draw(*drawable);

        window_->draw(*food_->getRectToDraw());

        window_->display();
    }
}
