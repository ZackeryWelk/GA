#include "mazeApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm/ext.hpp>

mazeApp::mazeApp() {

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

	m_player = new Sphere(glm::vec2(-20, -40), glm::vec2(0, 0), 5, 2, glm::vec4(0, 0, 1, 1));
	m_physicsScene->addActor(m_player);

	Plane* boundary1 = new Plane(glm::vec2(0, 1), -55);
	Plane* boundary2 = new Plane(glm::vec2(0, -1), -55);
	Plane* boundary3 = new Plane(glm::vec2(1, 0), -95);
	Plane* boundary4 = new Plane(glm::vec2(-1, 0), -95);

	m_physicsScene->addActor(boundary1);
	m_physicsScene->addActor(boundary2);
	m_physicsScene->addActor(boundary3);
	m_physicsScene->addActor(boundary4);



	Square* wall1 = new Square(glm::vec2(50, 50), glm::vec2(0, 0), 99999999, glm::vec2(10, 10), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall1);
	Square* wall2 = new Square(glm::vec2(-10, -50), glm::vec2(0, 0), 99999999, glm::vec2(5, 15), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall2);
	Square* wall3 = new Square(glm::vec2(30, 20), glm::vec2(0, 0), 99999999, glm::vec2(5, 4), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall3);
	Square* wall4 = new Square(glm::vec2(20, -30), glm::vec2(0, 0), 99999999, glm::vec2(4, 3), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(wall4);

	return true;
}

void mazeApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;

	delete m_physicsScene;
}

void mazeApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	
	aie::Gizmos::clear();
	
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_player->applyForce(glm::vec2(0, 2));
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_player->applyForce(glm::vec2(0, -2));
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_player->applyForce(glm::vec2(-2, 0));
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_player->applyForce(glm::vec2(2, 0));
	}

	m_physicsScene->update(deltaTime);
	m_physicsScene->updatedGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
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