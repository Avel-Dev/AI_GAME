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
	float speed;
	float Damage;

	float shootTimer = 0.0f;
	float shootCoolDown = 0.5f;
	bool isAlive;

	void Update(float delta);
	void Shoot();
	void Draw() const;
};
