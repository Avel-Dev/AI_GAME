#include "Enemy.hpp"

#include "Game.hpp"

#include <math.h>

Texture2D Enemy::sprite;
int Enemy::frameWidth = 32;
int Enemy::frameHeight = 32;
int Enemy::frameCount = 3;

void Enemy::Init() {
	sprite = LoadTexture("assets/Enemy.png");
}

// Enemy
void Enemy::Update(float delta) {
	if (position.y > SCREEN_HEIGHT) {
		isAlive = false;
		return;
	}
	// Store previous position
	Vector2 prevPosition = position;

	// Movement (your existing logic)
	position.x += sin(GetTime() * 2.0f) * 50.0f * delta;
	position.y += speed * delta;

	// Compute velocity from movement
	velocity.x = (position.x - prevPosition.x) / delta;
	velocity.y = (position.y - prevPosition.y) / delta;

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

	Game::SpawnBullet(BLUE, 4, spawnPos, direction);
}
