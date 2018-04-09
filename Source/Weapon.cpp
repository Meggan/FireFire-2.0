#include "Weapon.h"
#include "DataTables.h"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>
#include <cassert>

namespace{
	const std::vector<WeaponData> Table = initializeWeaponData();
}


Weapon::Weapon(Type type, const TextureHolder& textures): Actor(1)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mTargetDirection(){
	centerOrigin(mSprite);
}

void Weapon::updateCurrent(sf::Time dt, CommandQueue& commands){
	Actor::updateCurrent(dt, commands);
}

void Weapon::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

unsigned int Weapon::getCategory() const{
	if (mType == EnemyBulletZ)
		return Category::EnemyBulletZ;
	else if (mType == EnemyBulletD)
		return Category::EnemyBulletD;
	else if (mType == EnemyBulletV)
		return Category::EnemyBulletV;
	else if (mType == EnemyBulletV2)
		return Category::EnemyBulletV2;
	else if (mType == PlayerBomb)
		return Category::PlayerBomb;
	else
		return Category::PlayerBullet;
}

//bounding rect for collision
sf::FloatRect Weapon::getBoundingRect() const{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Weapon::getMaxSpeed() const{
	return Table[mType].speed;
}

int Weapon::getDamage() const{
	return Table[mType].dmg;
}

void Weapon::centerOrigin(sf::Sprite& sprite){
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}