#pragma once
#include "Enemy.hpp"
#include "GameObjects.hpp"
#include "GeneticAlgorithm.hpp"
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
	void DrawHealthBar(float x, float y, float width, float height, float current, float max);
	void Spawn();
	void SpawnAstroid();
	void SpawnEnemies(float delta);
	void Despawn();
	void StartNextWave();
	void UpdateWave(float delta);
	void EndWave(float delta);

	void UpdateHighScore();
	void DrawScoreBoard();
	void DrawGAStats();  // Debug visualization for GA
	void GameOverText();
	void AnimateCounter(float delta);

	static void SpawnBullet(BulletOwner owner, Color color, float damage, Vector2 position,
			    Vector2 direction, float speed);

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

	float maxHealth = 100.0f;
	bool m_gameOver;

	float m_asteroidSpawnCounter = 2.0f;
	float m_asteroidSpawnRate = 2.0f;

	int m_maxWaveEnemies = 1;
	int m_enemiesKilled = 0;
	float m_enemySpawnCounter = 2.0f;
	float m_enemySpawnRate = 2.0f;
	int m_wave = 0;
	GameWaveState m_waveState = START_NEXT_WAVE;

	int m_enemiesSpawned = 0;

	// Genetic Algorithm system
	GeneticAlgorithm m_ga;
	float m_waveStartTime = 0.0f;
	float m_playerHealthAtWaveStart = 100.0f;
	static constexpr int GA_POPULATION_SIZE = 10;  // Number of unique DNA profiles

	void EvaluateWaveFitness();  // Calculate fitness after wave ends
	void AssignEnemyDNA(Enemy& enemy);  // Assign DNA to spawned enemy
};
