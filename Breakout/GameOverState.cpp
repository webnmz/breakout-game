#include"GameOverState.h"
#include"Game.h"

void GameOverState::handleInput(Game& game, const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::R)
		{
			game.resetGame();
			game.startNewGame();
		}
	}
}

void GameOverState::render(Game& game)
{
	game.renderGameOver(m_win);
}