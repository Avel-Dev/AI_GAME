#include "EnemyDNA.hpp"

#include <cmath>
#include <cstdlib>

EnemyDNA::EnemyDNA()
    : health(100.0f),
      speed(200.0f),
      damage(20.0f),
      shootCooldown(0.5f),
      projectileSpeed(500.0f),
      attackRange(400.0f) {}

void EnemyDNA::Randomize() {
    // Generate random values within bounds using normalized random [0,1]
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;
    float r3 = static_cast<float>(rand()) / RAND_MAX;
    float r4 = static_cast<float>(rand()) / RAND_MAX;
    float r5 = static_cast<float>(rand()) / RAND_MAX;
    float r6 = static_cast<float>(rand()) / RAND_MAX;

    health = MIN_HEALTH + r1 * (MAX_HEALTH - MIN_HEALTH);
    speed = MIN_SPEED + r2 * (MAX_SPEED - MIN_SPEED);
    damage = MIN_DAMAGE + r3 * (MAX_DAMAGE - MIN_DAMAGE);
    shootCooldown = MIN_SHOOT_COOLDOWN + r4 * (MAX_SHOOT_COOLDOWN - MIN_SHOOT_COOLDOWN);
    projectileSpeed = MIN_PROJECTILE_SPEED + r5 * (MAX_PROJECTILE_SPEED - MIN_PROJECTILE_SPEED);
    attackRange = MIN_ATTACK_RANGE + r6 * (MAX_ATTACK_RANGE - MIN_ATTACK_RANGE);
}

void EnemyDNA::ClampToBounds() {
    // Clamp each trait to prevent extreme values
    if (health < MIN_HEALTH) health = MIN_HEALTH;
    if (health > MAX_HEALTH) health = MAX_HEALTH;

    if (speed < MIN_SPEED) speed = MIN_SPEED;
    if (speed > MAX_SPEED) speed = MAX_SPEED;

    if (damage < MIN_DAMAGE) damage = MIN_DAMAGE;
    if (damage > MAX_DAMAGE) damage = MAX_DAMAGE;

    if (shootCooldown < MIN_SHOOT_COOLDOWN) shootCooldown = MIN_SHOOT_COOLDOWN;
    if (shootCooldown > MAX_SHOOT_COOLDOWN) shootCooldown = MAX_SHOOT_COOLDOWN;

    if (projectileSpeed < MIN_PROJECTILE_SPEED) projectileSpeed = MIN_PROJECTILE_SPEED;
    if (projectileSpeed > MAX_PROJECTILE_SPEED) projectileSpeed = MAX_PROJECTILE_SPEED;

    if (attackRange < MIN_ATTACK_RANGE) attackRange = MIN_ATTACK_RANGE;
    if (attackRange > MAX_ATTACK_RANGE) attackRange = MAX_ATTACK_RANGE;
}

void EnemyDNA::CalculateFitness(float playerHealthLost, float waveDuration) {
    // Fitness function: rewards enemies that deal damage and survive
    // Penalizes enemies that die instantly (survival time too low)

    // Base fitness from damage dealt (primary objective)
    float damageFitness = damageDealtToPlayer * 10.0f;

    // Survival time bonus (surviving longer = more opportunities to deal damage)
    float survivalFitness = survivalTime * 5.0f;

    // Efficiency bonus: damage per second
    float efficiencyBonus = 0.0f;
    if (survivalTime > 0.0f) {
        efficiencyBonus = (damageDealtToPlayer / survivalTime) * 20.0f;
    }

    // Penalty for dying too quickly (less than 10% of wave duration)
    float survivalPenalty = 0.0f;
    if (waveDuration > 0.0f && survivalTime < waveDuration * 0.1f) {
        survivalPenalty = -50.0f; // Significant penalty for instant death
    }

    // Penalty for dealing no damage (ineffective enemy)
    float ineffectivenessPenalty = 0.0f;
    if (damageDealtToPlayer <= 0.0f) {
        ineffectivenessPenalty = -30.0f;
    }

    // Combine components
    fitness = damageFitness + survivalFitness + efficiencyBonus + survivalPenalty + ineffectivenessPenalty;

    // Ensure minimum fitness (avoid negative values causing issues)
    if (fitness < 1.0f) fitness = 1.0f;
}
