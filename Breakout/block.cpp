#include"block.h"

// ¹¹Ôìº¯Êý
Block::Block(float startX, float startY, float width, float height)
{
	m_shape.setSize(sf::Vector2f(width, height));  
	m_shape.setFillColor(sf::Color::Blue);
	m_shape.setPosition(startX, startY);
}

void Block::draw(sf::RenderWindow& window) const
{
	window.draw(m_shape);
}

sf::FloatRect Block::getBounds() const
{
	return m_shape.getGlobalBounds();
}

void Block::setColor()
{
	m_shape.setFillColor(sf::Color::Green);
}

float Block::getCenterX() const
{
	return m_shape.getPosition().x;
}

float Block::getCenterY() const
{
	return m_shape.getPosition().y;
}