#include "World.hpp"
#include "Foreach.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 20000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerCharacter(nullptr)
, mEnemySpawnPoints()
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerCharacter->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	//handle collisions
	handleCollisions();

	// Remove all destroyed entities, create new ones
	mSceneGraph.removeWrecks();
	spawnEnemies();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Player, "Media/Textures/CharacterDown.png");
	mTextures.load(Textures::PlayerUp, "Media/Textures/CharacterUp.png");
	mTextures.load(Textures::PlayerLeft, "Media/Textures/CharacterLeft.png");
	mTextures.load(Textures::PlayerRight, "Media/Textures/CharacterRight.png");
	mTextures.load(Textures::Background, "Media/Textures/Background.png");
	mTextures.load(Textures::Ball, "Media/Textures/Pokeball.png");
	mTextures.load(Textures::EnemyZ, "Media/Textures/Zubat.png");
	mTextures.load(Textures::EnemyD, "Media/Textures/Dragonite.png");
	mTextures.load(Textures::Bomb, "Media/Textures/Electrode.png");
	mTextures.load(Textures::BulletZ, "Media/Textures/BulletZ.png");
	mTextures.load(Textures::BulletD, "Media/Textures/BulletD.png");
}



void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 20.f;

	sf::Vector2f position = mPlayerCharacter->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerCharacter->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerCharacter->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	//mPlayerCharacter->accelerate(0.f, mScrollSpeed);
}


void World::buildScene(){
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)	{
		Category::Type category = (i == Air) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Background);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's character
	std::unique_ptr<Character> player(new Character(Character::Player, mTextures));
	mPlayerCharacter = player.get();
	mPlayerCharacter->setPosition(mSpawnPosition);
	mSceneLayers[Air]->attachChild(std::move(player));

	//add enemies to the scene
	addEnemies();
}

//add multiple enemies to the world
void World::addEnemies(){
	// Add enemies to the spawn point container
	for (int i = 1; i < 10; ++i) {
		addEnemy(Character::EnemyZ, (randNum(-150, +150)), (randNum(i * 500, i * 1000)));
		addEnemy(Character::EnemyD, (randNum(-150, +150)), (randNum(i * 500, i * 1000)));
		addEnemy(Character::EnemyZ, (randNum(-150, +150)), (randNum(i * 500, i * 1000)));
	}
	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs) {
		//std::cout << "All Enemies Sorted";
		return lhs.y < rhs.y;
	});
}

float World::randNum(float min, float max){
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

void World::addEnemy(Character::Type type, float relX, float relY){
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	//std::cout << "Spawning Enemy..";
	mEnemySpawnPoints.push_back(spawn);
}


void World::spawnEnemies(){
	// Spawns enemy pokemon
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getEnemySpawnBounds().top)	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures));
		enemy->setPosition(spawn.x, spawn.y);

		mSceneLayers[Air]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
		std::cout << "new instance of enemy created\n";
	}
}

sf::FloatRect World::getEnemySpawnBounds() const{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100;
	bounds.height += 100;

	return bounds;
}

sf::FloatRect World::getViewBounds() const{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs){
		if (matchesCategories(pair, Category::PlayerCharacter, Category::EnemyCharacter))		{
			//set first to play, second to enemy
			auto& player = static_cast<Character&>(*pair.first);
			auto& enemy = static_cast<Character&>(*pair.second);
			//kill enemy
			player.dmg(enemy.getHP());
			enemy.destroy();
			std::cout << "You bumped into someone!" << std::endl << "Current HP: " << player.getHP() << std::endl;
		}

		//else if someone gets hit by a projectile
		else if (matchesCategories(pair, Category::EnemyCharacter, Category::PlayerWeapon)
			|| matchesCategories(pair, Category::PlayerCharacter, Category::EnemyWeapon))		{
			//set types
			auto& enemy = static_cast<Character&>(*pair.first);
			auto& projectile = static_cast<Weapon&>(*pair.second);

			// Apply projectile damage to character, destroy projectile
			std::cout << "Projectile hit someone!"<< std::endl << "Enemy HP: " << enemy.getHP() << std::endl;;
			enemy.dmg(projectile.getDamage());
			projectile.destroy();
		}
	}
}

void World::destroyActorsOutsideView()
{
	Command command;
	command.category = Category::Weapon | Category::EnemyCharacter;
	command.action = derivedAction<Actor>([this] (Actor& a, sf::Time)
	{
		if (!getEnemySpawnBounds().intersects(a.getBoundingRect()))
			a.destroy();
	});

	mCommandQueue.push(command);
}