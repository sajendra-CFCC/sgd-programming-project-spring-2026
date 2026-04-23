#pragma once
#include "shared.h"

namespace Bosses {
	
	// called once at game start to setup all bosses
	void InitAll(GameState& gs);

	// drawing — each boss has its own primitive look
	void Draw(const BossState& boss, Vector2 pos, float scale);
	void Draw(const BossState& boss, float x, float y, float scale); //overload for convenience
	
	//draw health bar for given boss
	void DrawHealthBar(const BossState& boss, Vector2 pos, float width);
	void DrawHealthBar(const BossState& boss, float x, float y, float width); //overload for convenience

	// returns the boss's hitbox at a given position and scale, matches Draw()
	Rectangle GetHitbox(const BossState& boss, Vector2 pos, float scale);
	Rectangle GetHitbox(const BossState& boss, float x, float y, float scale);

	// convenience for circle type hitbox (for round bosses?), 
	// returns radius so caller can use CheckCollisionCircles etc
	float GetHitRadius(const BossState& boss, float scale);
	
	// call when boss hp hits zero — advances to next living boss
	// returns true if there are more bosses, false if all dead (win condition)
	bool AdvanceToNext(GameState& gs);

	// convenience
	BossState& ActiveBoss(GameState& gs);
	const BossState& ActiveBoss(const GameState& gs);
}