#pragma once
#include <SFML/Graphics.hpp>

class Game;

class  Dropitem
{
public:
	Dropitem(float radius, const sf::Vector2f& velocity, const sf::Color& color);
	
	void update(float dt);
	void draw(sf::RenderWindow& window);
	sf::FloatRect getBounds() const;
	virtual void onCollect(Game& game)=0;
	void setAlive(bool a);
	bool isAlive() const;
protected:
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
	bool alive = true;
};

class Droplife : public Dropitem
{
public:
	Droplife(float startX, float startY);
	void onCollect(Game& game) ;
};

class DropPaddleAdd : public Dropitem
{
public:
	DropPaddleAdd(float startX, float startY);
	void onCollect(Game& game);
};

class DropBallSlow : public Dropitem
{
public:
	DropBallSlow(float startX, float startY);
	void onCollect(Game& game);
};