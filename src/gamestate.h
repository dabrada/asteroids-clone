#pragma once
#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:
    GameState(Game * pGame);
    virtual ~GameState() {}
    // Sva moguća stanja igre.
    enum State {
        Welcome,
        Playing,
        Score,
        SIZE
    };
    // Metode koje moraju osigurati sva stanja igre.
    virtual void init() = 0;
    virtual void handleEvents(sf::Event event) = 0;
    virtual void update(sf::Time dt = sf::Time::Zero) = 0;
    virtual void render() = 0;

	friend class Game;
protected:
    Game * const mpGame; // Pokazivač na Game klasu
	// eventualno i druge varijable  
};


class WelcomeState : public GameState
{
public:
	WelcomeState(Game * pGame);
	virtual ~WelcomeState() {}
	void init() override;
	void handleEvents(sf::Event event) override;
	void update(sf::Time dt = sf::Time::Zero) override;
	void render() override;

	friend class Game;

	sf::Font font;
	sf::Text strTitle;
	sf::Text strSubtitle;
};

class ScoreState : public GameState
{
public:
	ScoreState(Game * pGame);
	virtual ~ScoreState() {}
	void init() override;
	void handleEvents(sf::Event event) override;
	void update(sf::Time dt = sf::Time::Zero) override;
	void render() override;
	friend class Game;

protected:
	sf::Text finalScoreText;
	sf::Text continueText;
	int score;
};


