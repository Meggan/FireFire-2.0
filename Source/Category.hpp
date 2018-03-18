#pragma once


// Entity/scene node category, used to dispatch commands
namespace Category{
	enum Type	{
		None = 0,
		Scene = 1 << 0,
		PlayerCharacter = 1 << 1,
		EnemyCharacterZ = 1 << 2,
		EnemyCharacterD = 1 << 3,
		PlayerBullet = 1 << 4,
		EnemyBulletZ = 1 << 5,
		EnemyBulletD = 1 << 6,
		PlayerBomb = 1 <<7,

		Character = PlayerCharacter | EnemyCharacterD | EnemyBulletZ,
		EnemyCharacter = EnemyCharacterD | EnemyBulletZ,
		Weapon = PlayerBullet | EnemyBulletD | EnemyBulletZ | PlayerBomb,
	};
}

