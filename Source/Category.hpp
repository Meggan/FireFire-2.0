#pragma once


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerCharacter		= 1 << 1,
		EnemyCharacter		= 1 << 2,
	};
}

