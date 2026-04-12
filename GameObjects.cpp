#include "GameObjects.hpp"

#include "Game.hpp"

void Player::Update(float delta) {
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) position.x -= speed * delta;

	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed * delta;

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) position.y -= speed * delta;

	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) position.y += speed * delta;

	// Clamp to screen
	if (position.x < 20) position.x = 20;
	if (position.x > SCREEN_WIDTH - 20) position.x = SCREEN_WIDTH - 20;
	if (position.y < 20) position.y = 20;
	if (position.y > SCREEN_HEIGHT - 20) position.y = SCREEN_HEIGHT - 20;
}

void Player::Draw() const {
	DrawTriangle({position.x, position.y - 20}, {position.x - 15, position.y + 15},
		   {position.x + 15, position.y + 15}, GREEN);
}

// Move downward
void Enemy::Update(float delta) {
	position.y += speed * delta;
	if (position.y > 600) active = false; // deactivate if off-screen
}

void Enemy::Draw() const {
	DrawRectangleV(position, {40, 40}, RED); // simple square ship
}

void Bullet::Update(float delta) {
	position.y -= speed * delta;
	if (position.y < 0) active = false;
}

void Bullet::Draw() const {
	DrawCircleV(position, 4, RED);
}
