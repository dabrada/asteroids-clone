#include "game.h"
#include "entities.h"

GameState::GameState(Game * pGame) : mpGame(pGame)
{

}

WelcomeState::WelcomeState(Game * pGame) : GameState(pGame)
{
	//printf("WelcomeState Ctor\n");
}

void WelcomeState::init()
{
	strTitle.setString("Asteroids clone by D. Bradaric");
	strTitle.setFont(mpGame->mFont);
	strTitle.setCharacterSize(26);
	float titleWidth = strTitle.getLocalBounds().width;
	float titleLeft = strTitle.getLocalBounds().left;
	float titleHeight = strTitle.getLocalBounds().height;
	float titleTop = strTitle.getLocalBounds().top;
	strTitle.setPosition( (static_cast<float>(mpGame->windowWidth) - titleWidth - titleLeft) / 2.f,
						   (static_cast<float>(mpGame->windowHeight) - titleHeight) / 2.7f);
	strTitle.setStyle(sf::Text::Regular);

	strSubtitle.setString("Press any key to continue");
	strSubtitle.setFont(mpGame->mFont);
	strSubtitle.setCharacterSize(20);
	strSubtitle.setPosition((static_cast<float>(mpGame->windowWidth) 
					- strSubtitle.getLocalBounds().width - strSubtitle.getLocalBounds().left) / 2.f,
					(static_cast<float>(mpGame->windowHeight) - titleHeight - titleTop) / 2.7f 
					+ titleHeight + 3.f * strSubtitle.getLocalBounds().top);
	strSubtitle.setStyle(sf::Text::Italic);
}

void WelcomeState::handleEvents(sf::Event event)
{
	// Close window : exit
	switch (event.type) {
		case sf::Event::Closed:
			mpGame->getWindow()->close();
			break;
		case sf::Event::KeyPressed:
			mpGame->newState = GameState::State::Playing;
			mpGame->changeState = true;
			break;
	}
}

void WelcomeState::update(sf::Time dt)
{

}

void WelcomeState::render()
{
	// Clear screen
	mpGame->getWindow()->clear();

	mpGame->getWindow()->draw(strTitle);
	mpGame->getWindow()->draw(strSubtitle);

	// Display screen
	mpGame->getWindow()->display();
}

ScoreState::ScoreState(Game * pGame) : GameState(pGame)
{
	//printf("ScoreState Ctor\n");
}

void ScoreState::init()
{
	score = dynamic_cast<PlayState*>(mpGame->mAllStates[static_cast<int>(GameState::Playing)])->mScore;

	finalScoreText.setString("Score: " + std::to_string(score));
	finalScoreText.setFont(mpGame->mFont);
	finalScoreText.setCharacterSize(24);
	finalScoreText.setStyle(sf::Text::Regular);
	
	continueText.setString("Enter to continue, Esc to finish");
	continueText.setFont(mpGame->mFont);
	continueText.setCharacterSize(24);
	continueText.setPosition((static_cast<float>(mpGame->windowWidth) - continueText.getLocalBounds().width) / 2.f,
		(static_cast<float>(mpGame->windowHeight)) / 2.6f + finalScoreText.getLocalBounds().height * 1.5f);
	continueText.setStyle(sf::Text::Regular);

	finalScoreText.setPosition(continueText.getPosition().x,
		(static_cast<float>(mpGame->windowHeight)) / 2.6f);
}

void ScoreState::handleEvents(sf::Event event)
{
	switch (event.type) {
		case sf::Event::Closed:
			mpGame->getWindow()->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				mpGame->getWindow()->close();
			}
			if (event.key.code == sf::Keyboard::Enter) {
				mpGame->changeState = true;
				mpGame->newState = GameState::State::Playing;
			}
			break;
	}
}

void ScoreState::update(sf::Time dt)
{

}

void ScoreState::render()
{
	// Clear screen
	mpGame->getWindow()->clear();

	mpGame->getWindow()->draw(finalScoreText);
	mpGame->getWindow()->draw(continueText);

	// Update the window
	mpGame->getWindow()->display();
}