#include "Player.hpp"

#include "Game.hpp"
#include "raylib.h"

#include <math.h>

void Player::Init() {
	m_PlayerTexture = LoadTexture("Roket_anim.png");
}

// Player
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
	};
}

void Player::Shoot(float delta) {
	// Shooting
	if (IsKeyDown(KEY_SPACE)) {
		static float shootCooldown = 0.0f;
		shootCooldown -= delta;
		if (shootCooldown <= 0.0f) {
			Vector2 pos = {
			  position.x + (frameWidth / 2.0f) + 4 // tweak this
			  ,
			  position.y - (frameHeight / 2.0f) + 4 // tweak this
			};

			Game::SpawnBullet(RED, 5, pos, {0, -1.0f});
			shootCooldown = 0.1f; // 4 m_Bullets per second
		}
	}
}

void Player::Draw() const {
	// Vector2 mouse = GetMousePosition();
	Rectangle source = {(float)(Game::curr_frame % frameCount) * frameWidth, 0,
			(float)frameWidth, (float)frameHeight};

	Rectangle dest = {position.x, position.y, (float)frameWidth * 2, (float)frameHeight * 2};

	DrawTexturePro(m_PlayerTexture, source, dest,
		     (Vector2){frameWidth / 2.0f, frameHeight / 2.0f}, // origin (top-left)
		     0.0f, WHITE);
}
