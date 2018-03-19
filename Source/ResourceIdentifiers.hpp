#pragma once

namespace sf
{
	class Texture;
}

//all textures for scenenodes
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

template <typename Resource, typename Identifier>
class ResourceHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

