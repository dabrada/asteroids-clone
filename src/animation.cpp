#pragma once
#include "animation.h"

Animation::Animation(sf::Texture const & texture, int texID, int rows, int cols,
	int width, int height, float vel, sf::Vector2f position) :
	Entity(texture, texID), mRows(rows), mCols(cols), mWidth(width / cols),
	mHeight(height / rows), mSpeed(vel), mCurrentCol(0.0f), mCurrentRow(0)
{
	mRadius = 0.25f * std::min(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2);
	mCircle.setRadius(mRadius);
	sf::Vector2f newPos(position.x + mWidth + mRadius, position.y + mHeight + mRadius);
	setPosition(newPos);
}

void Animation::update(sf::Time dt)
{
	if (!isAlive) return;
	mCurrentCol += mSpeed;
	frameNumber += 0.5f * mSpeed;
	if (static_cast<int>(mCurrentCol) == mCols) {
		mCurrentCol = 0.f;
		++mCurrentRow;
	}
	if (mCurrentRow == mRows && static_cast<int>(mCurrentCol) == mCols) {
		mCurrentCol = 0.f;
		mCurrentRow = 0;
		isAlive = false;
	}
	if (static_cast<int>(frameNumber) >= mRows * mCols) {
		isAlive = false;
	}
	else {
		int j = static_cast<int>(mCurrentCol);
		int k = mCurrentRow;
		mSprite.setTextureRect(sf::IntRect(j*mWidth, k*mHeight, mWidth, mHeight));
	}
}