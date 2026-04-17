#pragma once
#include <raylib.h>

// Player class
class Player {
        public:
	Vector2 position;
	float speed;
	float health;
	float projectileSpeed = 400.0f;

	void Init();
	void Update(float delta);
	void Draw() const;
	void Shoot(float delta);

        private:
	Texture2D m_PlayerTexture;
	int frameWidth = 32;
	int frameHeight = 32;
	int frameCount = 3;
	Vector2 direction;
	float angle;
};
