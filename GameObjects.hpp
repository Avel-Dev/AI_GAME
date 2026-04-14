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

	void Update(float delta);
	void Draw() const;
};

struct Enemy {
	static Texture2D sprite;
	static void Init();

        public:
	static int frameWidth;
	static int frameHeight;
	static int frameCount;

	Vector2 position;
	Vector2 velocity;
	float speed;
	float Damage;

	float shootTimer;
	float shootCoolDown;
	bool isAlive;

	void Update(float delta);
	void Shoot();
	void Draw() const;
};
