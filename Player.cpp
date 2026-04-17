#include "Player.hpp"

#include "Game.hpp"
#include "GameObjects.hpp"

#include <cmath>
#include <raylib.h>

void Player::Init() {
	m_PlayerTexture = LoadTexture("assets/Roket_anim.png");
}

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

	if (IsKeyDown(KEY_SPACE)) {
		Shoot(delta);
	}

	Vector2 mouse = GetMousePosition();
	direction = {mouse.x - position.x, mouse.y - position.y};
	angle = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;
}

void Player::Shoot(float delta) {
	static float shootCooldown = 0.0f;
	shootCooldown -= delta;

	if (shootCooldown <= 0.0f) {
		Game::SpawnBullet(PLAYER, RED, 5, position, direction, projectileSpeed);
		shootCooldown = 0.1f;
	}
}

void Player::Draw() const {
	Rectangle source = {(float)(Game::curr_frame % frameCount) * frameWidth, 0,
			(float)frameWidth, (float)frameHeight};

	Rectangle dest = {position.x, position.y, (float)frameWidth * 2, (float)frameHeight * 2};

	Vector2 origin = {dest.width / 2.0f, dest.height / 2.0f};
	DrawTexturePro(m_PlayerTexture, source, dest, origin, angle, WHITE);
}
