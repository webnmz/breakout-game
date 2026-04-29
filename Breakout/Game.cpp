#include <iostream>
#include<algorithm>
#include <fstream>
#include "Game.h"
#include"Constants.h"
#include"GameState.h"
#include"MenuState.h"
#include"PlayingState.h"
#include"PauseMenuState.h"
#include"GameOverState.h"


Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"打砖块 - Breakout",sf::Style::Close|sf::Style::Resize)
    , m_paddle(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 30, PADDLE_WIDTH, PADDLE_HEIGHT, 400.0f)  // 挡板起始x: 中间, y: 底部上方30
    , m_ball(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT - 50, 10.0f, sf::Vector2f(200.0f, -200.0f))
{
	m_currentState = std::make_unique<MenuState>();  // 初始状态为菜单

    // 设置帧率限制（可选）
    //m_window.setFramerateLimit(60);
    m_view.setSize(LOGICAL_SIZE);
    m_view.setCenter(LOGICAL_SIZE.x / 2, LOGICAL_SIZE.y / 2);
    m_window.setView(m_view);

    initBlocks();   //创建砖块

    // 加载字体
    if (!m_font.loadFromFile("simhei.ttf")) {
        // 处理错误：可能文件不存在，可以用系统默认字体路径或报错
        std::cerr << "无法加载字体文件" << std::endl;
    }

	// 加载碰撞音效
    if (!m_hitBuffer.loadFromFile("hit.wav")) {
        std::cerr << "无法加载音效文件" << std::endl;
    }
    m_hitSound.setBuffer(m_hitBuffer);

    // 得分显示
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(10, 40);   // 左上角显示

    // 生命值显示
    m_lifeText.setFont(m_font);
    m_lifeText.setCharacterSize(24);
    m_lifeText.setFillColor(sf::Color::Red);
    m_lifeText.setPosition(10, 40);  

    // 键位显示
    m_KeyboardText.setFont(m_font);
    m_KeyboardText.setCharacterSize(24);
    m_KeyboardText.setFillColor(sf::Color::White);
    m_KeyboardText.setPosition(WINDOW_WIDTH-127,10);
    m_KeyboardText.setString(L"菜单：Esc");

    // 最高分显示
    highScoreText.setFont(m_font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setString(L"最高分：" + std::to_wstring(m_highScore));
    highScoreText.setPosition(LOGIC_WIDTH - 150, 40); // 右上角

    // 加载纹理，注意检查是否成功
    if (!m_heartTexture.loadFromFile("heart.png")) {
        std::cerr << "无法加载心形图片文件" << std::endl;
    }
    m_heartSprite.setTexture(m_heartTexture);
    // 如果想统一心形大小，可以设置一下缩放
    // 根据目标尺寸计算缩放因子（更精确）
    float targetWidth = 32.f;   // 你想要的心形显示宽度（像素）
    float targetHeight = 32.f;  // 显示高度
    sf::Vector2u textureSize = m_heartTexture.getSize();
    float scaleX = targetWidth / textureSize.x;
    float scaleY = targetHeight / textureSize.y;
    m_heartSprite.setScale(scaleX, scaleY);
    
    updateScoreText();
    
	loadHighScore();  // 加载最高分

    rd.seed(std::random_device{}());
}

Game::~Game()
{

}

void Game::run()
{
    while (m_window.isOpen())
    {
        // 1. 计算deltaTime
        float dt = m_clock.restart().asSeconds();

        // 2. 处理事件（如关闭窗口）
        processEvents();

        // 3. 更新游戏逻辑
        update(dt);

        // 4. 渲染
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // 检测按键
        // 检测关闭按钮来关闭窗口
        // 检测调整窗口大小
        if (event.type == sf::Event::Closed)
            m_window.close();
        else if (event.type == sf::Event::Resized)
        {
            unsigned int newWidth = event.size.width;
            unsigned int newHeight = event.size.height;
            // 更新窗口
            updateView(newWidth, newHeight);
            // 重新计算物体位置和碰撞边界
            onWindowResize(newWidth, newHeight);
        }
        else
        {
            m_currentState->handleInput(*this, event);
        }
    }
}

void Game::update(float dt)
{
    m_currentState->update(*this, dt);
}

void Game::render()
{
    m_window.setView(m_view);
    m_window.clear(sf::Color::Black);
    m_currentState->render(*this);
    m_window.display();  // 将绘制结果显示到屏幕
}

void Game::initBlocks()
{
    float blockWidth = 60;
    float blockHeight = 20;
    float startX = 80;
    float startY = 80;
    int rows = 10;
    int cols = 10;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            float x = startX + row * (blockWidth + 5);
            float y = startY + col * (blockHeight+5);
            m_blocks.emplace_back(x, y, blockWidth, blockHeight);
        }
    }
    block_nums = m_blocks.size();
}

void Game::updateScoreText()
{
    sf::String str = L"得分：" + std::to_wstring(m_score);
    m_scoreText.setString(str);
}

void Game::resetGame()
{
    // 重置砖块（清空并初始化）
    m_blocks.clear();
    initBlocks();

    // 重置挡板位置和大小
    m_paddle.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 30);
    m_paddle.resetSize(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));

    // 重置小球位置和状态
    m_ball.resetPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT - 50);

    // 重置掉落物
    m_dropitems.clear();

    // 重置分数
    m_score = 0;
    m_combo = 0;
    updateScoreText();

    // 重置生命值
    m_lifes = 3;

	// 重置游戏状态
	m_mouseSelected = false;
	m_isLaunched = false;
}

void Game::updateView(unsigned int windowWidth, unsigned int windowHeight)
{
    float logicalWidth = LOGICAL_SIZE.x;
    float logicalHeight = LOGICAL_SIZE.y;

    // 计算缩放因子（取较小的比例，确保全部内容可见）
    float scaleX = (float)windowWidth / logicalWidth;
    float scaleY = (float)windowHeight / logicalHeight;
    float scale = std::min(scaleX, scaleY);

    // 计算实际显示的逻辑画面尺寸（像素）
    float viewWidth = logicalWidth * scale;
    float viewHeight = logicalHeight * scale;

    // 计算视口（归一化坐标）
    float viewportX = (windowWidth - viewWidth) / 2.f / windowWidth;
    float viewportY = (windowHeight - viewHeight) / 2.f / windowHeight;
    float viewportW = viewWidth / windowWidth;
    float viewportH = viewHeight / windowHeight;

    m_view.setViewport(sf::FloatRect(viewportX, viewportY, viewportW, viewportH));
    m_window.setView(m_view);
}

void Game::initDropLife(float x, float y)
{
    auto drop = std::make_unique<Droplife>(x,y);
    m_dropitems.push_back(std::move(drop));
}

void Game::initDropAddPaddle(float x, float y)
{
    auto drop = std::make_unique<DropPaddleAdd>(x, y);
    m_dropitems.push_back(std::move(drop));
}

void Game::initDropSlowBall(float x, float y)
{
    auto drop = std::make_unique<DropBallSlow>(x, y);
    m_dropitems.push_back(std::move(drop));
}

void Game::addLife()
{
    if (m_lifes < 3)
    {
        m_lifes++;
        std::cout << "生命值加一，当前生命值为：" << m_lifes << std::endl;
    }
}

void Game::addPaddle()
{
    m_paddle.addSize();
	std::cout << "挡板长度增加" << std::endl;
}

void Game::slowBall()
{
    m_ball.slowdownSpeed(0.95f);
	std::cout << "小球速度减慢" << std::endl;
}

sf::Vector2f Game::generateRandomDirection()
{
    int sign = (std::uniform_int_distribution<>(0, 1)(rd) == 0) ? 1 : -1;
    std::uniform_real_distribution<float> angleDist(15.f, 80.f);
    float angle = angleDist(rd);
    float rad = angle * 3.14159f / 180.f;
    return sf::Vector2f(sign * std::cos(rad), -std::sin(rad));
}

void Game::changeState(std::unique_ptr<GameState> newState)
{
	m_currentState = std::move(newState);
}

void Game::startNewGame()
{
    changeState(std::make_unique<PlayingState>());
}

void Game::PauseMenuGame()
{
    changeState(std::make_unique<PauseMenuState>());
}

void Game::ContinueGame()
{
    changeState(std::make_unique<PlayingState>());
}

void Game::ReturnMenuGame()
{
    changeState(std::make_unique<MenuState>());
}

void Game::renderMenu(int selectedItem)
{
    sf::Text title("Breakout", m_font, 48);
    title.setPosition(LOGIC_WIDTH / 2 - title.getLocalBounds().width / 2, 100);
    m_window.draw(title);

    sf::Text start(L"开始游戏", m_font, 32);
    start.setPosition(LOGIC_WIDTH / 2 - start.getLocalBounds().width / 2, 250);
    start.setFillColor(selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(start);

    sf::Text rules(L"游戏规则", m_font, 32);
	rules.setPosition(LOGIC_WIDTH / 2 - rules.getLocalBounds().width / 2, 300);
	rules.setFillColor(selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(rules);

    sf::Text quit(L"退出游戏", m_font, 32);
    quit.setPosition(LOGIC_WIDTH / 2 - quit.getLocalBounds().width / 2, 350);
    quit.setFillColor(selectedItem == 2 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(quit);
}

void Game::renderPlaying()
{
	m_window.clear(sf::Color::White);
    // 在视口内绘制游戏背景（黑色）
    sf::RectangleShape background(sf::Vector2f(LOGICAL_SIZE.x, LOGICAL_SIZE.y));
    background.setFillColor(sf::Color::Black);
    background.setPosition(0, 0);
    m_window.draw(background);

    // 绘制挡板和小球
    m_paddle.draw(m_window);
    m_ball.draw(m_window);

    // 绘制砖块
    for (const auto& block : m_blocks)
    {
        block.draw(m_window);
    }

    // 绘制辅助线
    if (!m_isLaunched && m_mouseSelected)
    {
        sf::Vector2f paddleCenter = m_paddle.getBounds().getPosition()
            + sf::Vector2f(m_paddle.getBounds().width / 2, m_paddle.getBounds().height / 2);
        sf::Vertex line[] = {
        sf::Vertex(paddleCenter, sf::Color::White),
        sf::Vertex(paddleCenter + m_targetDirection * 100.f, sf::Color::White)
        };
        m_window.draw(line, 2, sf::Lines);
    }

    // 绘制掉落物
    for (auto& item : m_dropitems)
    {
        item->draw(m_window);
    }

    // 绘制得分
    m_window.draw(m_scoreText);
    m_window.draw(highScoreText);

    // 绘制生命值
    // 在 (6, 10) 的位置开始绘制，每个心形间隔 40 像素
    float startX = 6.f;
    float startY = 10.f;
    float offsetX = 40.f;

    for (int i = 0; i < m_lifes; ++i) {
        m_heartSprite.setPosition(startX + i * offsetX, startY);
        m_window.draw(m_heartSprite);
    }

    // 绘制键位
    m_window.draw(m_KeyboardText);

	// 绘制游戏开始提示
    if (!m_isLaunched)
    {
        sf::Text gameStartText;
        gameStartText.setFont(m_font);
        gameStartText.setCharacterSize(40);
        gameStartText.setFillColor(sf::Color::Red);

        gameStartText.setString(L"鼠标选择方向后按S开始游戏\n若不选择方向小球将随机发射\n可移动挡板选择发射位置");
        // 居中显示
        sf::FloatRect textRect = gameStartText.getLocalBounds();
        gameStartText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        gameStartText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        m_window.draw(gameStartText);
    }
}

void Game::renderPauseMenu(int selectedItem)
{
    sf::Text title("Breakout", m_font, 48);
    title.setPosition(LOGIC_WIDTH / 2 - title.getLocalBounds().width / 2, 100);
    m_window.draw(title);

    sf::Text start(L"继续游戏", m_font, 32);
    start.setPosition(LOGIC_WIDTH / 2 - start.getLocalBounds().width / 2, 250);
    start.setFillColor(selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(start);

    sf::Text continues(L"重新开始", m_font, 32);
    continues.setPosition(LOGIC_WIDTH / 2 - continues.getLocalBounds().width / 2, 300);
    continues.setFillColor(selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(continues);

	sf::Text rules(L"游戏规则", m_font, 32);
	rules.setPosition(LOGIC_WIDTH / 2 - rules.getLocalBounds().width / 2, 350);
	rules.setFillColor(selectedItem == 2 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(rules);

    sf::Text quit(L"退出游戏", m_font, 32);
    quit.setPosition(LOGIC_WIDTH / 2 - quit.getLocalBounds().width / 2, 400);
    quit.setFillColor(selectedItem == 3 ? sf::Color::Yellow : sf::Color::White);
    m_window.draw(quit);
}

void Game::renderGameOver(bool win)
{
    sf::Text title("Breakout", m_font, 48);
    title.setPosition(LOGIC_WIDTH / 2 - title.getLocalBounds().width / 2, 100);
    m_window.draw(title);
    sf::Text result(win ? L"游戏胜利！" : L"游戏失败！", m_font, 32);
    result.setPosition(LOGIC_WIDTH / 2 - result.getLocalBounds().width / 2, 250);
    result.setFillColor(sf::Color::Red);
    m_window.draw(result);
    sf::Text restart(L"按R重新开始", m_font, 32);
    restart.setPosition(LOGIC_WIDTH / 2 - restart.getLocalBounds().width / 2, 300);
    restart.setFillColor(sf::Color::White);
    m_window.draw(restart);
}

void Game::renderRules()
{
	m_window.clear(sf::Color::Black);
	sf::Text rules(L"游戏规则：\n1. 使用鼠标选择小球发射方向，按S开始游戏\n"
        "2. 挡板可左右移动，控制小球反弹\n3. 消除所有砖块获胜，生命值耗尽则失败\n"
       "4. 击中砖块有概率掉落道具，增加生命(红色)、挡板长度(蓝色)\n或减慢小球速度(绿色)\n"
        "5. 最高分将被记录\n按任意键返回菜单", m_font, 24);
    rules.setPosition(50, 100);
	rules.setFillColor(sf::Color::White);
	m_window.draw(rules);
}

void Game::updatePaddle(float dt)
{
    m_paddle.update(dt);
}

void Game::updateBall(float dt)
{
    m_ball.update(dt);
}

void Game::updateDropitems(float dt)
{
    for (auto& item : m_dropitems)
    {
        item->update(dt);
    }
}

bool Game::checkBallPaddleCollision()
{
    return m_ball.getBounds().intersects(m_paddle.getBounds());
    
}

void Game::bounceBallY()
{
    m_ball.bounceY();
}

void Game::repositionBallFromPaddle()
{
    // 计算新的Y坐标：挡板顶部减去小球的高度
    float newY = m_paddle.getBounds().top - m_ball.getBounds().height;
    // 保持X坐标不变
    float currentX = m_ball.getBounds().left;
    m_ball.resetPosition(currentX, newY);
}

void Game::handleBallBlockCollisions()
{
    for (auto it = m_blocks.begin(); it != m_blocks.end();)
    {
        if (m_ball.getBounds().intersects(it->getBounds()))
        {
            float newX = it->getCenterX();
            float newY = it->getCenterY();
            sf::FloatRect ballBounds = m_ball.getBounds();
            sf::FloatRect blockBounds = it->getBounds();
            sf::FloatRect overlap;
            it = m_blocks.erase(it);  // 删除砖块
            playHitSound();

            // 计算分数
            m_combo++;
            m_score = m_score + SCORE_PER_BLOCK + m_combo * 2;
            updateScoreText();
            std::cout << "砖块被击中！剩余砖块数：" << m_blocks.size() << std::endl;

            // 生成掉落物
            // 权重：70，10，10，10   dist({70,10,10,10})
            std::discrete_distribution<int> dist({ 70,10,10,10 });

            // 返回 0，1，2，3 之一，概率分别为 70%，10%，10%，10%
            int result = dist(rd);

            switch (result)
            {
            case 0:break;
            case 1:initDropLife(newX, newY);
                break;
            case 2:initDropAddPaddle(newX, newY);
                break;
            case 3:initDropSlowBall(newX, newY);
                break;
            }

            // 小球与砖块碰撞后
            m_ball.increaseSpeed(1.05f, 500.f);   // 每次增加5%，最大速度400
            if (ballBounds.intersects(blockBounds, overlap))
            {
                // 根据重叠矩形的宽高判断碰撞方向
                if (overlap.width > overlap.height)
                {
                    // 垂直碰撞（上下）
                    m_ball.bounceY();
                    // 修正位置防止卡入（可选）
                    if (ballBounds.top < blockBounds.top)
                        m_ball.resetPosition(ballBounds.left, blockBounds.top - ballBounds.height);
                    else
                        m_ball.resetPosition(ballBounds.left, blockBounds.top + blockBounds.height);
                }
                else
                {
                    // 水平碰撞（左右）
                    m_ball.bounceX();
                    // 修正位置
                    if (ballBounds.left < blockBounds.left)
                        m_ball.resetPosition(blockBounds.left - ballBounds.width, ballBounds.top);
                    else
                        m_ball.resetPosition(blockBounds.left + blockBounds.width, ballBounds.top);
                }
            }
            // 一个球可能同时击中多个砖块，通常一次只处理一个
            // 因此处理完一个就break出去，避免同一帧多次反弹
            break;
        }
        else
            ++it;
    }
}

bool Game::areBlocksEmpty()
{
	return m_blocks.empty();
}

void Game::winGame(bool win)
{
    if (m_score > m_highScore) {
        m_highScore = m_score;
        saveHighScore();
        highScoreText.setString(L"最高分：" + std::to_wstring(m_highScore));
    }
	changeState(std::make_unique<GameOverState>(win));
    std::cout << "You win!" << std::endl;
}

void Game::endGame(bool end)
{
    if (m_score > m_highScore) {
        m_highScore = m_score;
        saveHighScore();
        highScoreText.setString(L"最高分：" + std::to_wstring(m_highScore));
    }
    changeState(std::make_unique<GameOverState>(end));
    std::cout << "Game Over!" << std::endl;
}

void Game::handleDropItemCollisions()
{
    for (auto& item : m_dropitems)
    {
        if (item->getBounds().intersects(m_paddle.getBounds()))
        {
            item->onCollect(*this);
            item->setAlive(false);
        }
    }
}

bool Game::isBallOutOfBottom()
{
    return m_ball.getBounds().top + m_ball.getBounds().height > WINDOW_HEIGHT;
}

void Game::decreaseLife()
{
    m_lifes -= 1;
    std::cout << "生命值-1" << std::endl;
    std::cout << "当前生命值为：" << m_lifes << std::endl;
}

void Game::resetBallPosition()
{
    m_ball.resetPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT - 50);
}

void Game::setBallVelocity(const sf::Vector2f& velocity)
{
    m_ball.setVelocity(velocity);
}

void Game::removeDeadDropitems()
{
    m_dropitems.erase(std::remove_if(m_dropitems.begin(), m_dropitems.end(),
        [](const auto& item) {return !item->isAlive(); }), m_dropitems.end());
}

void Game::MouseDirectionSelect(const sf::Vector2f& mousePos)
{
    sf::Vector2f paddleCenter = m_paddle.getBounds().getPosition() + sf::Vector2f(m_paddle.getBounds().width / 2, m_paddle.getBounds().height / 2);
    sf::Vector2f direction = mousePos - paddleCenter;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.01f)
    {
        direction /= length;
        m_targetDirection = direction;
        m_mouseSelected = true;
    }
}

void Game::playHitSound()
{
    m_hitSound.play();
}

void Game::loadHighScore()
{
    std::ifstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        file >> m_highScore;
        file.close();
    }
    else {
        m_highScore = 0;
    }
    highScoreText.setString(L"最高分：" + std::to_wstring(m_highScore));
}

void Game::saveHighScore()
{
    std::ofstream file(HIGH_SCORE_FILE);
    if (file.is_open())
    {
        file << m_highScore;
		file.close();
    }
}