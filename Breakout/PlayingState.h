#pragma once
#include"GameState.h"

class PlayingState : public GameState
{
public:
	PlayingState() {};
	~PlayingState() {};
	void handleInput(Game& game, const sf::Event& event) override;
	void update(Game& game, float dt) override;
	void render(Game& game) override;
};