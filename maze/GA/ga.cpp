#include "stdafx.h"
#include "ga.h"


Genome::Genome(const int numBits, const int geneSize):fitness(0),geneLength(geneSize)
{
	//creating a random bit string
	for (int i = 0; i < numBits; ++i)
	{
		vecBits.push_back(randInt(0, 1));
	}
}

std::vector<int> Genome::Decode()
{
	return std::vector<int>();
}

int Genome::BinToInt(const std::vector<int>& v)
{
	return 0;
}

void ga::epoch()
{
}

void ga::mutate(std::vector<int>& vecBits)
{
}

void ga::crossover(const std::vector<int>& mum, const std::vector<int>& dad, std::vector<int>& baby1, std::vector<int>& baby2)
{
}

Genome & ga::rouletteWheelSelection()
{
	// TODO: insert return statement here
}

void ga::grabNBest(int NBest, const int numCopies, std::vector<Genome>& vecNewPop)
{
}

//creating a starting population
void ga::createStartPop()
{
	m_vecGenome.clear();

	for (int i = 0; i < m_popSize; i++)
	{
		m_vecGenome.push_back(Genome(m_chromoLength, m_geneLength));
	}


	//resets all variables
	m_generation
}

void ga::calculateTotalFitness()
{
}
