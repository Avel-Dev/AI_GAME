#pragma once
#include "GameObjects.hpp"
#include "Player.hpp"

#include <raylib.h>
#include <vector>
// Screen size
static int SCREEN_WIDTH = 1920;
static int SCREEN_HEIGHT = 1080;

class Game {
        public:
	Game();
	~Game();

        public:
	void Init();
	void Start();
	void End();
	void Update(float delta);
	void Draw();
	void Spawn();
	void Despawn();

	void UpdateHighScore();
	void DrawScoreBoard();
	void GameOverText();
	void AnimateCounter(float delta);

	static void SpawnBullet(Color color, int damage, Vector2 position, Vector2 direction);

        public:
	struct GameData {
		int highscore = 0;
		int score = 0;
		bool gameOver = false;
	};

	static GameData s_gameData;

        private:
	Player m_Player;

	static std::vector<Astroid> m_Astroid;
	static std::vector<Bullet> m_Bullets;
	static std::vector<Enemy> m_Enemies;

	bool m_GameOver;

	float astroid_swapn_counter = 2;
	float astroid_swapn_rate = 2;

	float enemy_swapn_counter = 2;
	float enemy_swapn_rate = 2;

        public:
	static int curr_frame;
	static float animTimer;
	static float frameDuration; // 100ms per frame
};
