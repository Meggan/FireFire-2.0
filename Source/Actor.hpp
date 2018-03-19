#pragma once
#include "SceneNode.hpp"
#include <cassert>


//Main class for all Actors
class Actor : public SceneNode{
	public:
		explicit Actor(int hp);
		void setVelocity(sf::Vector2f vel);
		void setVelocity(float vx, float vy);
		void accelerate(sf::Vector2f vel);
		void accelerate(float vx, float vy);
		void dmg(int dmg);
		void destroy();
		int getHP() const;
		bool isDestroyed() const;

		sf::Vector2f getVelocity() const;

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	private:
		int mHP;

		sf::Vector2f mVelocity;
};


