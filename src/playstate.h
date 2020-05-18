#pragma once
#include <SFML/Graphics.hpp>
#include "entities.h"
#include "gamestate.h"
#include "textureHolder.h"
#include "animation.h"

class PlayState : public GameState
{
public:
	friend class Game;
	friend class ScoreState;
protected:
	PlayState(Game * pGame);
	virtual ~PlayState() {}
	void init() override;
	void handleEvents(sf::Event event) override;
	void update(sf::Time dt = sf::Time::Zero) override;
	void render() override;

	// helper methods
	void createRandomAsteroid(int a, int b);
	void createAsteroidDebris(int a, int b, sf::Vector2f destructedAsteroidPosition);
	void createBullet(sf::Texture &texture, int texID, sf::Vector2f position,
					  sf::Vector2f velocity, float angle);
	void createExplosion(sf::Texture &texture, sf::Vector2f position);

	TextureHolder mTextureHolder;
	sf::Sprite mBackgroundSprite;
	std::unique_ptr<Player> mPlayer;
	std::list<std::unique_ptr<Entity>> mEntities;
	std::list<std::unique_ptr<Animation>> mExplosions;
	
	const int numberOfAsteroids = 6;
	int aliveAsteroids = 0;
	sf::Text playerLives;
	sf::Text playerScore;
	int mScore = 0;
	// for limiting rate of fire
	sf::Clock playerFiredClock;
	float mVelocity = 120.0f; // pixel/sec
};
