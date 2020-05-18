#pragma once
#include <SFML/Graphics.hpp>
#include "textureHolder.h"
#include "entities.h"
#include "playstate.h"
#include <list>
#include <array>
#include <memory>

class Game{
public:
    Game();										// Ctor
	~Game();									// Dtor
    void run();
	friend class GameState;
	friend class WelcomeState;
	friend class ScoreState;
	friend class PlayState;
	
	const int windowHeight = 1024;
	const int windowWidth = 1280;
	bool changeState = true;
	GameState::State newState = GameState::State::Welcome;

protected:
    void processEvents();
    void update(sf::Time dt);
    void render();
	void setState(GameState::State newState);
	sf::RenderWindow *getWindow();

	std::array<GameState*, 3> mAllStates;
	GameState *currentState;

    sf::RenderWindow mWindow;
    TextureHolder mTextureHolder;

	sf::Font mFont;								// the font is the same in every state

    float mVelocity = 120.0f;					// pixel/sec
    sf::Time mDtFixed;
};
