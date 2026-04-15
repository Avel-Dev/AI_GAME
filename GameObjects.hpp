#pragma once

#include <raylib.h>

struct Astroid {
	Vector2 position;
	float speed;
	bool active;
	void Update(float delta);
	void Draw() const;
};

struct Bullet {
	Vector2 position;
	Vector2 direction;
	float speed;
	bool active;
	float timeAlive = 0.5f;

	void Update(float delta);
	void Draw() const;
};
