#include "Character.hpp"
#include "ResourceHolder.hpp"
#include "../DataTables.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <cmath>

namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder& textures)
: Entity(Table[type].hp)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTravelledDistance(0.f)
, mDirectionIndex(0)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}


Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::Player:
		return Textures::Player;

		//case Character::PlayerUp:
		//	return Textures::PlayerUp;

		//case Character::PlayerLeft:
		//	return Textures::PlayerLeft;

		//case Character::PlayerRight:
		//	return Textures::PlayerRight;

	case Character::Enemy:
		return Textures::Enemy;
	}
	return Textures::Player;
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Character::getCategory() const
{
	switch (mType)
	{
		case Player:
			return Category::PlayerCharacter;
		//case PlayerUp:
		//	return Category::PlayerCharacter;
		//case PlayerLeft:
		//	return Category::PlayerCharacter;
		//case PlayerRight:
		//	return Category::PlayerCharacter;

		default:
			return Category::EnemyCharacter;
	}
}

//set up enemy movement patterns based on data table
void Character::updateMovementPattern(sf::Time dt)
{
	
	const std::vector<Direction>& directions = Table[mType].direction;
	if (!directions.empty())
	{
		// change directions when current movement is complete
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
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

float Character::toRadian(float degree)
{
	return 3.141592653589793238462643383f / 180.f * degree;
}

float Character::getMaxSpeed() const
{
	return Table[mType].speed;
}