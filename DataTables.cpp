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

	data[Character::EnemyZ].hp = 20;
	data[Character::EnemyZ].speed = 80.f;
	data[Character::EnemyZ].texture = Textures::EnemyZ;
	data[Character::EnemyZ].direction.push_back(Direction(+45.f, 80.f));
	data[Character::EnemyZ].direction.push_back(Direction(-45.f, 160.f));
	data[Character::EnemyZ].direction.push_back(Direction(+45.f, 80.f));


	data[Character::EnemyD].hp = 40;
	data[Character::EnemyD].speed = 90.f;
	data[Character::EnemyD].texture = Textures::EnemyD;
	//Enemy movement for Dragonite
	data[Character::EnemyD].direction.push_back(Direction(45, 60));
	data[Character::EnemyD].direction.push_back(Direction(0.f, 25.f));
	data[Character::EnemyD].direction.push_back(Direction(-45, 130));
	data[Character::EnemyD].direction.push_back(Direction(0.f, 25.f));
	data[Character::EnemyD].direction.push_back(Direction(45, 60));
	return data;
}
