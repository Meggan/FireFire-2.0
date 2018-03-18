#include "DataTables.h"
#include "Source/Character.hpp"
//#include "Source/Projectile.h"

// For std::bind() placeholders
using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData() {
	//typecount is is the last in the enum..
	std::vector<CharacterData> data(Character::TypeCount);

	data[Character::Player].hp = 100;
	data[Character::Player].speed = 200.f;
	data[Character::Player].texture = Textures::Player;

	data[Character::Enemy].hp = 20;
	data[Character::Enemy].speed = 100.f;
	data[Character::Enemy].texture = Textures::Enemy;
	//Enemy movement
	data[Character::Enemy].direction.push_back(Direction(45, 60));
	data[Character::Enemy].direction.push_back(Direction(0.f, 25.f));
	data[Character::Enemy].direction.push_back(Direction(-45, 130));
	data[Character::Enemy].direction.push_back(Direction(0.f, 25.f));
	data[Character::Enemy].direction.push_back(Direction(45, 60));
	return data;
}
