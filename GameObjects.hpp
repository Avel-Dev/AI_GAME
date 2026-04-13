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
	float speed;
	bool active;

	void Update(float delta);
	void Draw() const;
};

struct Enemy {
	Vector2 position;
	float speed;
	float Damage;

	float shootTimer;
	float shootCoolDown;
	bool isAlive;

	void Update(float delta);
	void Shoot();
	void Draw() const;
};
