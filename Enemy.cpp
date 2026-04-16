#include "Enemy.hpp"
#include "Game.hpp"
#include "GameObjects.hpp"

#include <cmath>

Texture2D Enemy::sprite;
int Enemy::frameWidth = 32;
int Enemy::frameHeight = 32;
int Enemy::frameCount = 3;

void Enemy::Init() {
	sprite = LoadTexture("assets/Enemy.png");
}

void Enemy::Update(float delta) {
	// Direction to player
	Vector2 toPlayer = {Game::m_Player.position.x - position.x,
			    Game::m_Player.position.y - position.y};

	float len = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
	if (len != 0) {
		toPlayer.x /= len;
		toPlayer.y /= len;
	}

	float turnSpeed = 2.5f;
	float moveSpeed = 120.0f;

	// Smooth steering
	velocity.x += (toPlayer.x * moveSpeed - velocity.x) * turnSpeed * delta;
	velocity.y += (toPlayer.y * moveSpeed - velocity.y) * turnSpeed * delta;

	// Add wobble
	velocity.x += sin(GetTime() * 3.0f) * 20.0f * delta;

	// Move
	position.x += velocity.x * delta;
	position.y += velocity.y * delta;

	// Shooting
	shootTimer += delta;
	if (shootTimer >= shootCoolDown) {
		Shoot();
		shootTimer = 0.0f;
	}
}

void Enemy::Draw() const {
	Rectangle source = {(float)(Game::curr_frame % frameCount) * frameWidth, 0,
			(float)frameWidth, (float)frameHeight};

	Rectangle dest = {position.x, position.y, (float)frameWidth * 2, (float)frameHeight * 2};

	float angle = atan2f(velocity.y, velocity.x) * RAD2DEG + 90.0f;

	DrawTexturePro(sprite, source, dest, (Vector2){(float)frameWidth, (float)frameHeight},
		     angle, WHITE);
}

void Enemy::Shoot() {
	// Use movement direction
	Vector2 direction = velocity;

	// Avoid division by zero
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length == 0) return;

	// Normalize
	direction.x /= length;
	direction.y /= length;

	// Offset forward
	float offset = 20.0f;

	Vector2 spawnPos = {position.x + direction.x * offset, position.y + direction.y * offset};

	Game::SpawnBullet(ENEMY, BLUE, 4, spawnPos, direction);
}
