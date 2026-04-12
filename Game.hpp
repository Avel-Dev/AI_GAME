#pragma once
#include "GameObjects.hpp"

#include <raylib.h>
#include <vector>
// Screen size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Game {
        public:
	Game();
	~Game();

        public:
	void Init();
	void Start();
	void End();

        public:
	struct GameData {
		int hightscore = 0;
		int score = 0;
		bool gameOver = false;
	};

	static GameData s_gameData;

        private:
	Player m_Player;

	std::vector<Enemy> m_Enemies;
	std::vector<Bullet> m_Bullets;
	bool m_GameOver;
};
