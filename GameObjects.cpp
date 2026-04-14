#include "GameObjects.hpp"

#include "Game.hpp"
#include "math.h"
#include "raylib.h"

// Astroid
//  Move downward
void Astroid::Update(float delta) {
	position.y += speed * delta;
	if (position.y > SCREEN_HEIGHT) active = false; // deactivate if off-screen
}

void Astroid::Draw() const {
	DrawRectangleV(position, {40, 40}, RED); // simple square ship
}

// Bullet
void Bullet::Update(float delta) {
	Vector2 val = {direction.x * speed * delta, direction.y * speed * delta};
	position.x += val.x;
	position.y += val.y;
	if (position.y < 0) active = false;
}

void Bullet::Draw() const {
	DrawCircleV(position, 4, RED);
}

Texture2D Enemy::sprite;
int Enemy::frameWidth = 32;
int Enemy::frameHeight = 32;
int Enemy::frameCount = 3;

void Enemy::Init() {
	sprite = LoadTexture("Enemy.png");
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

	// Shooting timer (you had a bug here 👇)
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

	DrawTexturePro(sprite, source, dest,
		     (Vector2){(float)frameWidth, (float)frameHeight}, // ✅ fixed center
		     angle, WHITE);
}

void Enemy::Shoot() {}
