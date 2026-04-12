#pragma once
#include "Game.hpp"

#include <fstream>
#include <raylib.h>
#include <string>

static void updateHighScore() {
	if (Game::s_gameData.score) {
		if (Game::s_gameData.score > Game::s_gameData.hightscore) {
			std::ofstream data(".gamedata");
			if (data.is_open()) {
				data << Game::s_gameData.score;
				data.close();
			}
		}
	}
}

static void drawScoreBoard() {
	std::string scoreText = "Score: " + std::to_string(Game::s_gameData.score);
	std::string highScoreText =
	  "Last High Score: " + std::to_string(Game::s_gameData.hightscore);

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

static void GameOverText() {
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
