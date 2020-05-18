#include "playstate.h"
#include "entities.h"
#include "animation.h"
#include "game.h"

PlayState::PlayState(Game * pGame) : GameState(pGame)
{
	//printf("PlayState Ctor\n");
}

void PlayState::init()
{
	mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));

	mPlayer.reset(new Player(mTextureHolder.getTexture(Textures::player),
		static_cast<enum Textures::ID>(1), mpGame->windowWidth, mpGame->windowHeight));

	if(!mExplosions.empty())
		mExplosions.clear();

	playerLives.setString("Lives: " + std::to_string(mPlayer->getLives()));
	playerLives.setFont(mpGame->mFont);
	playerLives.setCharacterSize(24);
	playerLives.setPosition(20.f, playerLives.getLocalBounds().height);
	playerLives.setStyle(sf::Text::Regular);

	mScore = 0;
	playerScore.setString("Score: " + std::to_string(mScore));
	playerScore.setFont(mpGame->mFont);
	playerScore.setCharacterSize(24);
	playerScore.setPosition(mpGame->windowWidth - playerScore.getLocalBounds().width * 1.4f,
							playerScore.getLocalBounds().height);
	playerScore.setStyle(sf::Text::Regular);

	if (!mEntities.empty())
		mEntities.clear();
	for (int i = 0; i < numberOfAsteroids; ++i) {
		// large asteroids
		if (i % 3 == 0) {
			createRandomAsteroid(4, 7);
		}
		// medium asteroids
		else if (i % 3 == 1) {
			createRandomAsteroid(8, 9);
		}
		// small asteroids
		else {
			createRandomAsteroid(10, 11);
		}
	}
}

void PlayState::handleEvents(sf::Event event)
{
	// Close window : exit
	switch (event.type) {
	case sf::Event::Closed:
		mpGame->getWindow()->close();
		break;
	case sf::Event::KeyPressed:
		mPlayer->handlePlayerInput(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		mPlayer->handlePlayerInput(event.key.code, false);
		break;
	}
}

void PlayState::update(sf::Time dt)
{
	aliveAsteroids = 0;
	mPlayer->update(dt);
	// check if the player is out of bounds
	mPlayer->checkBounds(mpGame->windowWidth, mpGame->windowHeight);
	// fire rate control
	if (mPlayer->inputStatus(sf::Keyboard::Key::Space) &&
		playerFiredClock.getElapsedTime().asSeconds() > 0.25f)
	{
		createBullet(mTextureHolder.getTexture(Textures::bullet), 2, mPlayer->getPosition(),
			mPlayer->getVelocity(), mPlayer->getAngle());
		playerFiredClock.restart();
	}
	// checking bounds for asteroids and bullets
	for (auto it = mEntities.begin(); it != mEntities.end(); ) {
		(*it)->update(dt);
		// asteroids
		if ((*it)->type == 2) {
			aliveAsteroids++;
			(*it)->checkBounds(mpGame->windowWidth, mpGame->windowHeight);
			it++;
		}
		// bullets
		else if ((*it)->type == 3) {
			if ((*it)->outOfBounds(mpGame->windowWidth, mpGame->windowHeight)) {
				// remove bullet
				mEntities.erase(it++);
			}
			else {
				it++;
			}
		}
		//printf("izlazimo iz bound update\n");
	}
	// check for collisions between asteroids and bullets
	for (auto &asteroid : mEntities) {
		for (auto &bullet : mEntities) {
			if (asteroid->type == 2 && bullet->type == 3) {
				if (asteroid->collidedWith(*bullet)) {
					aliveAsteroids--;
					if ((asteroid->textureID - 4)*(asteroid->textureID - 7) <= 0) {
						mScore += 10;
						// create two small asteroids
						createAsteroidDebris(10, 11, asteroid->getPosition());
					}
					else if (asteroid->textureID == 8 || asteroid->textureID == 9) {
						mScore += 20;
					}
					else if (asteroid->textureID == 10 || asteroid->textureID == 11) {
						mScore += 40;
					}
					playerScore.setString("Score: " + std::to_string(mScore));
					sf::Vector2f bulletPos(bullet->getPosition().x, bullet->getPosition().y);
					createExplosion(mTextureHolder.getTexture(Textures::explosion), bullet->getPosition());
					asteroid->isAlive = bullet->isAlive = false;
				}
			}
		}
	}
	// check for collisions between player and asteroids
	for (auto &asteroid : mEntities) {
		if (asteroid->type == 2) {
			
			if (asteroid->collidedWith(* (mPlayer) )) {
				aliveAsteroids--;
				// create two small asteroids if player hit a big asteroid
				if ((asteroid->textureID - 4)*(asteroid->textureID - 7) <= 0)
					createAsteroidDebris(10, 11, asteroid->getPosition());
				if (mPlayer->getLives() > 0) {
					if (mPlayer->getLives() - 1 > 0) {
						sf::Vector2f velocity(0.f, 0.f);
						mPlayer->reposition(static_cast<float>(mpGame->windowWidth) / 2.f,
							static_cast<float>(mpGame->windowHeight) / 2.f, velocity, 0);
						mPlayer->updateLives(mPlayer->getLives() - 1);
						playerLives.setString("Lives: " + std::to_string(mPlayer->getLives()));
					}
					else {
						mPlayer->isAlive = false;
						mpGame->changeState = true;
						mpGame->newState = GameState::State::Score;
					}
				}
				createExplosion(mTextureHolder.getTexture(Textures::explosion), asteroid->getPosition());
				asteroid->isAlive = false;
			}
		}
	}
	for (auto it = mEntities.begin(); it != mEntities.end(); ) {
		if ((*it)->isAlive == false) {
			mEntities.erase(it++);
		}
		else {
			it++;
		}
	}
	// update explosions
	for (auto it = mExplosions.begin(); it != mExplosions.end(); ) {
		(*it)->update();
		if ((*it)->isAlive == false)
			mExplosions.erase(it++);
		else {
			it++;
		}
	}
	
	// change state
	if (aliveAsteroids == 0) {
		mpGame->changeState = true;
		mpGame->newState = GameState::State::Score;
	}
}

void PlayState::render()
{
	// Clear screen
    mpGame->getWindow()->clear();
	
	mpGame->getWindow()->draw(mBackgroundSprite);
	// Draw entities
	for (auto it = mEntities.begin(); it != mEntities.end(); ++it) {
		mpGame->getWindow()->draw(**it);
	}
	// Draw explosions
	for (auto it = mExplosions.begin(); it != mExplosions.end(); ++it) {
		mpGame->getWindow()->draw(**it);
	}
	mpGame->getWindow()->draw(playerLives);
	mpGame->getWindow()->draw(playerScore);
	mpGame->getWindow()->draw(*mPlayer);
	// Update the window
	mpGame->getWindow()->display();
}

// function which creates asteroids with textures in range [a, b] (from enum ID Textures)
void PlayState::createRandomAsteroid(int a, int b)
{
	enum Textures::ID texture = static_cast<enum Textures::ID>(a + (rand() % (b - a + 1)));
	mEntities.emplace_back(std::unique_ptr<Entity>(new Asteroid(mTextureHolder.getTexture(texture),
		static_cast<int>(texture), mpGame->windowWidth, mpGame->windowHeight)));
}

void PlayState::createAsteroidDebris(int a, int b, sf::Vector2f destructedAsteroidPosition)
{
	enum Textures::ID texture = static_cast<enum Textures::ID>(a + (rand() % (b - a + 1)));

	float x = static_cast<float>((rand() % mpGame->windowWidth) + 1);
	float y = static_cast<float>((rand() % mpGame->windowHeight) + 1);
	sf::Vector2f velocity(x / 12.f, y / 12);
	sf::Vector2f oppositeVelocity(velocity.x, -velocity.y);
	sf::Vector2f pos(destructedAsteroidPosition.x - static_cast<float>((rand() % 100) + 1),
		destructedAsteroidPosition.y - static_cast<float>((rand() % 100) + 1));

	mEntities.emplace_back(std::unique_ptr<Entity>(new Asteroid(mTextureHolder.getTexture(texture),
		static_cast<int>(texture), pos, velocity)));
	mEntities.emplace_back(std::unique_ptr<Entity>(new Asteroid(mTextureHolder.getTexture(texture),
		static_cast<int>(texture), pos, oppositeVelocity)));
}

void PlayState::createBullet(sf::Texture &texture, int texID, sf::Vector2f position,
	sf::Vector2f velocity, float angle)
{
	mEntities.emplace_back(std::unique_ptr<Entity>(new Bullet(texture, texID, position, velocity, angle)));
}

void PlayState::createExplosion(sf::Texture &texture, sf::Vector2f position)
{
	mExplosions.emplace_back(new Animation(mTextureHolder.getTexture(Textures::explosion),
		53, 4, 4, 256, 256, 0.3f, position));
}