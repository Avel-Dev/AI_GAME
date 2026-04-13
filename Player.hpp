#pragma once
#include <raylib.h>

// Player class
class Player {
        public:
	Vector2 position;
	float speed;
	float health;

	void Init();
	void Update(float delta);
	void Draw() const;
	void Shoot(float delta);

        private:
	Texture2D m_PlayerTexture;

	int curr_frame = 0;

        public:
	float animTimer = 0.0f;
	float frameDuration = 0.1f; // 100ms per frame
	int frameWidth = 32;
	int frameHeight = 32;
	int frameCount = 3;
};
