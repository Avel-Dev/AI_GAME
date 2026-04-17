#include "GameObjects.hpp"

#include "Game.hpp"

#include <raylib.h>

void Astroid::Update(float delta) {
	position.y += speed * delta;
	if (position.y > SCREEN_HEIGHT) active = false;
}

void Astroid::Draw() const {
	DrawRectangleV(position, {40, 40}, RED);
}

void Bullet::Update(float delta) {
	position.x += direction.x * speed * delta;
	position.y += direction.y * speed * delta;

	if (position.y < 0 || position.y > SCREEN_HEIGHT) active = false;
	if (position.x < 0 || position.x > SCREEN_WIDTH) active = false;
}

void Bullet::Draw() const {
	DrawCircleV(position, 4, RED);
}
