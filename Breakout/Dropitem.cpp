#include"Dropitem.h"
#include"Game.h"

// Dropitem methods
Dropitem::Dropitem(float radius, const sf::Vector2f& velocity, const sf::Color& color)
{
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_velocity = velocity;
}

void Dropitem::update(float dt)
{
    // 根据速度移动
    float newX = m_shape.getPosition().x;
    float newY = m_shape.getPosition().y + m_velocity.y * dt;
    m_shape.setPosition(newX, newY);
}

void Dropitem::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Dropitem::getBounds() const
{
    return m_shape.getGlobalBounds();
}



void Dropitem::setAlive(bool a)
{
    alive = a;
}

bool Dropitem::isAlive() const
{
    return alive;
}

// Droplife methods
Droplife::Droplife(float startX,float startY) : Dropitem(10.f, sf::Vector2f(0.f, 150.f), sf::Color::Red)
{
    m_shape.setPosition(startX, startY);
}

void Droplife::onCollect(Game& game)
{
    game.addLife();
}

// DropPaddleAdd methods
DropPaddleAdd::DropPaddleAdd(float startX, float startY) : Dropitem(10.f,sf::Vector2f(0.f,150.f),sf::Color::Blue)
{
    m_shape.setPosition(startX, startY);
}

void DropPaddleAdd::onCollect(Game& game)
{
    game.addPaddle();
}

// DropBallSlow methods
DropBallSlow::DropBallSlow(float startX, float startY) : Dropitem(10.f,sf::Vector2f(0.f,150.f),sf::Color::Green)
{
    m_shape.setPosition(startX, startY);
}

void DropBallSlow::onCollect(Game& game)
{
    game.slowBall();
}