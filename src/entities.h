#pragma once
#include <SFML/Graphics.hpp>
#include <list>

class Entity : public sf::Drawable, public sf::Transformable {
public:
    Entity(sf::Texture const & texture, int textID);
    virtual ~Entity() {}

	void setVelocity(sf::Vector2f const & vel) { mVelocity = vel; }
	virtual void update(sf::Time dt = sf::Time::Zero);
	sf::Vector2f& getVelocity();
	bool collidedWith(const Entity &ent) const;
	void checkBounds(int width, int height);
	bool outOfBounds(int width, int height);
	float generateRandomAngle(int x);
	bool isAlive = true;
	int type = 0; // player = 1, asteroid = 2, bullet = 3
	friend class Game;
	friend class PlayState;
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	sf::Sprite mSprite;
    sf::Vector2f mVelocity;
    sf::CircleShape mCircle;
    float mRadius;
	int textureID = -1;
};

class Player : public Entity {
public:
	Player(sf::Texture const & texture, int textID, int width, int height);
	~Player() {};

	void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
	virtual void update(sf::Time dt = sf::Time::Zero) override;
	bool inputStatus(sf::Keyboard::Key code);
	const float getAngle();
	const int getLives();
	void updateLives(const int &lives);
	void reposition(float x, float y, sf::Vector2f velocity, float angle);
private:
	float mAcceleration			= 2.f;
	const float maxSpeed		= 4.f;
	const float mRotationSpeed	= 150.f;
	float mCurrentSpeed			= 0.f;
	// remaining lives
	int remainingLives = 3;
	// rotation angle
	float mAngle = 0.f;
	
	bool mIsMovingUp	= false;
	bool mIsMovingDown	= false;
	bool mIsMovingLeft	= false;
	bool mIsMovingRight	= false;
	bool mSpacePressed	= false;
};

class Asteroid : public Entity {
public:
	Asteroid(sf::Texture const & texture, int textureID, int width, int height);
	Asteroid(sf::Texture const & texture, int textureID, int width, int height, sf::Vector2f position);
	Asteroid(sf::Texture const & texture, int textureID, sf::Vector2f position, sf::Vector2f velocity);
	~Asteroid() {};

	virtual void update(sf::Time dt = sf::Time::Zero) override;
	int textureID;
protected:
	// angle of rotation
	float angle;
};

class Bullet : public Entity {
public:
	Bullet(sf::Texture const & texture, int texID, sf::Vector2f position, sf::Vector2f velocity, float angle);
	~Bullet() {};

	virtual void update(sf::Time dt = sf::Time::Zero) override;
	const float speed = 620.f;
private:
	float mAngle;
	float offsetX, offsetY;
};
