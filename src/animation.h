#pragma once
#include "entities.h"

class Animation : public Entity {
public:
	Animation(sf::Texture const & texture, int texID, int rows, int cols, int width, int height, float vel, sf::Vector2f position);
	virtual void update(sf::Time dt = sf::Time::Zero) override;
private:
	int mRows, mCols, mWidth, mHeight;
	float mSpeed;
	float frameNumber = 0.f;
	float mCurrentCol;
	int   mCurrentRow;
};