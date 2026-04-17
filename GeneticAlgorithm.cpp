#include "GeneticAlgorithm.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm() = default;

void GeneticAlgorithm::InitializePopulation(int populationSize) {
    m_populationSize = populationSize;
    m_population.clear();
    m_population.reserve(populationSize);
    m_fitnessScores.resize(populationSize, 0.0f);

    // Create initial random population
    for (int i = 0; i < populationSize; ++i) {
        EnemyDNA dna;
        dna.Randomize();
        dna.ClampToBounds();
        m_population.push_back(dna);
    }

    m_generation = 1;
    m_nextSpawnIndex = 0;
    m_difficultyMultiplier = 1.0f;

    std::cout << "[GA] Initialized population of " << populationSize << " enemies" << std::endl;
}

EnemyDNA GeneticAlgorithm::GetEnemyDNA() {
    if (m_population.empty()) {
        EnemyDNA defaultDNA;
        defaultDNA.Randomize();
        return defaultDNA;
    }

    // Return DNA from population, cycling through available genomes
    // This ensures genetic diversity within the wave
    EnemyDNA dna = m_population[m_nextSpawnIndex % m_population.size()];
    m_nextSpawnIndex++;

    // Apply difficulty scaling (gradually increase challenge)
    dna.health *= m_difficultyMultiplier;
    dna.damage *= m_difficultyMultiplier;
    dna.speed *= std::sqrt(m_difficultyMultiplier); // Speed scales slower

    dna.ClampToBounds();
    return dna;
}

void GeneticAlgorithm::RecordEnemyPerformance(size_t enemyIndex, float damageDealt, float survivalTime) {
    if (enemyIndex >= m_population.size()) return;

    m_population[enemyIndex].damageDealtToPlayer = damageDealt;
    m_population[enemyIndex].survivalTime = survivalTime;
}

int GeneticAlgorithm::TournamentSelection(int tournamentSize) {
    int bestIndex = rand() % m_population.size();
    float bestFitness = m_population[bestIndex].fitness;

    // Run tournament: pick random candidates, select the fittest
    for (int i = 1; i < tournamentSize; ++i) {
        int candidateIndex = rand() % m_population.size();
        float candidateFitness = m_population[candidateIndex].fitness;

        if (candidateFitness > bestFitness) {
            bestFitness = candidateFitness;
            bestIndex = candidateIndex;
        }
    }

    return bestIndex;
}

int GeneticAlgorithm::RouletteWheelSelection() {
    // Calculate total fitness
    float totalFitness = 0.0f;
    for (const auto& dna : m_population) {
        totalFitness += dna.fitness;
    }

    if (totalFitness <= 0.0f) {
        return rand() % m_population.size();
    }

    // Spin the wheel
    float spin = static_cast<float>(rand()) / RAND_MAX * totalFitness;
    float cumulative = 0.0f;

    for (size_t i = 0; i < m_population.size(); ++i) {
        cumulative += m_population[i].fitness;
        if (cumulative >= spin) {
            return static_cast<int>(i);
        }
    }

    return static_cast<int>(m_population.size() - 1);
}

EnemyDNA GeneticAlgorithm::SinglePointCrossover(const EnemyDNA& parent1, const EnemyDNA& parent2) {
    EnemyDNA child;

    // Choose crossover point (0-6, representing each gene)
    int crossoverPoint = rand() % 6;

    // Copy genes from parent1 up to crossover point
    switch (crossoverPoint) {
        default: child.health = parent1.health;
        case 1: child.speed = (crossoverPoint <= 1) ? parent1.speed : parent2.speed;
        case 2: child.damage = (crossoverPoint <= 2) ? parent1.damage : parent2.damage;
        case 3: child.shootCooldown = (crossoverPoint <= 3) ? parent1.shootCooldown : parent2.shootCooldown;
        case 4: child.projectileSpeed = (crossoverPoint <= 4) ? parent1.projectileSpeed : parent2.projectileSpeed;
        case 5: child.attackRange = (crossoverPoint <= 5) ? parent1.attackRange : parent2.attackRange;
    }

    return child;
}

EnemyDNA GeneticAlgorithm::UniformCrossover(const EnemyDNA& parent1, const EnemyDNA& parent2) {
    EnemyDNA child;

    // For each gene, randomly select from either parent
    child.health = (rand() % 2 == 0) ? parent1.health : parent2.health;
    child.speed = (rand() % 2 == 0) ? parent1.speed : parent2.speed;
    child.damage = (rand() % 2 == 0) ? parent1.damage : parent2.damage;
    child.shootCooldown = (rand() % 2 == 0) ? parent1.shootCooldown : parent2.shootCooldown;
    child.projectileSpeed = (rand() % 2 == 0) ? parent1.projectileSpeed : parent2.projectileSpeed;
    child.attackRange = (rand() % 2 == 0) ? parent1.attackRange : parent2.attackRange;

    return child;
}

void GeneticAlgorithm::Mutate(EnemyDNA& dna) {
    // Helper lambda for mutating a single gene
    auto mutateGene = [](float& value, float minVal, float maxVal) {
        float randomChange = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f; // [-1, 1]
        float changeAmount = value * EnemyDNA::MUTATION_STRENGTH * randomChange;
        value += changeAmount;

        // Ensure we stay within bounds
        if (value < minVal) value = minVal;
        if (value > maxVal) value = maxVal;
    };

    // Mutate each gene with probability MUTATION_RATE
    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.health, EnemyDNA::MIN_HEALTH, EnemyDNA::MAX_HEALTH);
    }

    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.speed, EnemyDNA::MIN_SPEED, EnemyDNA::MAX_SPEED);
    }

    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.damage, EnemyDNA::MIN_DAMAGE, EnemyDNA::MAX_DAMAGE);
    }

    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.shootCooldown, EnemyDNA::MIN_SHOOT_COOLDOWN, EnemyDNA::MAX_SHOOT_COOLDOWN);
    }

    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.projectileSpeed, EnemyDNA::MIN_PROJECTILE_SPEED, EnemyDNA::MAX_PROJECTILE_SPEED);
    }

    if (static_cast<float>(rand()) / RAND_MAX < EnemyDNA::MUTATION_RATE) {
        mutateGene(dna.attackRange, EnemyDNA::MIN_ATTACK_RANGE, EnemyDNA::MAX_ATTACK_RANGE);
    }

    // Final clamp to ensure safety
    dna.ClampToBounds();
}

bool GeneticAlgorithm::EvolveGeneration(float playerHealthLost, float waveDuration) {
    if (m_population.empty()) {
        std::cout << "[GA] Cannot evolve: population is empty" << std::endl;
        return false;
    }

    // Step 1: Calculate fitness for all members
    for (auto& dna : m_population) {
        dna.CalculateFitness(playerHealthLost, waveDuration);
    }

    // Step 2: Sort by fitness (descending)
    std::vector<EnemyDNA> sortedPopulation = m_population;
    std::sort(sortedPopulation.begin(), sortedPopulation.end(),
              [](const EnemyDNA& a, const EnemyDNA& b) { return a.fitness > b.fitness; });

    // Step 3: Create new generation
    std::vector<EnemyDNA> newGeneration;
    newGeneration.reserve(m_population.size());

    // Elitism: preserve top performers unchanged
    m_eliteCount = std::min(ELITE_COUNT, static_cast<int>(sortedPopulation.size()));
    for (int i = 0; i < m_eliteCount; ++i) {
        newGeneration.push_back(sortedPopulation[i]);
        // Reset fitness tracking for next generation
        newGeneration.back().damageDealtToPlayer = 0.0f;
        newGeneration.back().survivalTime = 0.0f;
        newGeneration.back().fitness = 0.0f;
    }

    // Generate offspring to fill the rest of the population
    while (static_cast<int>(newGeneration.size()) < m_populationSize) {
        EnemyDNA child;

        // Decide whether to crossover or clone
        if (static_cast<float>(rand()) / RAND_MAX < CROSSOVER_RATE) {
            // Select parents using tournament selection
            int parent1Idx = TournamentSelection(3);
            int parent2Idx = TournamentSelection(3);

            // Use uniform crossover (generally better for real-valued GA)
            child = UniformCrossover(m_population[parent1Idx], m_population[parent2Idx]);
        } else {
            // Clone a single parent
            int parentIdx = TournamentSelection(3);
            child = m_population[parentIdx];
        }

        // Apply mutation
        Mutate(child);

        // Reset fitness tracking
        child.damageDealtToPlayer = 0.0f;
        child.survivalTime = 0.0f;
        child.fitness = 0.0f;

        newGeneration.push_back(child);
    }

    // Save previous population for reference
    m_previousPopulation = m_population;

    // Update to new generation
    m_population = std::move(newGeneration);
    m_generation++;
    m_nextSpawnIndex = 0;

    // Gradually increase difficulty multiplier (caps at 1.5x after gen 10)
    m_difficultyMultiplier = 1.0f + std::min(0.5f, (m_generation - 1) * 0.05f);

    // Debug output
    std::cout << "[GA] Generation " << m_generation << " evolved" << std::endl;
    std::cout << "[GA]  Average fitness: " << GetAverageFitness() << std::endl;
    std::cout << "[GA]  Best fitness: " << GetBestFitness() << std::endl;
    std::cout << "[GA]  Difficulty multiplier: " << m_difficultyMultiplier << "x" << std::endl;

    return true;
}

float GeneticAlgorithm::GetAverageFitness() const {
    if (m_population.empty()) return 0.0f;

    float total = 0.0f;
    for (const auto& dna : m_population) {
        total += dna.fitness;
    }
    return total / m_population.size();
}

float GeneticAlgorithm::GetBestFitness() const {
    if (m_population.empty()) return 0.0f;

    float best = 0.0f;
    for (const auto& dna : m_population) {
        if (dna.fitness > best) {
            best = dna.fitness;
        }
    }
    return best;
}
