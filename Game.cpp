#include "Game.hpp"

#include "Enemy.hpp"
#include "GameObjects.hpp"
#include "Player.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

Game::GameData Game::s_gameData;
std::vector<Bullet> Game::m_Bullets;
std::vector<Astroid> Game::m_Astroid;
std::vector<Enemy> Game::m_Enemies;

int Game::curr_frame = 0;
float Game::animTimer = 0.0f;
float Game::frameDuration = 0.1f; // 100ms per frame
Player Game::m_Player;

Game::Game() {}

Game::~Game() {}

void Game::AnimateCounter(float delta) {
	animTimer += delta;

	if (animTimer >= frameDuration) {
		animTimer = 0.0f;
		curr_frame = (curr_frame + 1);
	}
}

void Game::Init() {
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Game");
	SetTargetFPS(60);

	SCREEN_WIDTH = GetScreenWidth();
	SCREEN_HEIGHT = GetScreenHeight();

	srand(time(NULL));

	std::ifstream data(".gamedata");
	if (data.is_open() && (data >> s_gameData.highscore)) {
		data.close();
	} else {
		s_gameData.highscore = 0;
	}

	s_gameData.score = 0;

	m_Player.Init();
	m_Player.position = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
	m_Player.speed = 300.0f;
	m_Player.health = 100;
	m_gameOver = false;
	Enemy::Init();

	// Initialize Genetic Algorithm system
	m_ga.InitializePopulation(GA_POPULATION_SIZE);
	std::cout << "[Game] Genetic Algorithm initialized with population size " << GA_POPULATION_SIZE << std::endl;
}

void Game::SpawnBullet(BulletOwner owner, Color color, float damage, Vector2 position,
		   Vector2 direction, float speed) {
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length != 0) {
		direction.x /= length;
		direction.y /= length;
	}

	Bullet b;
	b.damage = damage;
	b.position = position;
	b.speed = speed;
	b.active = true;
	b.direction = direction;
	b.owner = owner;
	m_Bullets.push_back(b);
}

void Game::StartNextWave() {
	m_wave += 1;
	m_waveState = WAVE_CONTINUE;
	m_waveStartTime = GetTime();
	m_playerHealthAtWaveStart = m_Player.health;
	m_maxWaveEnemies = 1 + (m_wave / 2);  // Scale enemy count with waves

	std::cout << "[Game] Starting Wave " << m_wave << " with " << m_maxWaveEnemies << " enemies" << std::endl;
}

void Game::UpdateWave(float delta) {
	if (m_enemiesKilled == m_maxWaveEnemies) {
		m_waveState = END_WAVE;
		return;
	}
	SpawnEnemies(delta);
}

void Game::EndWave(float delta) {
	// Calculate wave duration and player damage taken
	float waveDuration = GetTime() - m_waveStartTime;
	float playerHealthLost = m_playerHealthAtWaveStart - m_Player.health;

	// Evaluate fitness for all enemies and evolve population
	EvaluateWaveFitness();
	m_ga.EvolveGeneration(playerHealthLost, waveDuration);

	// Reset wave state
	m_enemiesKilled = 0;
	m_enemiesSpawned = 0;
	m_Enemies.clear();
	m_waveState = START_NEXT_WAVE;
}

void Game::EvaluateWaveFitness() {
	// Enemies track their own performance via damageDealt and spawnTime
	// Their fitness is calculated during evolution in GeneticAlgorithm
	float waveDuration = GetTime() - m_waveStartTime;

	for (auto& enemy : m_Enemies) {
		if (enemy.active) {
			// Mark surviving enemies with survival time
			enemy.dna.survivalTime = waveDuration;
		} else {
			// Calculate survival time for defeated enemies
			enemy.dna.survivalTime = enemy.spawnTime > 0 ? GetTime() - enemy.spawnTime : waveDuration * 0.5f;
		}
		enemy.dna.damageDealtToPlayer = enemy.damageDealt;
	}
}

void Game::AssignEnemyDNA(Enemy& enemy) {
	// Get evolved DNA from GA system
	EnemyDNA dna = m_ga.GetEnemyDNA();
	enemy.ApplyDNA(dna);
}

void Game::Start() {
	while (!WindowShouldClose()) {
		float delta = GetFrameTime();

		if (m_gameOver) {
			BeginDrawing();
			ClearBackground(BLACK);

			GameOverText();
			UpdateHighScore();
			EndDrawing();

			if (IsKeyDown(KEY_R)) {
				return;
			}
			continue;
		}

		switch (m_waveState) {
		case START_NEXT_WAVE:
			StartNextWave();
			break;
		case WAVE_CONTINUE:
			UpdateWave(delta);
			break;
		case END_WAVE:
			EndWave(delta);
			break;
		}

		Update(delta);
		Despawn();
		Draw();
	}
}

void Game::End() {
	CloseWindow();
}

void Game::SpawnAstroid() {
	if (m_asteroidSpawnCounter < 0 && m_Astroid.size() < 20) {
		Astroid newAstroid;
		newAstroid.position = {static_cast<float>(rand() % (SCREEN_WIDTH - 20)),
				   -40.0f};
		newAstroid.speed = 80 + rand() % 170;
		newAstroid.active = true;
		m_Astroid.push_back(newAstroid);

		m_asteroidSpawnCounter = m_asteroidSpawnRate;
	}
	m_asteroidSpawnCounter -= 0.1f;
}

void Game::SpawnEnemies(float delta) {
	if (m_enemiesSpawned != m_maxWaveEnemies && m_enemySpawnCounter <= 0) {
		Enemy enemy;
		enemy.position = {static_cast<float>(rand() % SCREEN_WIDTH), 10.0f};

		// Assign evolved DNA from GA system (replaces hardcoded values)
		AssignEnemyDNA(enemy);

		enemy.active = true;
		enemy.velocity = {0.0f, 0.0f};
		enemy.shootTimer = 0;
		enemy.populationIndex = m_enemiesSpawned % GA_POPULATION_SIZE;

		m_enemiesSpawned += 1;
		m_Enemies.push_back(enemy);
		m_enemySpawnCounter = m_enemySpawnRate;

		std::cout << "[Game] Spawned enemy " << m_enemiesSpawned << "/" << m_maxWaveEnemies
		          << " [H:" << enemy.health << " S:" << enemy.speed << " D:" << enemy.damage << "]" << std::endl;
	}
	m_enemySpawnCounter -= delta;
}

void Game::Spawn() {}

void Game::Despawn() {
	// Track which enemy hit the player (for fitness)
	Enemy* hitPlayerEnemy = nullptr;

	for (auto& b : m_Bullets) {
		if (!b.active) continue;

		// Enemy bullet hitting player
		if (b.owner == ENEMY &&
		    CheckCollisionCircles(b.position, 4,
				      {m_Player.position.x + 20, m_Player.position.y + 20},
				      20)) {
			b.active = false;
			m_Player.health -= b.damage;

			// Track damage dealt by enemy for fitness calculation
			if (!m_Enemies.empty()) {
				// Find closest enemy as the one who shot (approximation)
				float minDist = 999999.0f;
				size_t closestIdx = 0;
				for (size_t i = 0; i < m_Enemies.size(); ++i) {
					if (!m_Enemies[i].active) continue;
					float dist = Vector2Distance(b.position, m_Enemies[i].position);
					if (dist < minDist) {
						minDist = dist;
						closestIdx = i;
					}
				}
				m_Enemies[closestIdx].RecordDamage(b.damage);
			}

			if (m_Player.health <= 0) {
				m_gameOver = true;
			}
		}

		// Check collision with asteroids
		for (auto& e : m_Astroid) {
			if (e.active &&
			    CheckCollisionCircles(
			      b.position, 4, {e.position.x + 20, e.position.y + 20}, 20)) {
				e.active = false;
				b.active = false;
				s_gameData.score++;
				break;
			}
		}

		if (!b.active) continue;
		if (b.owner == ENEMY) continue;

		// Check collision with enemies
		for (auto& enemy : m_Enemies) {
			if (enemy.active &&
			    CheckCollisionCircles(
			      b.position, 4, {enemy.position.x + 20, enemy.position.y + 20},
			      20)) {
				m_enemiesKilled += 1;

				enemy.health -= b.damage;
				if (enemy.health <= 0) {
					enemy.active = false;
				}
				b.active = false;
				s_gameData.score += 2;
				break;
			}
		}
	}
}

void Game::Update(float delta) {
	AnimateCounter(delta);

	// Update m_Bullets
	for (auto& b : m_Bullets) {
		if (b.active) b.Update(delta);
	}
	for (auto& e : m_Astroid) {
		if (e.active) e.Update(delta);
	}
	for (auto& e : m_Enemies) {
		if (e.active) e.Update(delta);
	}

	m_Player.Update(delta);

	// Player vs Asteroids
	for (auto& e : m_Astroid) {
		if (e.active &&
		    CheckCollisionCircles(m_Player.position, 4,
				      {e.position.x + 20, e.position.y + 20}, 20)) {
			m_gameOver = true;
			break;
		}
	}

	// Player vs Enemies
	for (auto& enemy : m_Enemies) {
		if (enemy.active &&
		    CheckCollisionCircles(m_Player.position, 4,
				      {enemy.position.x + 20, enemy.position.y + 20}, 20)) {
			m_gameOver = true;
			break;
		}
	}

	m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(),
				 [](const Bullet& b) { return !b.active; }),
		      m_Bullets.end());
	m_Astroid.erase(std::remove_if(m_Astroid.begin(), m_Astroid.end(),
				 [](const Astroid& e) { return !e.active; }),
		      m_Astroid.end());
	m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(),
				 [](const Enemy& e) { return !e.active; }),
		      m_Enemies.end());
}

void Game::DrawHealthBar(float x, float y, float width, float height, float current, float max) {
	float percent = current / max;

	DrawRectangle(x, y, width, height, GRAY);
	DrawRectangle(x, y, (int)(width * percent), height, RED);
	Rectangle rect = {x, y, width, height};
	DrawRectangleLinesEx(rect, 2.0f, WHITE); // 4px thick border}
}

void Game::Draw() {
	BeginDrawing();
	ClearBackground(BLACK);

	int barWidth = 500;
	for (const auto& b : m_Bullets) {
		if (b.active) b.Draw();
	}
	for (const auto& e : m_Astroid) {
		if (e.active) e.Draw();
	}
	for (const auto& e : m_Enemies) {
		if (e.active) e.Draw();
	}

	m_Player.Draw();

	std::string bulletsCount = "Bullets: " + std::to_string(m_Bullets.size());
	std::string asteroidCount = "Astroid: " + std::to_string(m_Astroid.size());
	std::string enemyCount = "Enemies: " + std::to_string(m_Enemies.size());
	std::string waveCount = "Wave: " + std::to_string(m_wave);

	DrawText("Move with WASD / Arrow Keys", 10, 10, 20, WHITE);
	DrawText(bulletsCount.c_str(), 10, 30, 20, WHITE);
	DrawText(asteroidCount.c_str(), 10, 50, 20, WHITE);
	DrawText(enemyCount.c_str(), 10, 70, 20, WHITE);
	DrawText(waveCount.c_str(), SCREEN_WIDTH / 2, 70, 40, WHITE);
	DrawScoreBoard();
	DrawHealthBar(((float)SCREEN_WIDTH / 2) - ((float)barWidth / 2), 10, barWidth, 25,
		    m_Player.health, maxHealth);

	// Draw GA debug stats
	DrawGAStats();

	EndDrawing();
}

void Game::DrawGAStats() {
	// Genetic Algorithm debug visualization panel
	int panelX = 10;
	int panelY = SCREEN_HEIGHT - 150;
	int panelWidth = 300;
	int panelHeight = 140;

	// Semi-transparent background
	DrawRectangle(panelX, panelY, panelWidth, panelHeight, Color{0, 0, 0, 180});
	DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, DARKGRAY);

	int lineHeight = 20;
	int y = panelY + 5;

	// Title
	DrawText("GENETIC ALGORITHM", panelX + 10, y, 18, YELLOW);
	y += lineHeight + 5;

	// Stats
	std::string genText = "Generation: " + std::to_string(static_cast<int>(m_ga.GetGeneration()));
	DrawText(genText.c_str(), panelX + 10, y, 16, WHITE);
	y += lineHeight;

	std::string fitnessText = "Avg Fitness: " + std::to_string(static_cast<int>(m_ga.GetAverageFitness()));
	DrawText(fitnessText.c_str(), panelX + 10, y, 16, WHITE);
	y += lineHeight;

	std::string bestFitText = "Best Fitness: " + std::to_string(static_cast<int>(m_ga.GetBestFitness()));
	DrawText(bestFitText.c_str(), panelX + 10, y, 16, WHITE);
	y += lineHeight;

	std::string popText = "Population: " + std::to_string(GA_POPULATION_SIZE);
	DrawText(popText.c_str(), panelX + 10, y, 16, WHITE);
	y += lineHeight;

	std::string eliteText = "Elite Preserved: " + std::to_string(m_ga.GetEliteCount());
	DrawText(eliteText.c_str(), panelX + 10, y, 16, WHITE);
}

void Game::UpdateHighScore() {
	if (Game::s_gameData.score) {
		if (Game::s_gameData.score > Game::s_gameData.highscore) {
			std::ofstream data(".gamedata");
			if (data.is_open()) {
				data << Game::s_gameData.score;
				data.close();
			}
		}
	}
}

void Game::DrawScoreBoard() {
	std::string scoreText = "Score: " + std::to_string(Game::s_gameData.score);
	std::string highScoreText =
	  "Last High Score: " + std::to_string(Game::s_gameData.highscore);

	int fontSize = 20;
	int margin = 10;

	// Get screen width
	int screenWidth = GetScreenWidth();

	// Measure text widths
	int scoreWidth = MeasureText(scoreText.c_str(), fontSize);
	int highScoreWidth = MeasureText(highScoreText.c_str(), fontSize);

	// Draw aligned to top-right
	DrawText(scoreText.c_str(), screenWidth - scoreWidth - margin, 10, fontSize, YELLOW);
	DrawText(highScoreText.c_str(), screenWidth - highScoreWidth - margin, 40, fontSize,
	         YELLOW);
}

void Game::GameOverText() {
	const char* text = "GAME OVER";
	int fontSize = 50;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int textWidth = MeasureText(text, fontSize);

	int x = (screenWidth - textWidth) / 2;
	int y = (screenHeight - fontSize) / 2;

	DrawText(text, x, y, fontSize, WHITE);
	DrawText(std::to_string(Game::s_gameData.score).c_str(), x + 100, y + 100, fontSize - 10,
	         WHITE);
	DrawText("Press R to restart", x + 100, y + 150, fontSize - 10, WHITE);
}
