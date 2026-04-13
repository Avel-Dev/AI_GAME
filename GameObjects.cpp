#include "GameObjects.hpp"

#include "Game.hpp"
#include "math.h"
#include "raylib.h"

// Astroid
//  Move downward
void Astroid::Update(float delta) {
	position.y += speed * delta;
	if (position.y > gameHeight) active = false; // deactivate if off-screen
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

// Enemy
void Enemy::Update(float delta) {
	// Optional: simple horizontal movement (zig-zag)
	position.x += sin(GetTime() * 2.0f) * 50.0f * delta;
	shootTimer += delta;
	shootTimer += delta;
	if (shootTimer >= shootCoolDown) {
		Shoot(); // your bullet spawn function
		shootTimer = 0.0f;
	}
}

void Enemy::Draw() const {
	DrawTriangle({position.x, position.y + 20}, {position.x - 15, position.y - 15},
		   {position.x + 15, position.y - 15}, GREEN);
}

void Enemy::Shoot() {}
