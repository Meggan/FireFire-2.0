#pragma once


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
}

namespace Textures{
	enum ID{
		PlayerUp,
		Player,
		PlayerLeft,
		PlayerRight,
		EnemyZ,
		EnemyD,
		Background,
		Ball,
		Bomb,
		BulletZ,
		BulletD,
		TextureCount

	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

