#pragma once

#include "SceneNode.hpp"
#include <cassert>


//Main class for all Actors/Entities
class Entity : public SceneNode{
	public:
		void setVelocity(sf::Vector2f vel);
		void setVelocity(float vx, float vy);
		void accelerate(sf::Vector2f vel);
		void accelerate(float vx, float vy);
		sf::Vector2f getVelocity() const;

	//health for Entities
	public:
		explicit Entity(int hp);
		//void heal(int hp);
		//void damage(int dmg);
		//void destroy();
		//int getHP() const;
		bool isDestroyed() const;

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	private:
		int mHP;
		sf::Vector2f mVelocity;
};


