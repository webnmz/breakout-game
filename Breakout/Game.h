#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<vector>
#include<memory>
#include<random>
#include "Paddle.h"
#include "Ball.h"
#include"block.h"
#include"Dropitem.h"

class GameState;

class Game
{
public:
    Game();
    ~Game();
    void run();  // 启动游戏循环
    void addLife();  // 增加生命值
    void addPaddle();  // 增长挡板
    void slowBall();  // 减速小球
    void closeWindow() { m_window.close(); }  // 关闭窗口   
	sf::RenderWindow& getWindow() { return m_window; }  // 获取窗口引用
    void changeState(std::unique_ptr<GameState> newState);  // 切换状态   
    void startNewGame();  // 开始新游戏  
	void PauseMenuGame();  // 暂停菜单游戏
    void ReturnMenuGame();  // 返回菜单
	void renderMenu(int selectItem);  // 渲染菜单
	void renderPlaying();  // 渲染游戏
	void renderPauseMenu(int selectItem);  // 渲染暂停菜单
	void renderGameOver(bool win);  // 渲染游戏结束界面
	void renderRules();  // 渲染游戏规则界面
    void resetGame();  // 重置游戏
	void ContinueGame();  // 继续游戏

	void updatePaddle(float dt);  // 更新挡板位置
	void updateBall(float dt);  // 更新小球位置
	void updateDropitems(float dt);  // 更新掉落物位置
	bool checkBallPaddleCollision();  // 检测小球与挡板的碰撞
	void bounceBallY();  // 小球垂直反弹
	void repositionBallFromPaddle();  // 碰撞后调整小球位置，防止卡入挡板
	void handleBallBlockCollisions();  // 处理小球与砖块的碰撞
	bool areBlocksEmpty();  // 检测是否所有砖块都被消除
    void winGame(bool win);  // 游戏胜利
	void endGame(bool end);  // 游戏结束
	void handleDropItemCollisions();  // 处理掉落物与挡板的碰撞
	bool isBallOutOfBottom();  // 检测小球是否超出底部边界
	void decreaseLife();  // 减少生命值
	void resetCombo() { m_combo = 0; }  // 重置连击奖励
	void setBallPosition(float x, float y) { m_ball.resetPosition(x, y); }  // 设置小球位置
	sf::FloatRect getPaddleBounds() { return m_paddle.getBounds(); }  // 获取挡板边界
	void resetBallPosition();  // 重置小球位置
	void setBallVelocity(const sf::Vector2f& velocity);  // 设置小球速度
    sf::Vector2f generateRandomDirection();  // 生成随机方向
	int getLifes() const { return m_lifes; }  // 获取生命值
	void removeDeadDropitems();  // 移除已收集的掉落物
    bool isMouseDirectionSelected() const { return m_mouseSelected; }
    sf::Vector2f getMouseDirection() const { return m_targetDirection; }
    void clearMouseDirection() { m_mouseSelected = false; } // 可选，发射后清除
	bool isLaunched() const { return m_isLaunched; }  // 获取是否已发射
	void setLaunched(bool launched) { m_isLaunched = launched; }  // 设置是否已发射
	void MouseDirectionSelect(const sf::Vector2f& mousePos);  // 选择小球发射方向
	void playHitSound();  // 播放碰撞音效

private:
    void processEvents();   // 处理窗口事件（关闭等）
	void update(float dt);  // 更新游戏逻辑
    void render();          // 渲染
    void initBlocks();  // 初始化砖块
    void updateScoreText();  // 显示得分
    void openMenu();  // 打开菜单
    void updateView(unsigned int width, unsigned int height);  // 更改视图
    void onWindowResize(unsigned int width, unsigned int height) {};
    void initDropLife(float x,float y);  // 初始化掉落物 生命
    void initDropAddPaddle(float x, float y);  // 初始化掉落物 增长挡板
    void initDropSlowBall(float x, float y);  // 初始化掉落物 减速小球
	void loadHighScore();  // 加载最高分
	void saveHighScore();  // 保存最高分
	
    sf::RenderWindow m_window;  // 窗口对象
    sf::RenderWindow m_window_menu;  // 菜单对象
    sf::Clock m_clock;          // 计时器，用于获取deltaTime
    Paddle m_paddle;  //挡板对象
    Ball m_ball;  // 小球对象
    std::vector<Block> m_blocks;  // 砖块对象
    int m_score=0;  // 分数
    const int  SCORE_PER_BLOCK = 10;  // 分值
    int m_combo = 0;  // 奖励分
    int block_nums = 0;  // 砖块数
    sf::Font m_font;  //字体设置
    sf::Text m_scoreText;  // 得分显示
    sf::Text highScoreText;  // 最高分显示
    int m_lifes = 3;  // 生命值
    sf::Text m_lifeText;  // 生命值显示
    sf::Text m_KeyboardText;  // 键位提示
    sf::View m_view;  // 视图
    sf::Texture m_heartTexture;  // 存储心形纹理
    sf::Sprite m_heartSprite;  // 实际绘制的精灵
	sf::SoundBuffer m_hitBuffer;  // 碰撞音效缓冲
	sf::Sound m_hitSound;  // 碰撞音效
    std::vector<std::unique_ptr<Dropitem>> m_dropitems;  // 掉落物
    std::mt19937 rd;  // 随机数
    sf::Vector2f m_targetDirection;  // 目标方向（单位向量）
    bool m_mouseSelected = false;    // 是否已选择方向
    bool m_isLaunched = false;   // 是否已发射
    int m_highScore;  // 最高分
    
	std::unique_ptr<GameState> m_currentState;  // 当前游戏状态
};
