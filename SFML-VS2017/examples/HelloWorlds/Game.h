#pragma once
#include </Users/dodobear/SFML-VS2017/include/SFML/Graphics.hpp>


class Game
{
public:
	Game();
	void run();
	~Game();
private:
	void processEvents();
	void update();
	void render();
private:
	sf::RenderWindow mWindow;
	sf::CircleShape mPlayer;

};

