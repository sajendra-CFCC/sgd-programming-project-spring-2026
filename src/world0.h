#pragma once
#include "shared.h"

namespace World0 {
	void Init();
	WorldUpdateResult Update(GameState& game);
	void Draw();
}