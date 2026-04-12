#pragma once

#include <raylib.h>

// Player class
struct Player {
	Vector2 position;
	float speed;

	void Update(float delta);
	void Draw() const;
};

struct Enemy {
	Vector2 position;
	float speed;
	bool active;
	void Update(float delta);
	void Draw() const;
};

struct Bullet {
	Vector2 position;
	float speed;
	bool active;

	void Update(float delta);
	void Draw() const;
};
