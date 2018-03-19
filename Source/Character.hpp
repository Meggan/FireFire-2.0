#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "Weapon.h"
#include "Command.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>


class Character : public Entity
{
	public:
		enum Type {
			Player,
			EnemyZ,
			EnemyD,
			TypeCount,

			Enemy = EnemyZ | EnemyD,
		};


	public:
		Character(Type type, const TextureHolder& textures);
		virtual unsigned int	getCategory() const;
		float getMaxSpeed() const;
		float toRadian(float degree);
		void shoot();
		bool isAllied() const;
		virtual sf::FloatRect getBoundingRect() const;


	private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		 void checkBulletLaunch(sf::Time dt, CommandQueue& commands);
		 void createBullets(SceneNode& node, const TextureHolder& textures) const;
		 void createProjectile(SceneNode& node, Weapon::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
		 //void shootBullet();

	private:
		Type mType;
		sf::Sprite mSprite;
		float mTravelledDistance;
		std::size_t mDirectionIndex;
		void updateMovementPattern(sf::Time dt);
		void updateCurrent(sf::Time dt, CommandQueue& commands);
		bool mIsFiring;
		sf::Time mFireCountdown;
		Command mFireCommand;
		int mFireRateLevel;
		//int mSpreadLevel;
};

