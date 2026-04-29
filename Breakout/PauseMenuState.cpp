#include"PauseMenuState.h"
#include"Game.h"

void PauseMenuState::handleInput(Game& game, const sf::Event& event)
{
    if (m_rulesDisplayed)
    {
        if (event.type == sf::Event::KeyPressed)
        {
			m_rulesDisplayed = false;  // 任何按键都关闭规则显示   
        }
        return;
    }
	if (event.type == sf::Event::KeyPressed)
	{
        if (event.key.code == sf::Keyboard::Up)
        {
            m_selectedItem = (m_selectedItem - 1 + 4) % 4;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            m_selectedItem = (m_selectedItem + 1) % 4;
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (m_selectedItem == 0)
            {
                game.ContinueGame();
            }
            else if (m_selectedItem == 1)
            {
                game.resetGame();
                game.startNewGame();
            }
            else if (m_selectedItem == 2)
            {
				m_rulesDisplayed = !m_rulesDisplayed;  // 切换规则显示状态
            }
            else  if (m_selectedItem == 3)
            {
                game.closeWindow();
            }
        }
	}
}

void PauseMenuState::render(Game& game)
{
	game.renderPauseMenu(m_selectedItem);
    if (m_rulesDisplayed)
        game.renderRules();
}