#include "mazeApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>

#define BITMULTIPLIER pow(2,GENELENGTH)

mazeApp::mazeApp() {
	screenHeight = 720;
	screenWidth  = 1280;
}

mazeApp::~mazeApp() {

}

bool mazeApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0,0));
	m_physicsScene->setTimeStep(0.01f);

	//generating players 
	for (int p = 0; p < POPSIZE; ++p)
	{
		m_player.push_back(new Sphere(glm::vec2(-70, -40), glm::vec2(0, 0), 2, 1.5f, glm::vec4(0, 0, 1, 1)));
		m_physicsScene->addActor(m_player[p]);
		m_playerSpawnPos = m_player[p]->getPosition();
	}
	m_startPoint = new Sphere(glm::vec2(-70, -40), glm::vec2(0, 0), 2, 2, glm::vec4(1, 1, 1, 1));
	m_physicsScene->addActor(m_startPoint);

	m_bestPath = new Sphere(glm::vec2(-70, -40), glm::vec2(0, 0), 2, 1.5f, glm::vec4(1, 1, 1, 1));
	m_physicsScene->addActor(m_bestPath);
	
	std::random_device generator;
	//number of walls and where they can spawn
	std::uniform_int_distribution<int> distribution(0, 25);
	std::uniform_int_distribution<int> locDistributionX(-60, 60);
	std::uniform_int_distribution<int> locDistributionY(-40, 40);

	int wallCount = distribution(generator);
	//randomly generating wall positions
	for (int w = 0; w < wallCount; ++w)
	{
		int spawnX = locDistributionX(generator);
		int spawnY = locDistributionY(generator);
		m_walls.push_back(new Square(glm::vec2(spawnX,spawnY), glm::vec2(0, 0), 99999999, glm::vec2(3, 3), glm::vec4(1, 1, 0, 1)));
		m_physicsScene->addActor(m_walls[w]);
	}

	std::uniform_int_distribution<int>goalspawnXRand(0, 70);
	std::uniform_int_distribution<int>goalspawnYRand(-40, 40);

	int goalSpawnX = goalspawnXRand(generator);
	int goalSpawnY = goalspawnYRand(generator);
	m_goal = new Sphere(glm::vec2(goalSpawnX, goalSpawnY), glm::vec2(0, 0), 1, 2, glm::vec4(0, 1, 0, 1));
	m_physicsScene->addActor(m_goal);

	//edge of the map
	//m_bounds.push_back(new Plane(glm::vec2(0, 1), -55));
	//m_physicsScene->addActor(m_bounds[0]);
	//m_bounds.push_back(new Plane(glm::vec2(0, -1), -55));
	//m_physicsScene->addActor(m_bounds[1]);
	//m_bounds.push_back(new Plane(glm::vec2(1, 0), -95));
	//m_physicsScene->addActor(m_bounds[2]);
	//m_bounds.push_back(new Plane(glm::vec2(-1, 0), -95));
	//m_physicsScene->addActor(m_bounds[3]);


	//genetic algorithm initialisation
	m_ga = new ga(CROSSOVERRATE, MUTATIONRATE, POPSIZE, CHROMOSOMELENGTH, GENELENGTH);
	
	return true;
}

void mazeApp::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
	
	delete m_physicsScene;
}

void mazeApp::update(float deltaTime) {
	m_timer += deltaTime;
	// input example
	aie::Input* input = aie::Input::getInstance();
	
	//sets the goal position (doesnt really work, have to click around the bottom left of the screen)
//	if (input->wasMouseButtonPressed(0) && hasStarted == false)
//	{
//		int goalSpawnX;
//		int goalSpawnY;
//		goalSpawnX = input->getMouseX();
//		goalSpawnY = input->getMouseY();
//		
//		hasStarted = true;
//		m_goal = new Sphere(glm::vec2(goalSpawnX,goalSpawnY), glm::vec2(0, 0), 1, 2, glm::vec4(0, 1, 0, 1));
//		m_physicsScene->addActor(m_goal);
//
//	}
	//after the person clicks 
//	if (hasStarted == true)
//	{
		aie::Gizmos::clear();

		m_physicsScene->update(deltaTime);
		m_physicsScene->updatedGizmos();

		// exit the application
		if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
			quit();

		if (isRunning == false)
		{//after each generation resets the players positions
			for (int i = 0; i < m_player.size(); i++)
			{
				m_player[i]->setPosition(m_playerSpawnPos);
				m_startPoint->setPosition(m_playerSpawnPos);
				m_bestPath->setPosition(m_playerSpawnPos);
			}
			isRunning = true;
			strEpoch();
		}


//	}
	

}

void mazeApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	
	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

//void mazeApp::epoch()
//{
//	float bestFitness = -1000;
//	for (int p = 0; p <  m_vecPop.size(); ++p)
//	{ 	
//		//decode the genome
//		float x, y;
//
//		decode(m_vecPop[p], x, y);
//
//		for (int i = 0; i < m_vecPop.size(); ++i)
//		{
//			//placeholder distance measurement
//			float dist = -glm::distance(m_player->getPosition(), m_goal->getPosition());
//			m_vecPop[i].fitness = dist;
//			
//			if (dist > bestFitness)
//			{
//				bestFitness = dist;
//				m_best = i;
//			}
//		}//onto the next genome
//		//std::cout << bestFitness << m_generation << std::endl;
//	}
//	//now does and epoch of the genetic alg
//	//replace the genome
//	m_ga->setGenome(m_vecPop);
//
//	m_ga->epoch();
//
//	//get new genome
//	m_vecPop = m_ga->getGenome();
//
//	++m_generation;
//}

//void mazeApp::decode(Genome &gen, float &x, float &y)
//{
//	std::vector<int> decodedGen = gen.Decode();
//
//	//calculate the force and the direction
//	float conversionMulti = 0.1f / BITMULTIPLIER;
//
//	conversionMulti = 0.1f / BITMULTIPLIER;
//
//	x = decodedGen[0] * conversionMulti;
//	y = decodedGen[1] * conversionMulti;
//
//}

void mazeApp::strMovement(std::string bits)
{
	int buffer[(int)(CHROMOSOMELENGTH / GENELENGTH)]{ 0 };
	//counting up how many moves the user can do

	int elementsNum = m_ga->strParseBits(bits, buffer);

	//after converting binary to numbers ranging from 0-15, separates them into numbers 1-8 for each direction of movement
	/*
	8/9=   upLeft	4/5= up		10/11= upRight
	0/1=   left				    2/3=   right
	14/15= downLeft	6/7= down	12/13= downRight
	*/
	for (int i = 0; i < elementsNum; i++)
	{
		switch (buffer[i])
		{
			//left
		case 0:
			m_moveOrder.push_back(1);
			break;
		case 1:
			m_moveOrder.push_back(1);
			break;
			//right
		case 2:
			m_moveOrder.push_back(2);
			break;
		case 3:
			m_moveOrder.push_back(2);
			break;
			//up
		case 4:
			m_moveOrder.push_back(3);
			break;
		case 5:
			m_moveOrder.push_back(3);
			break;
			//down
		case 6:
			m_moveOrder.push_back(4);
			break;
		case 7:
			m_moveOrder.push_back(4);
			break;
			//up left
		case 8:
			m_moveOrder.push_back(5);
			break;
		case 9:
			m_moveOrder.push_back(5);
			break;
			//up right
		case 10:
			m_moveOrder.push_back(6);
			break;
		case 11:
			m_moveOrder.push_back(6);
			break;
			//down right
		case 12:
			m_moveOrder.push_back(7);
			break;
		case 13:
			m_moveOrder.push_back(7);
			break;
			//down left
		case 14:
			m_moveOrder.push_back(8);
			break;
		case 15:
			m_moveOrder.push_back(8);
			break;
		}
	}
}

void mazeApp::aiMove(Sphere* test, strChromoType pop)
{
	m_numberOfMoves = 0;
	//goes through the moveorder vector that was made up of numbers 1-8 from strMovement and makes each of them move the player
	for (int i = 0; i < m_moveOrder.size(); i++)
	{

			//left
			if (m_moveOrder[i] == 1)
			{
				glm::vec2 origPos = test->getPosition();
				//moves the player by a set amount
				test->setPosition(test->getPosition() - glm::vec2(-2, 0));
				//draws a line from the place it was to where the player is now
				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//right
			else if (m_moveOrder[i] == 2)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(2, 0));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//up
			else if (m_moveOrder[i] == 3)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(0, 2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//down
			else if (m_moveOrder[i] == 4)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(0, -2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//up left
			else if (m_moveOrder[i] == 5)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(-2, 2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//up right
			else if (m_moveOrder[i] == 6)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(2, 2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//down right
			else if (m_moveOrder[i] == 7)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(2, -2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}
			//down left
			else if (m_moveOrder[i] == 8)
			{
				glm::vec2 origPos = test->getPosition();
				test->setPosition(test->getPosition() - glm::vec2(-2, -2));

				if (pop.strBits == m_ga->bestFitBits)
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 1, 1, 1));
				}
				else
				{
					aie::Gizmos::add2DLine(origPos, test->getPosition(), glm::vec4(1, 0, 0, 1));
				}
			}

			//counts how many move the ai makes to get to the goal and will stop moving, will use this in fitness function to try and reduce amount of moves
			m_numberOfMoves++;
			if (glm::distance(test->getPosition(), m_goal->getPosition()) <= 3)
			{
				break;
			}

			//checks to see if when moving around the player hit a wall
			for (int w = 0; w < m_walls.size(); w++)
			{
				if (glm::distance(test->getPosition(), m_walls[w]->getPosition()) <= 6)
				{
					glm::vec2 topRight(m_walls[w]->getPosition().x + m_walls[w]->getExtents().x, m_walls[w]->getPosition().y + m_walls[w]->getExtents().y);
					glm::vec2 BottomLeft(m_walls[w]->getPosition().x - m_walls[w]->getExtents().x, m_walls[w]->getPosition().y - m_walls[w]->getExtents().y);

					glm::vec2 closestPoint = glm::clamp(test->getPosition(), BottomLeft, topRight);
					
					glm::vec2 displacement = test->getPosition() - closestPoint;

					float overlap = test->getRadius() - glm::length(displacement);
					if (overlap >= 0)
					{
						pop.strWall = true;
						pop.strFitness = -100000;
						return;
					}
				}
			}
	}
}

//void mazeApp::aiPhysicsMove(Sphere * player)
//{
//	for (int i = 0; i < m_moveOrder.size(); i++)
//	{
//		if (m_timer > 0.3f)
//		{
//			//left
//			if (m_moveOrder[i] == 1)
//			{
//				player->applyForce(glm::vec2(-10, 0));
//				m_timer = 0;
//				continue;
//			}
//			//right
//			else if (m_moveOrder[i] == 2)
//			{
//				player->applyForce(glm::vec2(10, 0));
//				m_timer = 0;
//				continue;
//			}
//			//up
//			else if (m_moveOrder[i] == 3)
//			{
//				player->applyForce(glm::vec2(0, 10));
//				m_timer = 0;
//				continue;
//			}
//			//down
//			else if (m_moveOrder[i] == 4)
//			{
//				player->applyForce(glm::vec2(0, -10));
//				m_timer = 0;
//				continue;
//			}
//			//up left
//			else if (m_moveOrder[i] == 5)
//			{
//				player->applyForce(glm::vec2(-10, 10));
//				m_timer = 0;
//				continue;
//			}
//			//up right
//			else if (m_moveOrder[i] == 6)
//			{
//				player->applyForce(glm::vec2(10, 10));
//				m_timer = 0;
//				continue;
//			}
//			//down right
//			else if (m_moveOrder[i] == 7)
//			{
//				player->applyForce(glm::vec2(10, -10));
//				m_timer = 0;
//				continue;
//			}
//			//down left
//			else if (m_moveOrder[i] == 8)
//			{
//				player->applyForce(glm::vec2(-10, -10));
//				m_timer = 0;
//				continue;
//			}
//			m_timer = 0;
//		}
//	}
//}


void mazeApp::strEpoch()
{
	float totalFitness = 0.0f;

	for (int i = 0; i < POPSIZE; i++)
	{

		//runs the functions to move the player
		strMovement(m_ga->m_pop[i].strBits);
		aiMove(m_player[i], m_ga->m_pop[i]);

		//when the player hits the goal
		if (glm::distance(m_player[i]->getPosition(), m_goal->getPosition()) <= 2.5)
		{

			m_ga->m_pop[i].strFitness = (-glm::distance(m_player[i]->getPosition(), m_goal->getPosition()) * 15) - m_numberOfMoves;
			//m_ga->m_pop[i].strFitness = 0;
			if (m_found == false)
			{
				std::cout << "found after " << m_ga->m_generation << " generations" << std::endl << std::endl;

			}
			m_found = true;

		}
		//if the player hit a wall
		else if (m_ga->m_pop[i].strWall == true)
		{
			m_ga->m_pop[i].strFitness = -10000;
		}
		else
		//fitness calc, distance is weighted more than number of moves
		{
			m_ga->m_pop[i].strFitness = (-glm::distance(m_player[i]->getPosition(), m_goal->getPosition())*20) - m_numberOfMoves;
		}

		//outputs the fitness of every player
		//std::cout << m_ga->m_pop[i].strFitness << std::endl;


		//clears the moveorder made for that player to then be filled by the next player
		m_moveOrder.clear();

	}

	//runs the GA
	m_ga->strEpoch();
	//outputs the generation number
	std::cout << "generation " << m_ga->m_generation << std::endl;
	//outputs the fitness of the best of each gen
	std::cout << "best fitness score " << m_ga->temp.strFitness << std::endl << std::endl;

	
	//tells the if statement in update that it can run another generation
	isRunning = false;
}

//bool mazeApp::checkCol(Sphere* player)
//{
//	for (int i = 0; i < POPSIZE; i++)
//	{
//		for (int w = 0; w < m_walls.size(); w++)
//		{
//			glm::vec2 topRight(m_walls[w]->getPosition().x + m_walls[w]->getExtents().x, m_walls[w]->getPosition().y + m_walls[w]->getExtents().y);
//			glm::vec2 BottomLeft(m_walls[w]->getPosition().x - m_walls[w]->getExtents().x, m_walls[w]->getPosition().y - m_walls[w]->getExtents().y);
//
//			glm::vec2 closestPoint = glm::clamp(player[i].getPosition(), BottomLeft, topRight);
//
//			glm::vec2 displacement = player[i].getPosition() - closestPoint;
//			
//			float overlap = player[i].getRadius() - glm::length(displacement);
//			if (overlap >= 0)
//			{
//				return true;
//			}
//		}
//		for (int b = 0; b < m_bounds.size(); b++)
//		{
//			glm::vec2 collisionNormal = m_bounds[b]->getNormal();
//			float sphereToPlane = glm::dot(m_player[i]->getPosition(), m_bounds[b]->getNormal()) - m_bounds[b]->getDistance();
//		
//		
//			float intersection = m_player[i]->getRadius() - sphereToPlane;
//			if (intersection >= 0)
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}


