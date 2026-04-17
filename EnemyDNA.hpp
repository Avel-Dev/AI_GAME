#pragma once

// EnemyDNA.hpp - Genetic Algorithm Genome Structure
// Defines the genetic traits that enemies evolve over waves

struct EnemyDNA {
    // Genetic traits (genome)
    float health;           // Enemy health points
    float speed;            // Movement speed
    float damage;           // Bullet damage
    float shootCooldown;    // Time between shots (lower = faster firing)
    float projectileSpeed;  // Bullet speed
    float attackRange;      // Maximum firing distance

    // Fitness tracking metrics (not part of genome, used for evolution)
    float damageDealtToPlayer = 0.0f;  // Total damage dealt to player
    float survivalTime = 0.0f;         // How long the enemy survived
    float fitness = 0.0f;              // Calculated fitness score

    // Constructor with default initialization
    EnemyDNA();

    // Randomize all traits within bounds
    void Randomize();

    // Clamp all values to safe bounds
    void ClampToBounds();

    // Calculate fitness based on performance metrics
    void CalculateFitness(float playerHealthLost, float waveDuration);

    // Static bounds for genetic traits (prevents extreme difficulty spikes)
    static constexpr float MIN_HEALTH = 50.0f;
    static constexpr float MAX_HEALTH = 300.0f;

    static constexpr float MIN_SPEED = 100.0f;
    static constexpr float MAX_SPEED = 400.0f;

    static constexpr float MIN_DAMAGE = 10.0f;
    static constexpr float MAX_DAMAGE = 50.0f;

    static constexpr float MIN_SHOOT_COOLDOWN = 0.1f;
    static constexpr float MAX_SHOOT_COOLDOWN = 1.5f;

    static constexpr float MIN_PROJECTILE_SPEED = 300.0f;
    static constexpr float MAX_PROJECTILE_SPEED = 800.0f;

    static constexpr float MIN_ATTACK_RANGE = 200.0f;
    static constexpr float MAX_ATTACK_RANGE = 700.0f;

    // Mutation parameters
    static constexpr float MUTATION_RATE = 0.3f;       // 30% chance per gene
    static constexpr float MUTATION_STRENGTH = 0.15f; // Max 15% change per mutation
};
