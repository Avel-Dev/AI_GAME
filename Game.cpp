#include "Game.hpp"

#include "GameObjects.hpp"
#include "raylib.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

Game::GameData Game::s_gameData;
std::vector<Bullet> Game::m_Bullets;
std::vector<Astroid> Game::m_Astroid;
std::vector<Enemy> Game::m_Enemies;

int Game::curr_frame = 0;
float Game::animTimer = 0.0f;
float Game::frameDuration = 0.1f; // 100ms per frame

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
	//	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	// InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Game");
	InitWindow(800, 600, "Game");
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
	m_Player.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.0f};
	m_Player.speed = 300.0f;

	m_GameOver = false;
	Enemy::Init();
}

void Game::SpawnBullet(Color color, int damage, Vector2 position, Vector2 direction) {
	Bullet b;
	b.position = position;
	b.speed = 400;
	b.active = true;
	b.direction = direction;
	m_Bullets.push_back(b);
}

void Game::Start() {
	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		if (m_GameOver) {
			BeginDrawing();
			ClearBackground(BLACK);

			GameOverText();
			UpdateHighScore();

			if (IsKeyDown(KEY_R)) {
				return;
			}
			//			EndTextureMode();
			EndDrawing();
			continue;
		}
		Spawn();
		Update(delta);
		Despawn();
		Draw();
	}
}

void Game::End() {
	CloseWindow();
}

void Game::Spawn() {
	if (astroid_swapn_counter < 0 && m_Astroid.size() < 20) {
		Astroid newAstroid;
		newAstroid.position = {static_cast<float>(rand() % (SCREEN_WIDTH - 20)),
				   -40};	        // top of screen ;
		newAstroid.speed = 80 + rand() % 170; // random speed ;
		newAstroid.active = true;
		m_Astroid.push_back(newAstroid);

		astroid_swapn_counter = astroid_swapn_rate;
	}
	astroid_swapn_counter -= 0.1f;

	if (enemy_swapn_counter < 0 && m_Enemies.size() < 10) {
		Enemy enemy;
		enemy.position = {static_cast<float>(rand() % (SCREEN_WIDTH - 20)),
			        -40}; // top of screen
		enemy.speed = 80 + rand() % 170;
		enemy.isAlive = true;
		m_Enemies.push_back(enemy);
		enemy_swapn_counter = enemy_swapn_rate;
	}
	enemy_swapn_counter -= 0.1f;
}

void Game::Despawn() {
	for (auto& b : m_Bullets) {
		if (b.timeAlive > 0) continue;
		if (!b.active) continue;
		if (CheckCollisionCircles(b.position, 4,
				      {m_Player.position.x + 20, m_Player.position.y + 20},
				      20)) {
			m_GameOver = true;
		}

		// Check collision with asteroids
		for (auto& e : m_Astroid) {
			if (e.active &&
			    CheckCollisionCircles(
			      b.position, 4, {e.position.x + 20, e.position.y + 20}, 20)) {
				e.active = false;
				b.active = false;
				s_gameData.score++;
				break; // bullet is gone, stop checking
			}
		}

		if (!b.active) continue;

		// Check collision with enemies
		for (auto& enemy : m_Enemies) {
			if (enemy.isAlive &&
			    CheckCollisionCircles(
			      b.position, 4, {enemy.position.x + 20, enemy.position.y + 20},
			      20)) {
				enemy.isAlive = false;
				b.active = false;
				s_gameData.score += 2; // maybe different score
				break;
			}
		}
	} // Cleanup
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
		if (e.isAlive) e.Update(delta);
	}

	m_Player.Update(delta);

	// Player vs Asteroids
	for (auto& e : m_Astroid) {
		if (e.active &&
		    CheckCollisionCircles(m_Player.position, 4,
				      {e.position.x + 20, e.position.y + 20}, 20)) {
			m_GameOver = true;
			break;
		}
	}

	// Player vs Enemies
	for (auto& enemy : m_Enemies) {
		if (enemy.isAlive &&
		    CheckCollisionCircles(m_Player.position, 4,
				      {enemy.position.x + 20, enemy.position.y + 20}, 20)) {
			m_GameOver = true;
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
				 [](const Enemy& e) { return !e.isAlive; }),
		      m_Enemies.end());
}

void Game::Draw() {
	BeginDrawing();
	ClearBackground(BLACK);

	// Draw m_Bullets
	for (const auto& b : m_Bullets) {
		if (b.active) b.Draw();
	}
	for (const auto& e : m_Astroid) {
		if (e.active) e.Draw();
	}
	for (const auto& e : m_Enemies) {
		if (e.isAlive) e.Draw();
	}

	std::string m_Bullets_count = "m_Bullets: " + std::to_string(m_Bullets.size());
	std::string m_Astroid_count = "m_Astroid: " + std::to_string(m_Astroid.size());
	std::string m_Enemy_count = "m_Enemies: " + std::to_string(m_Enemies.size());

	m_Player.Draw();
	DrawText("Move with WASD / Arrow Keys", 10, 10, 20, WHITE);
	DrawText(m_Bullets_count.c_str(), 10, 30, 20, WHITE);
	DrawText(m_Astroid_count.c_str(), 10, 50, 20, WHITE);
	DrawText(m_Enemy_count.c_str(), 10, 70, 20, WHITE);
	DrawScoreBoard();
	EndDrawing();
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

	// Get screen dimensions
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	// Measure text width
	int textWidth = MeasureText(text, fontSize);

	// Calculate centered position
	int x = (screenWidth - textWidth) / 2;
	int y = (screenHeight - fontSize) / 2;

	// Draw text
	DrawText(text, x, y, fontSize, WHITE);
	if (Game::s_gameData.score >= 0) {
		DrawText(std::to_string(Game::s_gameData.score).c_str(), x + 100, y + 100,
		         fontSize - 10, WHITE);
	}
	DrawText("Press R to restart", x + 100, y + 150, fontSize - 10, WHITE);
}
