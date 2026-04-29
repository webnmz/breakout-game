#include "Paddle.h"

// 构造函数
Paddle::Paddle(float startX, float startY, float width, float height, float speed)
    : m_speed(speed)
{
    m_shape.setSize(sf::Vector2f(width, height));   // 设置大小
    m_shape.setFillColor(sf::Color::White);         // 颜色白色
    m_shape.setPosition(startX, startY);            // 初始位置
}

void Paddle::update(float dt)
{
    // 获取当前挡板位置
    float x = m_shape.getPosition().x;

    // 左右箭头输入（实时检测）
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        x -= m_speed * dt;   // 向左移动
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        x += m_speed * dt;   // 向右移动

    // 边界限制（不能超出左右边界）
    // 左边界为0，右边界为窗口宽度减去挡板宽度
    // 注意：这里m_rightBoundary需要在外部设置，我们暂时用窗口宽度减去自身宽度计算
    // 但为了简单，我们假设窗口宽度为800，挡板宽100，那么右边界就是700
    // 实际开发中可以在Game类中传入窗口边界，这里先硬编码一个示例
    // 我们将在Game类中调用getBounds并设置边界，这里先不做复杂处理
    // 简化版：只限制在0到800-100=700之间（假设窗口800，挡板100）
    // 为了让代码能直接运行，我们先硬编码，稍后在Game中会动态设置
    if (x < 0) x = 0;
    if (x > 800 - m_shape.getSize().x) x = 800 - m_shape.getSize().x;
    m_shape.setPosition(x, m_shape.getPosition().y);
}

void Paddle::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Paddle::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Paddle::setPosition(float x, float y)
{
    m_shape.setPosition(x, y);
}

void Paddle::addSize()
{
    // 每次增加10
    float m_size = m_shape.getSize().x + 10.f;
    m_shape.setSize(sf::Vector2f(m_size, m_shape.getSize().y));
}

void Paddle::resetSize(sf::Vector2f v)
{
    m_shape.setSize(v);
}