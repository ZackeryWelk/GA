#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include "square.h"
#include <ga.h>


class mazeApp : public aie::Application {
public:

	mazeApp();
	virtual ~mazeApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Sphere*				m_player;
	PhysicsScene*		m_physicsScene;

};