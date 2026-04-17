#include "Enemy.hpp"

#include "Game.hpp"
#include "GameObjects.hpp"
#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <string>

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

	// Smooth steering
	velocity.x += (toPlayer.x * speed - velocity.x) * turnSpeed * delta;
	velocity.y += (toPlayer.y * speed - velocity.y) * turnSpeed * delta;

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
	Vector2 distance = Vector2Subtract(position, Game::m_Player.position);
	int length = Vector2Length(distance);
	std::string length_text = "Distance :" + std::to_string(length);
	std::string Health_text = "Health :" + std::to_string(health);
	std::string speed_text = "Speed :" + std::to_string(speed);
	std::string coolDown_text = "Cool Down :" + std::to_string(shootCoolDown);
	std::string range_text = "Range :" + std::to_string(attackRange);
	std::string projectileSpeed_text = "P Speed :" + std::to_string(projectileSpeed);

	DrawText(length_text.c_str(), position.x, position.y, 15.0, WHITE);
	DrawText(speed_text.c_str(), position.x, position.y + 20, 15.0, WHITE);
	DrawText(coolDown_text.c_str(), position.x, position.y + 40, 15.0, WHITE);
	DrawText(range_text.c_str(), position.x, position.y + 60, 15.0, WHITE);
	DrawText(projectileSpeed_text.c_str(), position.x, position.y + 80, 15.0, WHITE);
	DrawText(Health_text.c_str(), position.x, position.y + 100, 15.0, WHITE);
}

void Enemy::Shoot() {
	Vector2 distance = Vector2Subtract(position, Game::m_Player.position);
	if (Vector2Length(distance) > attackRange) return;

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

	Game::SpawnBullet(ENEMY, BLUE, damage, spawnPos, direction, projectileSpeed);
}
