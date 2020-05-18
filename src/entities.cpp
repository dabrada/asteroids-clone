#include "entities.h"
#include <math.h>

Entity::Entity(sf::Texture const & texture, int texID)
{
	mSprite.setTexture(texture);
	textureID = texID;
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width/2, lb.height/2);
    mVelocity.x = mVelocity.y = 30.0f; 

    mRadius = 1.0f * std::min(lb.width/2, lb.height/2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
}

void Entity::update(sf::Time dt)
{
    auto pos = getPosition();
    float x = pos.x + mVelocity.x * dt.asSeconds();
    float y = pos.y + mVelocity.y * dt.asSeconds();
    setPosition(x, y);
}

sf::Vector2f& Entity::getVelocity()
{
	return mVelocity;
}

void Entity::checkBounds(int width, int height)
{
	if (getPosition().x > static_cast<float>(width)) {
		setPosition(0, getPosition().y);
	}
	else if (getPosition().x < 0) {
		setPosition(static_cast<float>(width), getPosition().y);
	}
	if (getPosition().y >static_cast<float>(height)) {
		setPosition(getPosition().x, 0);
	}
	else if (getPosition().y < 0) {
		setPosition(getPosition().x, static_cast<float>(height));
	}
}

bool Entity::outOfBounds(int width, int height)
{
	if (getPosition().x > static_cast<float>(width)
		|| getPosition().x < 0
		|| getPosition().y >static_cast<float>(height)
		|| getPosition().y < 0)
	{
		return true;
	}
	else
		return false;
}

bool Entity::collidedWith(const Entity &ent) const
{
	// calculate distance between entities' circle centers
	float distance = sqrtf(powf(getPosition().x - ent.getPosition().x, 2) +
								powf(getPosition().y - ent.getPosition().y, 2));
	if (distance <= mRadius + ent.mRadius) {
		return true;
	}
	else
		return false;
}

float Entity::generateRandomAngle(int upperBound)
{
	return static_cast<float>(rand() % upperBound) + 1;
}

Asteroid::Asteroid(sf::Texture const & texture, int texID, int width, int height) :
				   Entity(texture, texID), textureID(texID)
{
	type = 2;
	angle = generateRandomAngle(25);
	setRotation(angle);
	float x = static_cast<float>((rand() % width) + 1);
	float y = static_cast<float>((rand() % height) + 1);
	sf::Vector2f velocity(x / 12.f, static_cast<float>(pow(-1, rand() % 2) * y / 12));
	setPosition(x, y);
	setVelocity(velocity);
}

Asteroid::Asteroid(sf::Texture const & texture, int texID, int width, int height,
				   sf::Vector2f position) : Entity(texture, texID), textureID(texID)
{
	type = 2;
	angle = generateRandomAngle(25);
	setRotation(angle);
	float x = static_cast<float>((rand() % width) + 1);
	float y = static_cast<float>((rand() % height) + 1);
	sf::Vector2f velocity(x / 12.f, static_cast<float>(pow(-1, rand() % 2) * y / 12));
	setPosition(position.x, position.y);
	setVelocity(velocity);
}

Asteroid::Asteroid(sf::Texture const & texture, int textureID, sf::Vector2f position,
				   sf::Vector2f velocity) : Entity(texture, textureID), textureID(textureID)
{
	type = 2;
	angle = generateRandomAngle(25);
	setRotation(angle);
	setPosition(position.x + 15, position.y + 25);
	setVelocity(velocity);
}

void Asteroid::update(sf::Time dt)
{
	rotate(angle * dt.asSeconds());
	move(mVelocity.x * dt.asSeconds(), mVelocity.y * dt.asSeconds());
}

Player::Player(sf::Texture const & texture, int texID, int width, int height) :
			   Entity(texture, texID), mCurrentSpeed(0.f)
{
	setScale(sf::Vector2f(0.8f, 0.8f));
	type = 1;
	setPosition(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f);
	sf::Vector2f velocity(0.f, 0.f);
	setVelocity(velocity);
}

void Player::handlePlayerInput(sf::Keyboard::Key code, bool isPressed)
{
	 if (code == sf::Keyboard::Key::Up)
		 mIsMovingUp = isPressed;
	 if (code == sf::Keyboard::Key::Down)
		 mIsMovingDown = isPressed;
	 if (code == sf::Keyboard::Key::Left)
		 mIsMovingLeft = isPressed;
	 if (code == sf::Keyboard::Key::Right)
		 mIsMovingRight = isPressed;
	 if (code == sf::Keyboard::Key::Space)
		 mSpacePressed = isPressed;
 }

bool Player::inputStatus(sf::Keyboard::Key code)
{
	if (code == sf::Keyboard::Key::Up)
		return mIsMovingUp;
	else if (code == sf::Keyboard::Key::Down)
		return mIsMovingDown;
	else if (code == sf::Keyboard::Key::Left)
		return mIsMovingLeft;
	else if (code == sf::Keyboard::Key::Right)
		return mIsMovingRight;
	else if (code == sf::Keyboard::Key::Space)
		return mSpacePressed;
	else
		return false;
}

void Player::update(sf::Time dt)
{
	if (mIsMovingLeft) {
		mAngle -= mRotationSpeed * dt.asSeconds();
	}
	if (mIsMovingRight) {
		mAngle += mRotationSpeed * dt.asSeconds();
		if (mAngle > 360)
			mAngle = fmodf(mAngle, 360.f);
	}
	if (mIsMovingUp || mIsMovingLeft || mIsMovingRight) {
		if (mCurrentSpeed != 0) {
			mVelocity.y -= cos(mAngle * 3.1415f / 180.f) + ((mCurrentSpeed + mAcceleration) * dt.asSeconds());
			mVelocity.x += sin(mAngle * 3.1415f / 180.f) + ((mCurrentSpeed + mAcceleration) * dt.asSeconds());
		}
		else {
			mVelocity.y -= cos(mAngle * 3.1415f / 180.f) + mAcceleration * dt.asSeconds();
			mVelocity.x += sin(mAngle * 3.1415f / 180.f) + mAcceleration * dt.asSeconds();
		}
	}
	// the ship will gradually slow down when DOWN arrow is pressed
	if (mIsMovingDown) {
		mVelocity.x *= 0.95f;
		mVelocity.y *= 0.95f;
	}
	mCurrentSpeed = sqrt(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);
	if (mCurrentSpeed > maxSpeed) {
		mVelocity.x *= maxSpeed / mCurrentSpeed;
		mVelocity.y *= maxSpeed / mCurrentSpeed;
	}
	setPosition(getPosition().x + mVelocity.x, getPosition().y + mVelocity.y);
	setRotation(mAngle);
}

const float Player::getAngle()
{
	return mAngle;
}

const int Player::getLives()
{
	return remainingLives;
}

void Player::updateLives(const int &lives)
{
	remainingLives = lives;
}

void Player::reposition(float x, float y, sf::Vector2f velocity, float angle)
{
	setPosition(x, y);
	setVelocity(velocity);
	mAngle = angle;
	setRotation(mAngle);
}

Bullet::Bullet(sf::Texture const & texture, int texID, sf::Vector2f position, sf::Vector2f velocity,
			   float angle) : Entity(texture, texID), mAngle(angle), offsetX(0), offsetY(0)
{
	setScale(sf::Vector2f(0.8f, 0.8f));
	type = 3;
	setPosition(position);
	rotate(angle - 90);
	setVelocity(velocity);
}

void Bullet::update(sf::Time dt)
{
	offsetX = sin(mAngle * 3.1415f / 180.f);
	offsetY = -cos(mAngle * 3.1415f / 180.f);
	move((mVelocity.x + offsetX * speed) * dt.asSeconds(), (mVelocity.y + offsetY * speed) * dt.asSeconds());
}