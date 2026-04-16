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
	timeAlive -= delta;
	Vector2 val = {direction.x * speed * delta, direction.y * speed * delta};
	// position.x += val.x;
	// position.y += val.y;
	position.x += direction.x * speed * delta;
	position.y += direction.y * speed * delta;
	if (position.y < 0 || position.y > SCREEN_HEIGHT) active = false;
	if (position.x < 0 || position.x > SCREEN_WIDTH) active = false;
}

void Bullet::Draw() const {
	DrawCircleV(position, 4, RED);
}
