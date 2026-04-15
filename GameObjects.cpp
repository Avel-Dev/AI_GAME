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
	position.x += val.x;
	position.y += val.y;
	if (position.y < 0) active = false;
}

void Bullet::Draw() const {
	DrawCircleV(position, 4, RED);
}
