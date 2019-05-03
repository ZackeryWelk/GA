#pragma once
#include <vector>
#include <algorithm>

#include "utilities.h"
#include "define.h"


struct Genome
{
	std::vector<int> vecBits;

	float fitness;

	//number of bits in a gene
	int geneLength;

	Genome():fitness(0), geneLength(GENELENGTH){}

	Genome(const int numBits, const int geneSize);

	//decode each gene into decimal from bin
	std::vector<int> Decode();

	int BinToInt(const std::vector<int> &vec);

	//overload < for sorting
	friend bool operator<(const Genome& lhs, const Genome& rhs)
	{
		return (lhs.fitness > rhs.fitness);
	}
};


class ga
{
public:
	ga(float crossRat, float mutRat, int popSize, int numBits, int geneLength) :m_crossoverRate(crossRat),
																				m_mutationRate(mutRat),
																				m_popSize(popSize),
																				m_chromoLength(numBits),
																				m_totalFitnessScore(0.0f),
																				m_generation(0),
																				m_geneLength(geneLength)
	{
		createStartPop();
	}


	//updates fitness scores, creates a new pop using selection crossover and mutation.
	void epoch();

	//setter getter
	std::vector<Genome> getGenome() { return m_vecGenome; }
	void setGenome(std::vector<Genome> genome) { m_vecGenome = genome; }



private:
	std::vector<Genome> m_vecGenome;

	int m_popSize;

	float m_crossoverRate;
	float m_mutationRate;

	//how many bits per chromosome
	int m_chromoLength;

	//how many bit in a gene
	int m_geneLength;

	float m_totalFitnessScore;

	int m_generation;


	void mutate(std::vector<int> &vecBits);

	void crossover(const std::vector<int> &mum,
				   const std::vector<int> &dad,
						std::vector<int> &baby1,
						std::vector<int> &baby2);

	Genome&	rouletteWheelSelection();

	void grabNBest(int NBest, const int numCopies, std::vector<Genome> &vecNewPop);


	void createStartPop();

	void calculateTotalFitness();
};

