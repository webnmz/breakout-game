#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
public:
    Ball(float startX, float startY, float radius, sf::Vector2f velocity);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    // 反弹（改变速度方向）
    void bounceX();  // 水平反弹
    void bounceY();  // 垂直反弹

    // 获取/设置速度
    sf::Vector2f getVelocity() const { return m_velocity; }
    void setVelocity(sf::Vector2f v) { m_velocity = v; }

    // 重置位置（小球掉落后重新放置）
    void resetPosition(float x, float y);

    // 碰撞加速
    void increaseSpeed(float factor, float maxSpeed);

    // 掉落物减速
    void slowdownSpeed(float factor);

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;  // 速度向量（像素/秒）
};
