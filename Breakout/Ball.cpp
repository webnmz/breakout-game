#include "Ball.h"

Ball::Ball(float startX, float startY, float radius, sf::Vector2f velocity)
    : m_velocity(velocity)
{
    m_shape.setRadius(radius);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setPosition(startX, startY);
}

void Ball::update(float dt)
{
    // 根据速度移动
    float newX = m_shape.getPosition().x + m_velocity.x * dt;
    float newY = m_shape.getPosition().y + m_velocity.y * dt;
    m_shape.setPosition(newX, newY);

    // 边界反弹（简单版本，只检查窗口边缘）
    // 窗口大小先硬编码800x600，后面优化
    float radius = m_shape.getRadius();
    float left = m_shape.getPosition().x;
    float right = left + radius * 2;
    float top = m_shape.getPosition().y;
    float bottom = top + radius * 2;

    if (left < 0) {   // 撞左墙
        m_velocity.x = -m_velocity.x;
        m_shape.setPosition(0, m_shape.getPosition().y);
    }
    if (right > 800) { // 撞右墙
        m_velocity.x = -m_velocity.x;
        m_shape.setPosition(800 - radius * 2, m_shape.getPosition().y);
    }
    if (top < 0) {    // 撞上墙
        m_velocity.y = -m_velocity.y;
        m_shape.setPosition(m_shape.getPosition().x, 0);
    }
    // 底部边界不在这里处理，因为掉到底部应该丢失一条命，而不是反弹
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Ball::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Ball::bounceX()
{
    m_velocity.x = -m_velocity.x;
}

void Ball::bounceY()
{
    m_velocity.y = -m_velocity.y;
}


void Ball::resetPosition(float x, float y)
{
    m_shape.setPosition(x, y);
}

void Ball::increaseSpeed(float factor, float maxSpeed) {
    float speedX = m_velocity.x;
    float speedY = m_velocity.y;
    // 计算当前速率
    float currentSpeed = std::sqrt(speedX * speedX + speedY * speedY);
    if (currentSpeed < maxSpeed) {
        float newSpeed = std::min(currentSpeed * factor, maxSpeed);
        // 保持方向不变，只改变大小
        float ratio = newSpeed / currentSpeed;
        m_velocity.x *= ratio;
        m_velocity.y *= ratio;
    }
}

void Ball::slowdownSpeed(float factor)
{
    float speedX = m_velocity.x;
    float speedY = m_velocity.y;
    float minSpeed = 100.f;
    // 计算当前速率
    float currentSpeed = std::sqrt(speedX * speedX + speedY * speedY);
    if (currentSpeed > minSpeed)
    {
        float newSpeed = std::max(currentSpeed * factor, minSpeed);
        float ratio = newSpeed / currentSpeed;
        m_velocity.x *= ratio;
        m_velocity.y *= ratio;
    }
}