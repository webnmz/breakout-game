#pragma once
#include"GameState.h"


class PauseMenuState : public GameState
{
public:
	PauseMenuState() {};
	~PauseMenuState() {};
	void handleInput(Game& game, const sf::Event& event) override;
	void update(Game& game, float dt) override {};
	void render(Game& game) override;
private:
	int m_selectedItem = 0;  // 菜单索引
	bool m_rulesDisplayed = false;  // 是否显示规则
};
