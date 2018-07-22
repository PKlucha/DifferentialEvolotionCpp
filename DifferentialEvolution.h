#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "FunctionToOpt.h"

typedef std::vector<double> entity;

class DifferentialEvolution {
private:
	double F;						// Mutation factor, constant from 0 to 2 [def: 0.8]
	double Cr;						// Crossover rate (0 - 1) -> Mo¿e byæ sta³¹ lub zmieniaæ siê w czasie [def: 0.9]
	int numberOfParameters;		
	std::vector<entity> population;
	std::vector<entity> hallOfFame;	// Lista najlepszych osobnikow w danych populacjach
	FunctionToOpt *func;
	std::default_random_engine random_generator;

	bool MINISKNOWN;
	double knownMinimum;

public:
	// Konstruktor bez precyzowania znanego minimum,
	// obliczenia bêd¹ wykonywane a¿ nie skoñcz¹ siê iteracje
	DifferentialEvolution(FunctionToOpt &function, int populationSize /** NP */) : F(0.8), Cr(0.9),  func(&function), MINISKNOWN(false){
		srand(time(NULL));
		random_generator.seed(rand());
		numberOfParameters = func->getNumOfParameters();

		initPopulation(populationSize);
	}
	// Podczas tworzenia obiektu podajemy znane minimum funkcji
	// Program skoñczy pracê gdy osi¹gnie zadany próg b³êdu
	DifferentialEvolution(FunctionToOpt &function, int populationSize /** NP */, double knownMin) 
			: F(0.8), Cr(0.5), func(&function), MINISKNOWN(true), knownMinimum(knownMin) {
		srand(time(NULL));
		random_generator.seed(rand());
		numberOfParameters = func->getNumOfParameters();

		initPopulation(populationSize);
	}

	void initPopulation(int populationSize);
	void optimalization(int iterations);
	entity bestFitness();
};

void DifferentialEvolution::initPopulation(int pupulationSize) {
	std::uniform_real_distribution<> distribution(func->getBottomConstrain(), func->getTopConstrain());
	for (int i = 0; i < pupulationSize; i++) {
		entity tmp;
		for (int j = 0; j < numberOfParameters; j++) {
			tmp.push_back(distribution(random_generator));
			//std::cout << tmp[j] << " ";
		}
		//std::cout << func->evaluateFitness(tmp) << std::endl;
		population.push_back(tmp);
	}

	// Szacowanie najlepszego osobnika w pocz¹tkowej populacji
	hallOfFame.push_back(bestFitness());
	std::cout << "Initial population best fitness: " << func->evaluateFitness(hallOfFame[0]) << std::endl;
}
void DifferentialEvolution::optimalization(int iterations) {
	for (int i = 0; i < iterations; i++) {
		std::uniform_real_distribution<> distribution(0, population.size());
		for (int mem = 0; mem < population.size(); mem++) {
			// Losowanie indeksów do mutacji	
			int a = distribution(random_generator);
			int b = distribution(random_generator);
			int c = distribution(random_generator);
			// a, b, c && x musz¹ byæ ró¿ne 
			while (a == mem || b == mem || c == mem || a == b || a == c || b == c) {
				a = distribution(random_generator);
				b = distribution(random_generator);
				c = distribution(random_generator);
			}

			// Obliczanie wektora mutacji (Mutacja)
			entity mutantVector(numberOfParameters);
			for (int j = 0; j < numberOfParameters; j++) {
				// Vig = Xrir1g + F * (Xir2g - Xir3g)
				mutantVector[j] = population[a][j] + F * (population[b][j] - population[c][j]);

				// Czy wartoœci wektora mutacji nie wychodz¹ poza przedzia³ poszukiwañ
				if (mutantVector[j] < func->getBottomConstrain() || mutantVector[j] > func->getTopConstrain()) {
					mutantVector[j] = population[mem][j];
				}
			}

			// Generowanie wektora losowych r [if r[j] < Cr than crossover]
			std::vector<double> r(numberOfParameters);
			std::uniform_real_distribution<double> rDistribution(0, 1);
			for (int j = 0; j < numberOfParameters; j++) {
				r[j] = (double)rand() / (double)RAND_MAX;
			}

			// Losowy R [if R == j than crossover]
			std::uniform_real_distribution<double> RDist(0, numberOfParameters);
			int R = RDist(random_generator);

			// Binomial (Uniform) Crossover (Recombination) -> Najpopularniejsza metoda
			entity newMember(numberOfParameters);
			newMember = population[mem];
			for (int j = 0; j < numberOfParameters; j++) {
				if (r[j] <= Cr || j == R) {
					newMember[j] = mutantVector[j];
				}
			}

			// Selekcja
			if (func->evaluateFitness(newMember) < func->evaluateFitness(population[mem])) {
				population[mem] = newMember;
			}
		}

		// Podsumowanie
		entity best(numberOfParameters);
		best = bestFitness();

		std::cout << std::endl << std::endl << "Population " << i + 1 << " best: Points: ";
		for (int k = 0; k < numberOfParameters; k++) {
			std::cout << best[k] << " ";
		}
		std::cout << " Fitness: " << func->evaluateFitness(best) << std::endl;
		hallOfFame.push_back(best);
	}
}
entity DifferentialEvolution::bestFitness() {
	double best = func->evaluateFitness(population[0]);
	int bestIndex = 0;
	for (int i = 1; i < population.size(); i++) {
		if (func->evaluateFitness(population[i]) < best) {
			best = func->evaluateFitness(population[i]);
			bestIndex = i;
		}
	}
	return population[bestIndex];
}