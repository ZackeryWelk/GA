#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include "square.h"
#include <ga.h>
#include <math.h>
#include <Windows.h>


struct gaDirForce
{
	int x, y;
	float force;
};

class mazeApp : public aie::Application{
public:

	mazeApp();
	virtual ~mazeApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//void epoch();
	//void decode(Genome &gen, float &x, float &y);


	//string ga
	void strMovement(std::string bits);
	void aiMove(Sphere* test, strChromoType pop);
	//void aiPhysicsMove(Sphere* player);
	void strEpoch();
	//bool checkCol(Sphere* player);



	int getScreenWidth() { return screenWidth; }
	int getScreenHeight() { return screenHeight; }

protected:
	//string ga
	std::vector<int>	m_moveOrder;
	bool				m_found = false;
	float				strFitness;
	float				m_timer = 0.0f;
	bool				m_isMoving;

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Sphere*				m_startPoint;
	std::vector<Sphere*> m_player;
	std::vector<Square*> m_walls;
	std::vector<Plane*>  m_bounds;
	glm::vec2			m_playerSpawnPos;
	Sphere*				m_goal;
	Sphere*				m_bestPath;
	PhysicsScene*		m_physicsScene;

	char				m_playAfterFound;

	//genetic algorithm stuff
	//std::vector<Genome>		m_vecPop;


	std::vector<gaDirForce> m_dirForce;
	ga*						m_ga;

	float					m_best;
	int						m_genCountForSol = 0;
	int						m_numberOfMoves = 0;


	bool hasStarted = false;

	int screenWidth;
	int screenHeight;

	bool isRunning = false;
	bool isDone;


	int goalX;
	int goalY;
	glm::vec2 startGoalP;

};