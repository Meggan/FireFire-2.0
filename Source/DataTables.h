#pragma once
#include "ResourceIdentifiers.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <functional>

class Character;

//struct for direction for enemy movement
struct Direction {
	Direction(float an, float dis)
		: angle(an)
		, distance(dis) {
	}
	float angle;
	float distance;
};

//struct for all character data
struct CharacterData{
	int hp;
	float speed;

	Textures::ID texture;
	sf::Time fireInterval;
	std::vector<Direction> direction;
};

//struct for all weapon data
struct WeaponData{
	int	dmg;
	float speed;

	Textures::ID texture;
};

std::vector<CharacterData>	initializeCharacterData();
std::vector<WeaponData>	initializeWeaponData();
