#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.h"
#include "CommandQueue.hpp"
#include "Weapon.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>
#include <iostream>

namespace{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder& textures) : Entity(Table[type].hp)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mFireCountdown(sf::Time::Zero)
, mIsFiring(false)
, mFireRateLevel(1)
, mFireCommand()
, mSpreadLevel(1)

{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands){
	
	// Check if bullets or missiles are fired
	checkBulletLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);
}

Textures::ID toTextureID(Character::Type type){
	switch (type)	{
	case Character::Player:	
		return Textures::Player;

	case Character::EnemyZ:
		return Textures::EnemyZ;

	case Character::EnemyD:
		return Textures::EnemyD;
	}
	return Textures::Player;
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(mSprite, states);
}

unsigned int Character::getCategory() const{
		if (isAllied())
			return Category::PlayerCharacter;
		else
			return Category::EnemyCharacterZ;
	}


//set up enemy movement patterns based on data table
void Character::updateMovementPattern(sf::Time dt){
	
	const std::vector<Direction>& directions = Table[mType].direction;
	if (!directions.empty())	{
		// change directions when current movement is complete
		if (mTravelledDistance > directions[mDirectionIndex].distance){
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

float Character::toRadian(float degree){
	return 3.141592653589793238462643383f / 180.f * degree;
}

float Character::getMaxSpeed() const{
	return Table[mType].speed;
}

void Character::shoot()
{
	// Only characters with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero) 
		mIsFiring = true;
}

bool Character::isAllied() const
{
	return mType == Player;
}

void Character::checkBulletLaunch(sf::Time dt, CommandQueue& commands)
{
	if (!isAllied())
		shoot();

	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Weapon::Type type = isAllied() ? Weapon::PlayerBullet : Weapon::EnemyBulletZ;
		createProjectile(node, type, 0.0f, 0.5f, textures);

}
void Character::createProjectile(SceneNode& node, Weapon::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Weapon> projectile(new Weapon(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}