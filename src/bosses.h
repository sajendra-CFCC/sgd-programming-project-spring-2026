#pragma once
#include "shared.h"

namespace Bosses {
	
	// called once at game start to setup all bosses
	void InitAll(GameState& gs);

	// drawing — each boss has its own primitive look
	void Draw(const BossState& boss, Vector2 pos, float scale);
	void DrawHealthBar(const BossState& boss, Vector2 pos, float width);
	//overloads for convenience
	void Draw(const BossState& boss, float x, float y, float scale);
	void DrawHealthBar(const BossState& boss, float x, float y, float width);

	// call when boss hp hits zero — advances to next living boss
	// returns true if there are more bosses, false if all dead (win condition)
	bool AdvanceToNext(GameState& gs);

	// convenience
	BossState& ActiveBoss(GameState& gs);
	const BossState& ActiveBoss(const GameState& gs);
}