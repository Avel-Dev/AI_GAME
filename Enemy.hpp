#pragma once
#include <raylib.h>
#include <cstddef>
#include "EnemyDNA.hpp"

class Enemy {
	static Texture2D sprite;

        public:
	static int frameWidth;
	static int frameHeight;
	static int frameCount;

	static void Init();
	Vector2 position;
	Vector2 velocity;

	float health;
	float speed;
	float damage;
	float projectileSpeed;
	float attackRange;

	float shootTimer;
	float shootCoolDown;
	bool active;

	// GA integration
	EnemyDNA dna;              // Genetic traits
	float spawnTime;           // When this enemy was spawned
	float damageDealt;         // Total damage dealt to player
	size_t populationIndex;    // Index in GA population

	void Update(float delta);
	void Shoot();
	void Draw() const;

	// Apply DNA traits to enemy
	void ApplyDNA(const EnemyDNA& dna);

	// Record damage dealt (for fitness)
	void RecordDamage(float dmg) { damageDealt += dmg; }
};
