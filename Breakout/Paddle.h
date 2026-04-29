#pragma once  // 防止头文件重复包含
#include <SFML/Graphics.hpp>

class Paddle
{
public:
    // 构造函数：初始化挡板的大小、颜色、起始位置、速度
    Paddle(float startX, float startY, float width, float height, float speed);

    // 更新挡板位置（根据输入和deltaTime）
    void update(float dt);

    // 在窗口中绘制挡板
    void draw(sf::RenderWindow& window);

    // 获取挡板的边界矩形（用于碰撞检测）
    sf::FloatRect getBounds() const;

    // 更改挡板位置
    void setPosition(float x, float y);

    // 增加挡板长度
    void addSize();

    // 更改挡板长度
    void resetSize(sf::Vector2f v);

private:
    sf::RectangleShape m_shape;  // 挡板的图形对象
    float m_speed;               // 移动速度（像素/秒）
    float m_leftBoundary;        // 左边界（通常为0）
    float m_rightBoundary;       // 右边界（窗口宽度减去挡板宽度）
};