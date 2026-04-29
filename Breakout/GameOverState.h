#pragma once
#include"GameState.h"

class GameOverState : public GameState
{
public:
	GameOverState(bool win) : m_win(win) {};
	~GameOverState() {};
	void handleInput(Game& game, const sf::Event& event) override;
	void update(Game& game, float dt) override {};
	void render(Game& game) override;
private:
	bool m_win;  // ÊÇ·ñÊ¤Àû
};