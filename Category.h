#pragma once

namespace Category {
	enum Type {
		None = 0,
		Background = 1 << 0,
		Player = 1 << 1,
		NPC = 1 << 2,
		Collidable = 1 << 3,
		JumpDown = 1 << 4,
		JumpLeft = 1 << 5,
		JumpRight = 1 << 6,
		Entrance = 1 << 7,
		World = 1 << 8,
		Interactable = 1 << 10,
	};

}