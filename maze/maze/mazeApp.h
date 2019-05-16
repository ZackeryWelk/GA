#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include "square.h"
#include <ga.h>
#include <math.h>


struct gaDirForce
{
	int x, y;
	float force;
};

class mazeApp : public aie::Application {
public:

	mazeApp();
	virtual ~mazeApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void epoch();
	void decode(Genome &gen, float &x, float &y);

	int getScreenWidth() { return screenWidth; }
	int getScreenHeight() { return screenHeight; }

protected:
	
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Sphere*				m_player;
	Sphere*				m_goal;
	PhysicsScene*		m_physicsScene;

	

	//genetic algorithm stuff
	std::vector<Genome>		m_vecPop;
	std::vector<gaDirForce> m_dirForce;
	ga*						m_ga;

	float					m_best;
	int						m_generation;

	int screenWidth;
	int screenHeight;
	

};