#pragma once
#include "game.h"
#include "playstate.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <math.h>
#include <memory>

Game::Game() : mWindow(sf::VideoMode(windowWidth, windowHeight), "Asteroids"), mAllStates{new WelcomeState(this), new PlayState(this), new ScoreState(this)}
{
	// Load and check the availability of the font file
	if (!mFont.loadFromFile("neuropol_x_rg.ttf")) {
		printf("can't load font");
	}
    mDtFixed = sf::seconds(1.0f / 60.0f);
}

Game::~Game()
{
	for (auto it = mAllStates.begin(); it != mAllStates.end(); ++it) {
		delete (*it);
	}
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) {
		// Initialize everything for the new state
		if (changeState) {
			setState(newState);
			currentState->init();
			//changeState = false;
			//printf("inicijalizirano stanje\n");
		}
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate >= mDtFixed) {
			timeSinceLastUpdate -= mDtFixed;
			processEvents();
			update(mDtFixed);
			//printf("izasao iz updatea\n");
		}
		//printf("idem u render\n");
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		currentState->handleEvents(event);
	}
}

void Game::update(sf::Time dt)
{
	currentState->update(dt);
}

void Game::render()
{
	//printf("u game renderu sam\n");
	// Clear screen
    //mWindow.clear();
	// Render the current state
	currentState->render();
    // Update the window
    //mWindow.display();
}

sf::RenderWindow *Game::getWindow()
{
	return &mWindow;
}

void Game::setState(GameState::State newState)
{
	currentState = mAllStates[static_cast<int>(newState)];
	changeState = false;
}