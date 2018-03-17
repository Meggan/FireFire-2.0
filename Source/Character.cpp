#include "Character.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
		case Character::Player:
			return Textures::Player;

		case Character::PlayerUp:
			return Textures::PlayerUp;

		case Character::PlayerLeft:
			return Textures::PlayerLeft;

		case Character::PlayerRight:
			return Textures::PlayerRight;
	}
	return Textures::Player;
}

Character::Character(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
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
		case PlayerUp:
			return Category::PlayerCharacter;
		case PlayerLeft:
			return Category::PlayerCharacter;
		case PlayerRight:
			return Category::PlayerCharacter;

		default:
			return Category::EnemyCharacter;
	}
}
