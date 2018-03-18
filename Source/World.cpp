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
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
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
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerCharacter->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

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


void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
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
void World::addEnemies()
{
	// Add enemies to the spawn point container
	addEnemy(Character::EnemyZ, 0.f, 300.f);
	addEnemy(Character::EnemyZ, +50.f, 500.f);
	addEnemy(Character::EnemyZ, +100.f, 800.f);
	addEnemy(Character::EnemyD, +80.f, 1000.f);
	addEnemy(Character::EnemyZ, -70.f, 1200.f);
	addEnemy(Character::EnemyZ, +60.f, 1200.f);
	addEnemy(Character::EnemyD, 70.f, 1400.f);
	addEnemy(Character::EnemyZ, 70.f, 1550.f);
	addEnemy(Character::EnemyD, +100.f, 1600.f);
	addEnemy(Character::EnemyD, -100.f, 1750.f);
	addEnemy(Character::EnemyZ, -90.f, 1700.f);
	addEnemy(Character::EnemyD, -45.f, 1800.f);
	addEnemy(Character::EnemyZ, +40.f, 1850.f);
	addEnemy(Character::EnemyZ, 0.f, 1850.f);
	std::cout << "All Enemies created\n";

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		std::cout << "All Enemies Sorted";
		return lhs.y < rhs.y;
	});
}

void World::addEnemy(Character::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	std::cout << "Spawning Enemy..";
	mEnemySpawnPoints.push_back(spawn);
}


void World::spawnEnemies()
{
	// Spawns enemy pokemon
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getEnemySpawnBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures));
		enemy->setPosition(spawn.x, spawn.y);

		mSceneLayers[Air]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
		std::cout << "new instance of enemy created\n";
	}
}
sf::FloatRect World::getEnemySpawnBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 3000.f;
	bounds.height += 3000.f;

	return bounds;
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

