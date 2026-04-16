#pragma once
#include "Enemy.hpp"
#include "GameObjects.hpp"
#include "Player.hpp"

#include <raylib.h>
#include <vector>
// Screen size
static int SCREEN_WIDTH = 1920;
static int SCREEN_HEIGHT = 1080;

enum GameWaveState { START_NEXT_WAVE, WAVE_CONTINUE, END_WAVE };

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
	void SpawnAstroid();
	void SpawnEnemies(float delta);
	void Despawn();

	void StartNextWave();
	void UpdateWave(float delta);
	void EndWave(float delta);

	void UpdateHighScore();
	void DrawScoreBoard();
	void GameOverText();
	void AnimateCounter(float delta);

	static void SpawnBullet(BulletOwner owner, Color color, int damage, Vector2 position,
			    Vector2 direction);

        public:
	struct GameData {
		int highscore = 0;
		int score = 0;
		bool gameOver = false;
	};

	static GameData s_gameData;
	static int curr_frame;
	static float animTimer;
	static float frameDuration; // 100ms per frame

	static Player m_Player;

        private:
	static std::vector<Astroid> m_Astroid;
	static std::vector<Bullet> m_Bullets;
	static std::vector<Enemy> m_Enemies;

	bool m_GameOver;

	float astroid_swapn_counter = 2;
	float astroid_swapn_rate = 2;

	int MAX_WAVE_ENEMY = 2;
	int enemy_killed = 0; // in this wave
	float enemy_swapn_counter = 2;
	float enemy_swapn_rate = 2;
	int m_Wave = 0;
	GameWaveState m_WaveState = START_NEXT_WAVE;

	int EnemySpawned = 0;
};
