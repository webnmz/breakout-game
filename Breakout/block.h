#pragma once
#include <SFML/Graphics.hpp>

class Block
{
public:
	Block(float startX, float startY, float width, float height);
	// 在窗口中绘制挡板
	void draw(sf::RenderWindow& window) const;

	// 获取挡板的边界矩形（用于碰撞检测）
	sf::FloatRect getBounds() const;

	void setColor();

	float getCenterX() const;  //获得砖块中心点X坐标
	float getCenterY()  const;  //获得砖块中心的Y坐标

private:
	sf::RectangleShape m_shape;
};