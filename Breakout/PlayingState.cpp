#include"PlayingState.h"
#include"Game.h"
#include"Constants.h"

// 处理输入事件
void PlayingState::handleInput(Game& game, const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePos = game.getWindow().mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
		game.MouseDirectionSelect(mousePos);
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			game.PauseMenuGame();
		}
		if (event.key.code == sf::Keyboard::S && !game.isLaunched()) {
			sf::Vector2f direction;
			if (game.isMouseDirectionSelected()) {
				direction = game.getMouseDirection();
			}
			else {
				direction = game.generateRandomDirection();
			}
			game.setBallVelocity(direction * 300.f);
			game.setLaunched(true);
		}
	}
}

void PlayingState::update(Game& game, float dt)
{
	// 挡板始终可以移动（瞄准）
	game.updatePaddle(dt);

	// 如果未发射，不更新其他逻辑
	if (!game.isLaunched())
	{
		sf::FloatRect paddleBounds = game.getPaddleBounds(); // 需要提供该接口
		float ballRadius = BALL_RADIUS; 
		// 计算小球位置：挡板顶部中央，Y 坐标为挡板上方 - 小球直径
		float ballX = paddleBounds.left + paddleBounds.width / 2 - ballRadius;
		float ballY = paddleBounds.top - ballRadius * 2;
		game.setBallPosition(ballX, ballY);
		return; // 未发射时不更新其他逻辑
	}

	// 更新小球
	game.updateBall(dt);

	// 掉落物
	game.updateDropitems(dt);

	// 处理小球与挡板碰撞
	if (game.checkBallPaddleCollision()) {
		game.bounceBallY();
		game.repositionBallFromPaddle();  // 防止卡入
		game.playHitSound();  // 播放碰撞音效
	}

	// 处理小球与砖块碰撞
	game.handleBallBlockCollisions();

	// 判断游戏胜利
	if(game.areBlocksEmpty())
	{
		game.winGame(true);
	}

	// 处理掉落物与挡板碰撞
	game.handleDropItemCollisions();

	// 检查小球是否超出底部边界（y > 窗口高度）
	if (game.isBallOutOfBottom())
	{
		game.decreaseLife();
		game.resetCombo();
		game.resetBallPosition();
		game.setBallVelocity(game.generateRandomDirection() * 300.f);
		if (game.getLifes() == 0)
		{
			game.endGame(false);
		}
	}
	
	// 移除超出底部或已收集的掉落物
	game.removeDeadDropitems();
}

void PlayingState::render(Game& game)
{
	// 渲染游戏
	game.renderPlaying();
}