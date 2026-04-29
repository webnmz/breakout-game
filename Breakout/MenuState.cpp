#include"MenuState.h"
#include"Game.h"

void MenuState::handleInput(Game& game, const sf::Event& event)
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
            m_selectedItem = (m_selectedItem - 1 + 3) % 3;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            m_selectedItem = (m_selectedItem + 1) % 3;
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (m_selectedItem == 0)
            {
                game.startNewGame();
            }
            else if (m_selectedItem == 1)
            {
				m_rulesDisplayed = !m_rulesDisplayed;  // 切换规则显示状态
            }
            else if (m_selectedItem == 2)
            {
                game.closeWindow();
            }
        }
    }
}

void MenuState::render(Game& game)
{
    game.renderMenu(m_selectedItem);
    if (m_rulesDisplayed)
        game.renderRules();
}