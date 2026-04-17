#pragma once

#include "EnemyDNA.hpp"

#include <vector>

// GeneticAlgorithm.hpp - Evolution System Manager
// Handles the entire GA pipeline: selection, crossover, mutation

class GeneticAlgorithm {
public:
    GeneticAlgorithm();

    // Initialize with random population
    void InitializePopulation(int populationSize);

    // Get DNA for spawning an enemy
    EnemyDNA GetEnemyDNA();

    // Record enemy performance metrics
    void RecordEnemyPerformance(size_t enemyIndex, float damageDealt, float survivalTime);

    // Evolve population after wave completes
    // Returns true if evolution was successful
    bool EvolveGeneration(float playerHealthLost, float waveDuration);

    // Get statistics for debugging/display
    float GetAverageFitness() const;
    float GetBestFitness() const;
    float GetGeneration() const { return static_cast<float>(m_generation); }

    // Elite count for debugging
    int GetEliteCount() const { return m_eliteCount; }

    // Access current population (for debug visualization)
    const std::vector<EnemyDNA>& GetPopulation() const { return m_population; }

private:
    // Selection methods
    int TournamentSelection(int tournamentSize = 3);
    int RouletteWheelSelection();

    // Crossover methods
    EnemyDNA SinglePointCrossover(const EnemyDNA& parent1, const EnemyDNA& parent2);
    EnemyDNA UniformCrossover(const EnemyDNA& parent1, const EnemyDNA& parent2);

    // Mutation
    void Mutate(EnemyDNA& dna);

    // Evolution parameters
    static constexpr int ELITE_COUNT = 2;        // Top performers preserved unchanged
    static constexpr float CROSSOVER_RATE = 0.8f; // 80% chance of crossover vs copy
    static constexpr float MUTATION_RATE = 0.2f;  // Additional mutation chance

    std::vector<EnemyDNA> m_population;         // Current generation
    std::vector<EnemyDNA> m_previousPopulation; // Previous generation (for reference)
    std::vector<float> m_fitnessScores;         // Cached fitness values

    int m_generation = 0;
    int m_eliteCount = 0;
    int m_nextSpawnIndex = 0; // For sequential DNA assignment

    // Population size tracking
    int m_populationSize = 0;

    // Difficulty scaling factor (increases over generations)
    float m_difficultyMultiplier = 1.0f;
};
