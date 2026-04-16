#pragma once
#include <raylib.h>

struct Astroid {
	Vector2 position;
	float speed;
	bool active;
	void Update(float delta);
	void Draw() const;
};

enum BulletOwner { PLAYER, ENEMY };

struct Bullet {
	Vector2 position;
	Vector2 direction;
	float speed;
	float timeAlive = 0.2f;
	bool active;
	BulletOwner owner;
	void Update(float delta);
	void Draw() const;
};
