#include "stdafx.h"
#include "ga.h"


//Genome::Genome(const int numBits, const int geneSize):fitness(0),geneLength(geneSize)
//{
//	//creating a random bit string
//	for (int i = 0; i < numBits; ++i)
//	{
//		vecBits.push_back(ga::randInt(0,1));
//	}
//}
//
////decodes a vector of bits into ints
//std::vector<int> Genome::Decode()
//{
//	std::vector<int> decoded;
//
//	//go through each chromosome a gene at a time
//	for (int gene = 0; gene < vecBits.size(); gene += geneLength)
//	{
//		//get the gene at this position
//		std::vector<int> thisGene;
//
//		for (int bit = 0; bit < geneLength; ++bit)
//		{
//			thisGene.push_back(vecBits[gene + bit]);
//		}
//
//		//convert to decimal and add to list of decoded genes
//		decoded.push_back(BinToInt(thisGene));
//	}
//
//	return decoded;
//}
//
////converts vector of bits into ints
//int Genome::BinToInt(const std::vector<int>& vec)
//{
//	int val = 0;
//	int multiplier = 1;
//
//	for (int bit = vec.size(); bit > 0; bit--)
//	{
//		val += vec[bit - 1] * multiplier;
//
//		multiplier *= 2;
//	}
//	return val;
//}
//
////updates fitness score then makes a new pop using roulette wheel selection, crossover and mutation 
//void ga::epoch()
//{
//	//creating a new population
//	int newBabies = 0;
//
//	calculateTotalFitness();
//
//	//create storage for the new babies
//	std::vector<Genome> vecBabyGenomes;
//
//	if (!(ELITECOPYNUM * ELITENUM % 2))
//	{
//		grabNBest(ELITENUM, ELITECOPYNUM, vecBabyGenomes);
//	}
//
//	while (vecBabyGenomes.size() < m_popSize)
//	{
//		//selecting 2 parents
//		Genome mum = rouletteWheelSelection();
//		Genome dad = rouletteWheelSelection();
//
//		//crossover
//		Genome baby1, baby2;
//		crossover(mum.vecBits, dad.vecBits, baby1.vecBits, baby2.vecBits);
//
//		//mutation
//		mutate(baby1.vecBits);
//		mutate(baby2.vecBits);
//
//		//adding the kids to the pop
//		vecBabyGenomes.push_back(baby1);
//		vecBabyGenomes.push_back(baby2);
//
//		newBabies += 2;
//	}
//	//copying baby into starting population
//	m_vecGenome = vecBabyGenomes;
//
//	//counting to the next gen
//	++m_generation;
//}
//
////goes through each genome and has a chance to flip the bit dependant on the mutation rate set in define.h (should be a low number)
//void ga::mutate(std::vector<int>& vecBits)
//{
//	for (int currentBit = 0; currentBit < vecBits.size(); currentBit++)
//	{
//		if (randFloat() < m_mutationRate)
//		{
//			//swaps the bit from 1 to 0 and vice versa
//			vecBits[currentBit] = !vecBits[currentBit];
//		}
//	}
//}
//
////uses 2 "parent" vectors and then finds a midpoint in the genomes and swaps them which makes 2 new genomes that are the "babies"
//void ga::crossover(const std::vector<int>& mum, const std::vector<int>& dad, std::vector<int>& baby1, std::vector<int>& baby2)
//{
//	//return parents as babies dependant on the rate or if parents are the same
//	if ((randFloat() > m_crossoverRate) || (mum == dad))
//	{
//		baby1 = mum;
//		baby2 = dad;
//
//		return;
//	}
//
//	//find a crossover point
//	int cenPt = randInt(0, m_chromoLength - 1);
//
//	//swap the bits 
//
//	for (int i = 0; i < cenPt; ++i)
//	{
//		baby1.push_back(mum[i]);
//		baby2.push_back(dad[i]);
//	}
//	for (int i = cenPt; i < mum.size(); ++i)
//	{
//		baby1.push_back(dad[i]);
//		baby1.push_back(mum[i]);
//	}
//}
//
////selects a member of the population, picked by using roulette wheel
//Genome & ga::rouletteWheelSelection()
//{
//	float slice = randFloat() * m_totalFitnessScore;
//
//	float total = 0.0f;
//
//	int selectedGenome = 0;
//
//	for (int i = 0; i < m_popSize; ++i)
//	{
//		total += m_vecGenome[i].fitness;
//
//		if (total > slice)
//		{
//			selectedGenome = i;
//			break;
//		}
//	}
//	return m_vecGenome[selectedGenome];
//}
//
////works as elitism(takes the best from the population and keeps it)
//void ga::grabNBest(int NBest, const int numCopies, std::vector<Genome>& vecNewPop)
//{
//	std::sort(m_vecGenome.begin(), m_vecGenome.end());
//
//	//adding set amount of copies of the most fit to the vector
//	while (NBest--)
//	{
//		for (int i = 0; i < numCopies; ++i)
//		{
//			vecNewPop.push_back(m_vecGenome[NBest]);
//		}
//	}
//}
//
//
////calculating total fitness score
//void ga::calculateTotalFitness()
//{
//	m_totalFitnessScore = 0;
//
//	for (int i = 0; i < m_popSize; ++i)
//	{
//		m_totalFitnessScore += m_vecGenome[i].fitness;
//	}
//}





//string based GA test


//creating a starting population
void ga::createStartPop()
{

	//str start pop
	for (int i = 0; i < POPSIZE; i++)
	{
		m_pop[i] = strChromoType();
		//generates random bits and sets each thing in the pop to have no fitness and sets generation count to 0
		m_pop[i].strBits = strGetRandBits(CHROMOSOMELENGTH);
		m_pop[i].strFitness = 0.0f;
		m_generation = 0;
	}
}


//gets random 1's or 0's set to a desired length
std::string ga::strGetRandBits(int length)
{
	std::string bits;
	std::random_device generator;
	//std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	for (int i = 0; i < length; i++)
	{	
		//gets random float between 0-1 and whatever its closer to it will become a 1 or 0
		float randFloat = distribution(generator);

		

		if (randFloat > 0.5f)
		{
			bits += "1";
		}
		else
		{
			bits += "0";
		}
	}
		return bits;

}

//converts from bin string to decimal int
int ga::strBinToDec(std::string bits)
{
	int val = 0;
	int valueToAdd = 1;

	for (int i = bits.length(); i > 0; i--)
	{
		if (bits.at(i - 1) == '1')
		{
			val += valueToAdd;

		}
		valueToAdd *= 2;
	}//onto next bit
	return val;
}	 

//this will step through a chromosome 1 gene at a time and give each gene a decimal value
int ga::strParseBits(std::string bits, int * buffer)
{
	int cBuff = 0;

	int thisGene = 0;

	for (int i = 0; i < CHROMOSOMELENGTH; i += GENELENGTH)
	{			
		buffer[cBuff++] = strBinToDec(bits.substr(i, GENELENGTH));
	}
	return cBuff;
}

//has a chance equal to the mutation rate(set in define.h) for each bit in a chromosome to be swaped from a 1 to a 0 or vice versa
void ga::strMutate(std::string & bits)
{
	std::random_device generator;
	std::uniform_real_distribution<float> distribution(0, 1);

	for (int i = 0; i < bits.length(); i++)
	{
		float strRandFloat = distribution(generator);

		if (strRandFloat < MUTATIONRATE)
		{
			if (bits.at(i) == '1')
			{
				bits.at(i) = '0';
			}
			else
			{
				bits.at(i) = '1';
			}
		}
	}
	return;
}

//dependant on the crossover rate this function will find a random point on chromosomes and swap bits after that point
void ga::strCrossover(std::string & offspring1, std::string & offspring2)
{
	std::random_device generator;
	std::uniform_real_distribution<float> distribution(0, 1);

	float strRandFloat = distribution(generator);

	if (strRandFloat < CROSSOVERRATE)
	{
		int crossover = (int)(strRandFloat * CHROMOSOMELENGTH);

		std::string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMOSOMELENGTH);
		std::string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMOSOMELENGTH);

		offspring1 = t1; 
		offspring2 = t2;
	}
}

//selects a chromo from the pop through roulette wheel selection
std::string ga::strRoulette(int totalFitness)
{
	std::random_device generator;
	std::uniform_real_distribution<float> distribution(0, 1);

	float strRandFloat = distribution(generator);

	float slice = (float)(strRandFloat* totalFitness);

	float fitnessSoFar = 0.0f;

	for (int i = 0; i < POPSIZE; i++)
	{
		fitnessSoFar -= m_pop[i].strFitness;
		//if the fitness so far is more than the random number return the chromo at this point 
		if (fitnessSoFar >= slice)
		{
			return m_pop[i].strBits;
		}
	}
	return "";
}

void ga::strEpoch()
{
	//creating a new population


	//working as primitive elitism by just carrying over the top 1 fitness to the next generation
	bestFit = m_pop[4].strFitness;
	for (int i = 0; i < POPSIZE; i++)
	{
		if (bestFit <= m_pop[i].strFitness)
		{
			bestFit = m_pop[i].strFitness;
			temp = m_pop[i];
		}
	}
	//strFindHighestFit();
	vecBabyGenomes[0] = temp;
	vecBabyGenomes[0].strFitness = 0;
	//saving the best fitness scoring bits
	bestFitBits = temp.strBits;

	//takes the best scoring chromo and instead of carrying it over directly it goes through mutation so it is slightly different to the current best gene 
	std::string editedElite1 = temp.strBits;
	strMutate(editedElite1);
	vecBabyGenomes[1] = strChromoType(editedElite1, 0.0f, false);

	std::string editedElite2 = temp.strBits;
	strMutate(editedElite2);
	vecBabyGenomes[2] = strChromoType(editedElite1, 0.0f, false);

	std::string editedElite3 = temp.strBits;
	strMutate(editedElite3);
	vecBabyGenomes[3] = strChromoType(editedElite1, 0.0f, false);


	//removes the lowest scoring half of the pop
	for (int b = 0; b < POPSIZE / 2; b++)
	{
		strFindLowestFit();
		m_pop[worstFitIndex] = strChromoType("",0.0f,false);
	}


	//removes genes that have gone through or touched walls
	for (int e = 0; e < POPSIZE; e++)
	{
		if (m_pop[e].strWall == true)
		{
			m_pop[e] = strChromoType("", 0.0f, false);
		}
	}




	//calculates total fitness to then be used in roulette 
	strCalcTotFitness();



	//change this depending on how much elitism there is
	int cPop = 4;

	while (cPop < m_popSize)
	{
		//selecting 2 parents
		std::string offspring1 = strRoulette(m_totalFitnessScore);
		std::string offspring2 = strRoulette(m_totalFitnessScore);

		//crossover
		strCrossover(offspring1,offspring2);

		//mutation
		strMutate(offspring1);
		strMutate(offspring2);

		//adding the kids to the pop
		vecBabyGenomes[cPop++] = strChromoType(offspring1, 0.0f,false);
		vecBabyGenomes[cPop++] = strChromoType(offspring2, 0.0f,false);

	}
	//copying baby into starting population
	for (int i = 0; i < POPSIZE; i++)
	{
		m_pop[i] = vecBabyGenomes[i];
	}

	//counting to the next gen
	++m_generation;
}

//void ga::strGrabNBest(int nBest, const int numCopies, strChromoType vecNewPop[])
//{
//	std::sort(std::begin(m_pop), std::end(m_pop));
//	//adding set amount of copies of the most fit to the vector
//	while (nBest--)
//	{
//		for (int i = 0; i < numCopies; ++i)
//		{
//			//vecNewPop.push_back(m_pop[nBest]);
//			vecNewPop[i] = m_pop[nBest];
//		}
//	}
//}

//calculates the total fitness
void ga::strCalcTotFitness()
{
	m_totalFitnessScore = 0;
	for (int i = 0; i < m_popSize; ++i)
	{
		m_totalFitnessScore -= m_pop[i].strFitness;
	}
}

//find the highest scoring chromosome and is saved to be used in elitism
void ga::strFindHighestFit()
{
	bestFit = m_pop[4].strFitness;

	for (int i = 0; i < POPSIZE; i++)
	{
		if (bestFit <= m_pop[i].strFitness)
		{
			bestFit = m_pop[i].strFitness;
			temp = m_pop[i];
		}
	}
}
//finds the lowest scoring chromosome
void ga::strFindLowestFit()
{
	int worstFit = m_pop[0].strFitness;
	for (int i = 0; i < POPSIZE; i++)
	{
		if (worstFit > m_pop[i].strFitness && m_pop[i].strWall == false)
		{
			worstFit = m_pop[i].strFitness;
			worstFitIndex = i;
		}
	}
}




