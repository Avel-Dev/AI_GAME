#pragma once
#include <raylib.h>

class Enemy {
	static Texture2D sprite;

        public:
	static int frameWidth;
	static int frameHeight;
	static int frameCount;

	static void Init();
	Vector2 position;
	Vector2 velocity;

	float health;
	float speed;
	float damage;
	float projectileSpeed;
	float attackRange;

	float shootTimer;
	float shootCoolDown;
	bool active;

	void Update(float delta);
	void Shoot();
	void Draw() const;
};
