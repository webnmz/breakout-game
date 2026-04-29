#pragma once
#include<SFML/Graphics.hpp>

class Game;

class GameState
{
public:
	GameState() {};
	virtual ~GameState() {};
	virtual void handleInput(Game& game,const sf::Event& event) = 0;
	virtual void update(Game& game,float dt) = 0;
	virtual void render(Game& game) = 0;
};

