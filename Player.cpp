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
	if (position.x > gameWidth - 20) position.x = gameWidth - 20;
	if (position.y < 20) position.y = 20;
	if (position.y > gameHeight - 20) position.y = gameHeight - 20;

	animTimer += delta;

	if (animTimer >= frameDuration) {
		animTimer = 0.0f;
		curr_frame = (curr_frame + 1) % frameCount;
	}
}

void Player::Draw() const {
	// Vector2 mouse = GetMousePosition();
	Rectangle source = {(float)curr_frame * frameWidth, 0, (float)frameWidth,
			(float)frameHeight};

	Rectangle dest = {position.x, position.y, (float)frameWidth * 2, (float)frameHeight * 2};

	DrawTexturePro(m_PlayerTexture, source, dest,
		     (Vector2){frameWidth / 2.0f, frameHeight / 2.0f}, // origin (top-left)
		     0.0f, WHITE);
}
