#include "GameObjects.hpp"

#include <algorithm>
#include <cstdlib> // for rand()
#include <ctime>   // for srand()
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>

GameData gameData_;

void updateHighScore() {
	if (gameData_.score) {
		if (gameData_.score > gameData_.hightscore) {
			std::ofstream data(".gamedata");
			if (data.is_open()) {
				data << gameData_.score;
				data.close();
			}
		}
	}
}

void drawScoreBoard() {
	std::string scoreText = "Score: " + std::to_string(gameData_.score);
	std::string highScoreText = "Last High Score: " + std::to_string(gameData_.hightscore);

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

void GameOverText() {
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
	if (gameData_.score >= 0) {
		DrawText(std::to_string(gameData_.score).c_str(), x + 100, y + 100,
		         fontSize - 10, WHITE);
	}
	DrawText("Press R to restart", x + 100, y + 150, fontSize - 10, WHITE);
}

void game() {
	std::ifstream data(".gamedata");
	if (data.is_open()) {
		data >> gameData_.hightscore;
		data.close();
	}
	gameData_.score = 0;
	Player player;
	player.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.0f};
	player.speed = 300.0f;

	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;

	bool gameOver = false;
	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		// Draw
		BeginDrawing();
		ClearBackground(BLACK);
		if (gameOver) {
			GameOverText();
			updateHighScore();

			if (IsKeyDown(KEY_R)) {
				return;
			}
			EndDrawing();
			continue;
		}

		player.Update(delta);
		// Shooting
		if (IsKeyDown(KEY_SPACE) && !gameOver) {
			static float shootCooldown = 0.0f;
			shootCooldown -= delta;
			if (shootCooldown <= 0.0f) {
				Bullet b;
				b.position = {player.position.x, player.position.y - 20};
				b.speed = 400;
				b.active = true;
				bullets.push_back(b);
				shootCooldown = 0.1f; // 4 bullets per second
			}
		}
		if (enemies.size() < 3) {
			// Spawn enemies;
			Enemy newEnemy;
			newEnemy.position = {static_cast<float>(rand() % 760),
					 -40};	      // top of screen
			newEnemy.speed = 80 + rand() % 100; // random speed
			newEnemy.active = true;
			enemies.push_back(newEnemy);
		}

		// Update bullets
		for (auto& b : bullets) {
			if (b.active) b.Update(delta);
		}
		for (auto& e : enemies) {
			if (e.active) e.Update(delta);
		}
		for (auto& e : enemies) {
			if (e.active && CheckCollisionCircles(
				        player.position, 4,
				        {e.position.x + 20, e.position.y + 20}, 20)) {
				gameOver = true;
			}

			for (auto& b : bullets) {
				if (b.active && e.active &&
				    CheckCollisionCircles(
				      b.position, 4, {e.position.x + 20, e.position.y + 20},
				      20)) {
					e.active = false;
					b.active = false;
					gameData_.score++;
				}
			}
		}

		// Cleanup
		bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
				         [](const Bullet& b) { return !b.active; }),
			    bullets.end());
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
				         [](const Enemy& e) { return !e.active; }),
			    enemies.end());

		// Draw bullets
		for (const auto& b : bullets) {
			if (b.active) b.Draw();
		}
		for (const auto& e : enemies) {
			if (e.active) e.Draw();
		}

		std::string bullets_count = "Bullets: " + std::to_string(bullets.size());

		player.Draw();
		DrawText("Move with WASD / Arrow Keys", 10, 10, 20, WHITE);
		DrawText(bullets_count.c_str(), 10, 30, 20, WHITE);
		drawScoreBoard();
		EndDrawing();
	}
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D Spaceship Shooter (C++)");
	SetTargetFPS(60);

	srand(time(NULL));

	while (!WindowShouldClose()) {
		// home screen

		//  game
		std::cout << "Game Started" << "\n";
		game();
	}

	CloseWindow();

	return 0;
}
