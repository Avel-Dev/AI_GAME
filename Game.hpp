#pragma once
#include "GameObjects.hpp"
#include "Player.hpp"

#include <raylib.h>
#include <vector>
// Screen size
static int SCREEN_WIDTH = 1920;
static int SCREEN_HEIGHT = 1080;
static int gameWidth = 1920;
static int gameHeight = 1080;

class Game {
        public:
	Game();
	~Game();

        public:
	void Init();
	void Start();
	void End();

	void UpdateHighScore();
	void DrawScoreBoard();
	void GameOverText();

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

	std::vector<Astroid> m_Astroid;
	static std::vector<Bullet> m_Bullets;
	bool m_GameOver;

	RenderTexture2D m_Target;
	float astroid_swapn_counter = 2;
	float astroid_swapn_rate = 2;
};
