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

	//player circle
	for (int p = 0; p < 26; ++p)
	{
		m_player.push_back(new Sphere(glm::vec2(-70, -40), glm::vec2(0, 0), 5, 2, glm::vec4(0, 0, 1, 1)));
		m_physicsScene->addActor(m_player[p]);
	}
	//goal
	m_goal = new Sphere(glm::vec2(70, 40), glm::vec2(0, 0), 99999999, 2, glm::vec4(0, 1, 0, 1));
	m_physicsScene->addActor(m_goal);

	//edge of the map
	Plane* boundary1 = new Plane(glm::vec2(0, 1), -55);
	Plane* boundary2 = new Plane(glm::vec2(0, -1), -55);
	Plane* boundary3 = new Plane(glm::vec2(1, 0), -95);
	Plane* boundary4 = new Plane(glm::vec2(-1, 0), -95);

	m_physicsScene->addActor(boundary1);
	m_physicsScene->addActor(boundary2);
	m_physicsScene->addActor(boundary3);
	m_physicsScene->addActor(boundary4);

	
	//walls for maze
	Square* wall1 = new Square(glm::vec2(40, 50), glm::vec2(0, 0), 99999999, glm::vec2(5, 5), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall1);
	Square* wall2 = new Square(glm::vec2(-10, -50), glm::vec2(0, 0), 99999999, glm::vec2(5, 15), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall2);
	Square* wall3 = new Square(glm::vec2(30, 20), glm::vec2(0, 0), 99999999, glm::vec2(5, 4), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall3);
	Square* wall4 = new Square(glm::vec2(20, -30), glm::vec2(0, 0), 99999999, glm::vec2(4, 3), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall4);

	//genetic algorithm initialisation
	m_ga = new ga(CROSSOVERRATE, MUTATIONRATE, POPSIZE, CHROMOSOMELENGTH, GENELENGTH);
		
	//m_vecPop = m_ga->getGenome();
	
	strEpoch();
	return true;
}

void mazeApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	
	delete m_physicsScene;
}

void mazeApp::update(float deltaTime) {
	m_timer = deltaTime;
	// input example
	aie::Input* input = aie::Input::getInstance();
	
	aie::Gizmos::clear();
	

	m_physicsScene->update(deltaTime);
	m_physicsScene->updatedGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	
	//epoch();
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
	int buffer[(int)(CHROMOSOMELENGTH / GENELENGTH)];
	//int buffer = CHROMOSOMELENGTH / GENELENGTH;
	int elementsNum = m_ga->strParseBits(bits, buffer);


	/*
	5=upLeft	3= up		6= upRight
	1= left				    2= right
	8=downLeft	4= down		7= downRight
	*/
	for (int i = 0; i < elementsNum - 1; i += 2)
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

void mazeApp::aiMove(Sphere* test)
{
	
	for (int i = 0; i < m_moveOrder.size(); i++)
	{
		if (m_timer > 0.5f)
		{
			for (int p = 0; p < 26; ++p)
			{
				if (m_moveOrder[i] == 1)
				{
					test[p].applyForce(glm::vec2(-3, 0));//left
				}
				else if (m_moveOrder[i] == 2)
				{
					test[p].applyForce(glm::vec2(3, 0));//right
				}
				else if (m_moveOrder[i] == 3)
				{
					test[p].applyForce(glm::vec2(0, 3));//up
				}
				else if (m_moveOrder[i] == 4)
				{
					test[p].applyForce(glm::vec2(0, -3));//down
				}
				else if (m_moveOrder[i] == 5)
				{
					test[p].applyForce(glm::vec2(-3, 3));//up left
				}
				else if (m_moveOrder[i] == 6)
				{
					test[p].applyForce(glm::vec2(3, 3));//up right
				}
				else if (m_moveOrder[i] == 7)
				{
					test[p].applyForce(glm::vec2(3, 3));//down right
				}
				else if (m_moveOrder[i] == 8)
				{
					test[p].applyForce(glm::vec2(-3, -3));//down left
				}
				m_timer = 0;
			}
		}
	}
	fitnessCalc();
}

int mazeApp::fitnessCalc()
{
	for (int p = 0; p < 26; ++p)
	{
		strFitness = -glm::distance(m_player[p]->getPosition(), m_goal->getPosition());
	}
	return strFitness;
}


void mazeApp::strEpoch()
{
	strChromoType Population[POPSIZE];

	for (int i = 0; i < POPSIZE; i++)
	{
		Population[i].strBits = m_ga->strGetRandBits(CHROMOSOMELENGTH);
		Population[i].strFitness = 0.0f;
	}

	int generation = 0;

	m_found = false;

	while (!m_found)
	{
		float totalFitness = 0.0f;

		for (int i = 0; i < POPSIZE; i++)
		{
			//
			strMovement(Population[i].strBits);
			aiMove(m_player[i]);
		}
	}
}



