#include "Game.hpp"

#include "utility.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

Game::GameData Game::s_gameData;

Game::Game() {}

Game::~Game() {}

void Game::Init() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D Spaceship Shooter (C++)");
	SetTargetFPS(60);

	srand(time(NULL));
}

void Game::Start() {
	std::ifstream data(".gamedata");
	if (data.is_open()) {
		data >> s_gameData.hightscore;
		data.close();
	}
	s_gameData.score = 0;
	m_Player.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.0f};
	m_Player.speed = 300.0f;
	bool m_GameOver = false;
	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		if (m_GameOver) {
			GameOverText();
			updateHighScore();

			if (IsKeyDown(KEY_R)) {
				return;
			}
			EndDrawing();
			continue;
		}

		m_Player.Update(delta);
		// Shooting
		if (IsKeyDown(KEY_SPACE) && !m_GameOver) {
			static float shootCooldown = 0.0f;
			shootCooldown -= delta;
			if (shootCooldown <= 0.0f) {
				Bullet b;
				b.position = {m_Player.position.x,
					    m_Player.position.y - 20};
				b.speed = 400;
				b.active = true;
				m_Bullets.push_back(b);
				shootCooldown = 0.1f; // 4 m_Bullets per second
			}
		}
		if (m_Enemies.size() < 3) {
			// Spawn m_Enemies;
			Enemy newEnemy;
			newEnemy.position = {static_cast<float>(rand() % 760),
					 -40};	      // top of screen
			newEnemy.speed = 80 + rand() % 100; // random speed
			newEnemy.active = true;
			m_Enemies.push_back(newEnemy);
		}

		// Update m_Bullets
		for (auto& b : m_Bullets) {
			if (b.active) b.Update(delta);
		}
		for (auto& e : m_Enemies) {
			if (e.active) e.Update(delta);
		}
		for (auto& e : m_Enemies) {
			if (e.active && CheckCollisionCircles(
				        m_Player.position, 4,
				        {e.position.x + 20, e.position.y + 20}, 20)) {
				m_GameOver = true;
			}

			for (auto& b : m_Bullets) {
				if (b.active && e.active &&
				    CheckCollisionCircles(
				      b.position, 4, {e.position.x + 20, e.position.y + 20},
				      20)) {
					e.active = false;
					b.active = false;
					s_gameData.score++;
				}
			}
		}

		// Cleanup
		m_Bullets.erase(std::remove_if(m_Bullets.begin(), m_Bullets.end(),
					 [](const Bullet& b) { return !b.active; }),
			      m_Bullets.end());
		m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(),
					 [](const Enemy& e) { return !e.active; }),
			      m_Enemies.end());

		// Draw m_Bullets
		for (const auto& b : m_Bullets) {
			if (b.active) b.Draw();
		}
		for (const auto& e : m_Enemies) {
			if (e.active) e.Draw();
		}

		std::string m_Bullets_count = "m_Bullets: " + std::to_string(m_Bullets.size());

		m_Player.Draw();
		DrawText("Move with WASD / Arrow Keys", 10, 10, 20, WHITE);
		DrawText(m_Bullets_count.c_str(), 10, 30, 20, WHITE);
		drawScoreBoard();
		EndDrawing();
	}
}

void Game::End() {
	CloseWindow();
}
