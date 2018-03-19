#pragma once
#include "Category.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <set>
#include <vector>
#include <memory>
#include <utility>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable{
	public:
		explicit SceneNode(Category::Type category = Category::None);
		typedef std::unique_ptr<SceneNode> Ptr;
		typedef std::pair<SceneNode*, SceneNode*> Pair;
		void attachChild(Ptr child);
		Ptr detachChild(const SceneNode& node);
		void update(sf::Time dt, CommandQueue& commands);
		sf::Vector2f getWorldPosition() const;
		sf::Transform getWorldTransform() const;
		void onCommand(const Command& command, sf::Time dt);
		virtual unsigned  	getCategory() const;
		void removeWrecks();
		virtual bool isMarkedForRemoval() const;
		virtual bool isDestroyed() const;
		float length(sf::Vector2f vector);
		void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
		void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
		virtual sf::FloatRect getBoundingRect() const;


	private:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		void updateChildren(sf::Time dt, CommandQueue& commands);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
		std::vector<Ptr> mChildren;
		SceneNode* mParent;
		Category::Type mDefaultCategory;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);