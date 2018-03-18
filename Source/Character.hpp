#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>


class Character : public Entity
{
	public:
		enum Type
		{
			//PlayerUp,
			Player,
			//PlayerLeft,
			//PlayerRight,
			EnemyZ,
			EnemyD,
			TypeCount,
		};


	public:
								Character(Type type, const TextureHolder& textures);
		virtual unsigned int	getCategory() const;
		float					getMaxSpeed() const;
		float					toRadian(float degree);


	private:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type					mType;
		sf::Sprite				mSprite;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		void					updateMovementPattern(sf::Time dt);
		void		updateCurrent(sf::Time dt, CommandQueue& commands);
};

