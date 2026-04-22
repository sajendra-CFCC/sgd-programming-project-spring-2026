#include "bosses.h"

namespace Bosses {

	void InitAll(GameState& gs) {
						//boss type, health, maxhealth, alive
		gs.bosses[0] = { BOSS_CREATURE_A, 100, 100, true };
		gs.bosses[1] = { BOSS_CREATURE_B, 200, 200, true };
		gs.bosses[2] = { BOSS_CREATURE_C, 300, 300, true };
		
		gs.activeBoss = 0; //first boss active
	}

	BossState& Bosses::ActiveBoss(GameState& gs) {
		return gs.bosses[gs.activeBoss];
	}

	const BossState& Bosses::ActiveBoss(const GameState& gs) {
		return gs.bosses[gs.activeBoss];
	}

	bool Bosses::AdvanceToNext(GameState& gs) {
		gs.bosses[gs.activeBoss].alive = false;
		for (int i = gs.activeBoss + 1; i < BOSS_COUNT; i++) {
			if (gs.bosses[i].alive) {
				gs.activeBoss = i;
				return true;
			}
		}
		return false;  // all dead
	}

	void Bosses::Draw(const BossState& boss, Vector2 pos, float scale) {
		switch (boss.type) {
			case BOSS_CREATURE_A:
				DrawCircle(pos.x, pos.y, 30 * scale, RED);
				DrawCircle(pos.x - 10 * scale, pos.y - 8 * scale, 6 * scale, WHITE);
				DrawCircle(pos.x + 10 * scale, pos.y - 8 * scale, 6 * scale, WHITE);
				break;
			case BOSS_CREATURE_B:
				DrawRectangle(pos.x - 25 * scale, pos.y - 30 * scale, 50 * scale, 60 * scale, PURPLE);
				DrawCircle(pos.x - 10 * scale, pos.y - 15 * scale, 5 * scale, WHITE);
				DrawCircle(pos.x + 10 * scale, pos.y - 15 * scale, 5 * scale, WHITE);
				break;
			case BOSS_CREATURE_C:
				DrawPoly(pos, 6, 30 * scale, 0, ORANGE);
				DrawCircle(pos.x, pos.y - 8 * scale, 8 * scale, BLACK);
				break;
			default: 
				break;
		}
	}

	void Draw(const BossState& boss, float x, float y, float scale) {
		Draw(boss, {x,y}, scale);
	}

	void Bosses::DrawHealthBar(const BossState& boss, Vector2 pos, float width) {
		float hp = boss.health;
		float maxHp = boss.maxHealth;
		float fill = (hp / maxHp) * width;
		DrawRectangle(pos.x, pos.y, width, 14, DARKGRAY);
		DrawRectangle(pos.x, pos.y, fill, 14, RED);
		DrawRectangleLinesEx({ pos.x, pos.y, width, 14 }, 1, BLACK);
	}
	void DrawHealthBar(const BossState& boss, float x, float y, float width) {
		DrawHealthBar(boss, {x,y}, width);
	}

}