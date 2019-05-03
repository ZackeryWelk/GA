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

//decodes a vector of bits into ints
std::vector<int> Genome::Decode()
{
	std::vector<int> decoded;

	//go through each chromosome a gene at a time
	for (int gene = 0; gene < vecBits.size(); gene += geneLength)
	{
		//get the gene at this position
		std::vector<int> thisGene;

		for (int bit = 0; bit < geneLength; ++bit)
		{
			thisGene.push_back(vecBits[gene + bit]);
		}

		//convert to decimal and add to list of decoded genes
		decoded.push_back(BinToInt(thisGene));
	}

	return decoded;
}

//converts vector of bits into ints
int Genome::BinToInt(const std::vector<int>& vec)
{
	int val = 0;
	int multiplier = 1;

	for (int bit = vec.size(); bit > 0; bit--)
	{
		val += vec[bit - 1] * multiplier;

		multiplier *= 2;
	}
	return val;
}

void ga::epoch()
{
}

//goes through each genome and has a chance to flip the bit dependant on the mutation rate set in define.h (should be a low number)
void ga::mutate(std::vector<int>& vecBits)
{
	for (int currentBit = 0; currentBit < vecBits.size(); currentBit++)
	{
		if (randFloat() < m_mutationRate)
		{
			//swaps the bit from 1 to 0 and vice versa
			vecBits[currentBit] = !vecBits[currentBit];
		}
	}
}

//uses 2 "parent" vectors and then finds a midpoint in the genomes and swaps them which makes 2 new genomes that are the "babies"
void ga::crossover(const std::vector<int>& mum, const std::vector<int>& dad, std::vector<int>& baby1, std::vector<int>& baby2)
{
	//return parents as babies dependant on the rate or if parents are the same
	if ((randFloat() > m_crossoverRate) || (mum == dad))
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//find a crossover point
	int cenPt = randInt(0, m_chromoLength - 1);

	//swap the bits 

	for (int i = 0; i < cenPt; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}
	for (int i = cenPt; i < mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby1.push_back(mum[i]);
	}
}

//selects a member of the population, picked by using roulette wheel
Genome & ga::rouletteWheelSelection()
{
	float slice = randFloat() * m_totalFitnessScore;

	float total = 0.0f;

	int selectedGenome = 0;

	for (int i = 0; i < m_popSize; ++i)
	{
		total += m_vecGenome[i].fitness;

		if (total > slice)
		{
			selectedGenome = i;
			break;
		}
	}
	return m_vecGenome[selectedGenome];
}

//works as elitism(takes the best from the population and keeps it)
void ga::grabNBest(int NBest, const int numCopies, std::vector<Genome>& vecNewPop)
{
	std::sort(m_vecGenome.begin(), m_vecGenome.end());

	//adding set amount of copies of the most fit to the vector
	while (NBest--)/*?????????????????????????????????????????????????????????????????????????????????*/
	{
		for (int i = 0; i < numCopies; ++i)
		{
			vecNewPop.push_back(m_vecGenome[NBest]);
		}
	}
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
	m_generation = 0;
	m_totalFitnessScore = 0;
}

//calculating total fitness score
void ga::calculateTotalFitness()
{
	m_totalFitnessScore = 0;

	for (int i = 0; i < m_popSize; ++i)
	{
		m_totalFitnessScore += m_vecGenome[i].fitness;
	}
}
