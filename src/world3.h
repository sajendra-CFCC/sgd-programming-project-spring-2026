#pragma once
#include "shared.h"


namespace World0 {
	void Init(); //set up anything you need for your game / world here
	WorldUpdateResult Update(GameState& game); //update your world every frame
	void Draw(const GameState& game); //draw your world every frame
}