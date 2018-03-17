#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>


class Character : public Entity
{
	public:
		enum Type
		{
			PlayerUp,
			Player,
			PlayerLeft,
			PlayerRight,
			Enemy,
		};


	public:
								Character(Type type, const TextureHolder& textures);
		virtual unsigned int	getCategory() const;


	private:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type					mType;
		sf::Sprite				mSprite;
};

