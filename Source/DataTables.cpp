#include "DataTables.h"
#include "Character.hpp"
#include "Weapon.h"

// For std::bind() placeholders
using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData(){
	//typecount is is the last in the enum..
	std::vector<CharacterData> data(Character::TypeCount);

	data[Character::Player].hp = 100;
	data[Character::Player].speed = 300.f;
	data[Character::Player].fireInterval = sf::seconds(1);
	data[Character::Player].texture = Textures::Player;

	data[Character::EnemyZ].hp = 20;
	data[Character::EnemyZ].speed = 80.f;
	data[Character::EnemyZ].texture = Textures::EnemyZ;
	data[Character::EnemyZ].fireInterval = sf::Time::Zero;
	data[Character::EnemyZ].direction.push_back(Direction(+45.f, 80.f));
	data[Character::EnemyZ].direction.push_back(Direction(-45.f, 160.f));
	data[Character::EnemyZ].direction.push_back(Direction(+45.f, 80.f));


	data[Character::EnemyD].hp = 50;
	data[Character::EnemyD].speed = 50.f;
	data[Character::EnemyD].texture = Textures::EnemyD;
	data[Character::EnemyD].fireInterval = sf::Time::Zero;
	//Enemy movement for Dragonite
	data[Character::EnemyD].direction.push_back(Direction(45, 60));
	data[Character::EnemyD].direction.push_back(Direction(0.f, 25.f));
	data[Character::EnemyD].direction.push_back(Direction(-45, 130));
	data[Character::EnemyD].direction.push_back(Direction(0.f, 25.f));
	data[Character::EnemyD].direction.push_back(Direction(45, 60));
	return data;
}


std::vector<WeaponData> initializeWeaponData()
{
	std::vector<WeaponData> data(Weapon::TypeCount);

	data[Weapon::PlayerBullet].dmg = 10;
	data[Weapon::PlayerBullet].speed = 350.f;
	data[Weapon::PlayerBullet].texture = Textures::Ball;

	data[Weapon::PlayerBomb].dmg = 50;
	data[Weapon::PlayerBomb].speed = 500.f;
	data[Weapon::PlayerBomb].texture = Textures::Bomb;

	data[Weapon::EnemyBulletZ].dmg = 10;
	data[Weapon::EnemyBulletZ].speed = 200.f;
	data[Weapon::EnemyBulletZ].texture = Textures::BulletZ;

	data[Weapon::EnemyBulletD].dmg = 10;
	data[Weapon::EnemyBulletD].speed = 350.f;
	data[Weapon::EnemyBulletD].texture = Textures::BulletD;

	return data;
}

