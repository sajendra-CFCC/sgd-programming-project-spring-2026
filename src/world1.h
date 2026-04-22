#pragma once
#include "shared.h"

namespace World1 {
	void Init();
	WorldUpdateResult Update(GameState& game);
	void Draw(const GameState& game);
}
