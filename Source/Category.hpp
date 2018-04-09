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
		EnemyCharacterV = 1 << 8,
		EnemyBulletV = 1<<9,
		EnemyCharacterV2 = 1 << 10,
		EnemyBulletV2 = 1 << 11,

		Character = PlayerCharacter | EnemyCharacterD | EnemyCharacterZ | EnemyCharacterV | EnemyCharacterV2,
		EnemyCharacter = EnemyCharacterD | EnemyCharacterZ | EnemyCharacterV | EnemyCharacterV2,
		Weapon = PlayerBullet | EnemyBulletD | EnemyBulletZ | EnemyBulletV | EnemyBulletV2 | PlayerBomb,
		PlayerWeapon = PlayerBullet | PlayerBomb,
		EnemyWeapon = EnemyCharacterD | EnemyBulletZ | EnemyBulletV | EnemyBulletV2,
	};
}

