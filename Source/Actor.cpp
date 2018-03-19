#include "Actor.hpp"

Actor::Actor(int hp) : mVelocity()
	, mHP(hp){
}

void Actor::setVelocity(sf::Vector2f velocity){
	mVelocity = velocity;
}

void Actor::setVelocity(float vx, float vy){
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Actor::getVelocity() const{
	return mVelocity;
}

void Actor::accelerate(sf::Vector2f velocity){
	mVelocity += velocity;
}

void Actor::accelerate(float vx, float vy){
	mVelocity.x += vx;
	mVelocity.y += vy;
}

void Actor::updateCurrent(sf::Time dt, CommandQueue&) {
	move(mVelocity * dt.asSeconds());
}

bool Actor::isDestroyed() const
{
	return mHP <= 0;
}

int Actor::getHP() const
{
	return mHP;
}

void Actor::dmg(int points)
{
	assert(points > 0);

	mHP -= points;
}

void Actor::destroy()
{
	mHP = 0;
}