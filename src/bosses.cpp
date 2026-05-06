#include "bosses.h"
#include "math.h"

namespace Bosses {
	//boss drawing function declarations (defined later)
	void DrawBossA(Vector2 pos, float s);
	void DrawBossB(Vector2 pos, float s);
	void DrawBossC(Vector2 pos, float s);
	void DrawBossD(Vector2 pos, float s);
	void DrawBossE(Vector2 pos, float s);
	void DrawBossF(Vector2 pos, float s);
	void DrawBossG(Vector2 pos, float s);
	void DrawBossH(Vector2 pos, float s);



	void InitAll(GameState& gs) {
						//boss type, health, maxhealth, alive
		gs.bosses[0] = { BOSS_CREATURE_A, 100, 100, true };
		gs.bosses[1] = { BOSS_CREATURE_B, 100, 100, true };
		gs.bosses[2] = { BOSS_CREATURE_C, 100, 100, true };
		gs.bosses[3] = { BOSS_CREATURE_D, 100, 100, true };
		gs.bosses[4] = { BOSS_CREATURE_E, 100, 100, true };
		gs.bosses[5] = { BOSS_CREATURE_F, 100, 100, true };
		gs.bosses[6] = { BOSS_CREATURE_G, 100, 100, true };
		gs.bosses[7] = { BOSS_CREATURE_H, 100, 100, true };
		gs.activeBoss = 0; //first boss active
	}

	BossState& ActiveBoss(GameState& gs) {
		return gs.bosses[gs.activeBoss];
	}

	const BossState& ActiveBoss(const GameState& gs) {
		return gs.bosses[gs.activeBoss];
	}

	bool AdvanceToNext(GameState& gs) {
		gs.bosses[gs.activeBoss].alive = false;
		gs.numBossesKilled++;
		//looking for next living boss
		for (int i = gs.activeBoss + 1; i < BOSS_COUNT; i++) {
			if (gs.bosses[i].alive) {
				gs.activeBoss = i;
				return true;
			}
		}
		// all dead
		
		if (gs.endlessBosses) {
			Bosses::InitAll(gs);   // reset all bosses back to full hp
			gs.activeBoss = 0;
			return true;           // never signals "done"
		}
		
		return false;  // all dead and no endless bossess
	}

	void Draw(const BossState& boss, float x, float y, float scale) {
		Draw(boss, { x,y }, scale);
	}

	void Draw(const BossState& boss, Vector2 pos, float scale) {
		switch (boss.type) {
			case BOSS_CREATURE_A: DrawBossA(pos, scale); break;
			case BOSS_CREATURE_B: DrawBossB(pos, scale); break;
			case BOSS_CREATURE_C: DrawBossC(pos, scale); break;
			case BOSS_CREATURE_D: DrawBossD(pos, scale); break;
			case BOSS_CREATURE_E: DrawBossE(pos, scale); break;
			case BOSS_CREATURE_F: DrawBossF(pos, scale); break;
			case BOSS_CREATURE_G: DrawBossG(pos, scale); break;
			case BOSS_CREATURE_H: DrawBossH(pos, scale); break;
			default: 
				break;
		}
	}

	Rectangle GetHitbox(const BossState& boss, Vector2 pos, float scale) {
		switch (boss.type) {
			case BOSS_CREATURE_A:   // round body + wobbly bottom
				return { pos.x - 32 * scale, pos.y - 32 * scale, 64 * scale, 68 * scale };
			case BOSS_CREATURE_B:   // tall rect including horns
				return { pos.x - 32 * scale, pos.y - 60 * scale, 64 * scale, 90 * scale };
			case BOSS_CREATURE_C:   // hex + spikes — use spike tip radius
				return { pos.x - 48 * scale, pos.y - 48 * scale, 96 * scale, 96 * scale };
			case BOSS_CREATURE_D:   // sleek cat head + ears
				return { pos.x - 26 * scale, pos.y - 38 * scale, 52 * scale, 62 * scale };
			case BOSS_CREATURE_E:   // chonky cat — wide cheeks dominate
				return { pos.x - 40 * scale, pos.y - 46 * scale, 80 * scale, 80 * scale };
			case BOSS_CREATURE_F:   // feral cat — ellipse body
				return { pos.x - 30 * scale, pos.y - 36 * scale, 60 * scale, 64 * scale };
			case BOSS_CREATURE_G:   // skull head + jaw
				return { pos.x - 28 * scale, pos.y - 28 * scale, 56 * scale, 56 * scale };
			case BOSS_CREATURE_H:   // slime — blob footprint + stalks
				return { pos.x - 42 * scale, pos.y - 58 * scale, 84 * scale, 94 * scale };
			default:
				return { 0, 0, 0, 0 };
		}
	}

	Rectangle GetHitbox(const BossState& boss, float x, float y, float scale) 	{
		return GetHitbox(boss, { x,y }, scale);
	}

	float GetHitRadius(const BossState& boss, float scale) {
		switch (boss.type) {
			case BOSS_CREATURE_A: return 34 * scale;   // body + slight wobble
			case BOSS_CREATURE_B: return 42 * scale;   // half diagonal of tall rect
			case BOSS_CREATURE_C: return 48 * scale;   // full spike tip
			case BOSS_CREATURE_D: return 30 * scale;   // head ellipse major axis
			case BOSS_CREATURE_E: return 40 * scale;   // cheek to cheek
			case BOSS_CREATURE_F: return 30 * scale;   // ellipse semi-major
			case BOSS_CREATURE_G: return 30 * scale;   // skull circle radius + jaw
			case BOSS_CREATURE_H: return 44 * scale;   // widest blob point
			default:              return 30 * scale;
		}
	}

	void DrawHealthBar(const BossState& boss, Vector2 pos, float width) {
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


	//drawing related functions below
	
	//helper function declarations (definated later)
	void DrawEye(Vector2 center, float r, float pupilOffX, float pupilOffY);
	void DrawCatEar(Vector2 tip, float size, float angle, Color outer, Color inner);
	void DrawWhiskers(Vector2 center, float s, Color c);

	// boss draw functions
	// A: round eye-heavy — ghostly white/blue
	void DrawBossA(Vector2 pos, float s) {
		DrawCircleV(pos, 32 * s, { 200, 220, 255, 255 });
		for (int i = -3; i <= 3; i++)
			DrawCircle(pos.x + i * 10 * s, pos.y + 28 * s, 8 * s, { 200, 220, 255, 255 });
		DrawEye({ pos.x - 11 * s, pos.y - 4 * s }, 9 * s, -2 * s, 2 * s);
		DrawEye({ pos.x + 11 * s, pos.y - 4 * s }, 9 * s, 2 * s, 2 * s);
		DrawRectangle(pos.x - 8 * s, pos.y + 10 * s, 16 * s, 4 * s, BLACK);
	}

	// B: tall rect — dark charcoal with red eyes
	void DrawBossB(Vector2 pos, float s) {
		DrawRectangle(pos.x - 22 * s, pos.y - 38 * s, 44 * s, 76 * s, { 50, 50, 60, 255 });
		DrawTriangle({ pos.x - 22 * s, pos.y - 38 * s },
			{ pos.x - 32 * s, pos.y - 60 * s },
			{ pos.x - 10 * s, pos.y - 38 * s }, { 30, 30, 40, 255 });
		DrawTriangle({ pos.x + 22 * s, pos.y - 38 * s },
			{ pos.x + 32 * s, pos.y - 60 * s },
			{ pos.x + 10 * s, pos.y - 38 * s }, { 30, 30, 40, 255 });
		DrawEye({ pos.x - 10 * s, pos.y - 18 * s }, 7 * s, -1 * s, 2 * s);
		DrawEye({ pos.x + 10 * s, pos.y - 18 * s }, 7 * s, 1 * s, 2 * s);
		// red glow over pupils
		DrawCircleV({ pos.x - 10 * s, pos.y - 16 * s }, 3 * s, { 255, 60, 60, 200 });
		DrawCircleV({ pos.x + 10 * s, pos.y - 16 * s }, 3 * s, { 255, 60, 60, 200 });
		for (int i = 0; i < 4; i++)
			DrawRectangle(pos.x - 14 * s + i * 8 * s, pos.y + 4 * s, 4 * s, 8 * s, WHITE);
	}

	// C: hexagon spiky — deep orange/amber
	void DrawBossC(Vector2 pos, float s) {
		DrawPoly(pos, 6, 30 * s, 30.0f, { 220, 130, 30, 255 });
		for (int i = 0; i < 6; i++) {
			float a = DEG2RAD * (60 * i + 30);
			float a2 = DEG2RAD * (60 * i + 30 + 180);
			Vector2 tip = { pos.x + cosf(a) * 48 * s,      pos.y + sinf(a) * 48 * s };
			Vector2 bl = { pos.x + cosf(a2) * 8 * s,      pos.y + sinf(a2) * 8 * s };
			Vector2 br = { pos.x + cosf(a + 0.4f) * 26 * s, pos.y + sinf(a + 0.4f) * 26 * s };
			DrawTriangle(tip, bl, br, { 180, 90, 10, 255 });
		}
		DrawEye({ pos.x, pos.y - 4 * s }, 10 * s, 0, 3 * s);
	}

	// D: cat sleek — dark grey/blue, menacing
	void DrawBossD(Vector2 pos, float s) {
		DrawEllipse(pos.x, pos.y, 26 * s, 24 * s, { 70, 75, 90, 255 });
		DrawCatEar({ pos.x - 18 * s, pos.y - 28 * s }, 18 * s, -0.6f, { 70, 75, 90, 255 }, { 180, 100, 120, 255 });
		DrawCatEar({ pos.x + 18 * s, pos.y - 28 * s }, 18 * s, 3.8f, { 70, 75, 90, 255 }, { 180, 100, 120, 255 });
		DrawEye({ pos.x - 10 * s, pos.y - 4 * s }, 7 * s, -1 * s, 1 * s);
		DrawEye({ pos.x + 10 * s, pos.y - 4 * s }, 7 * s, 1 * s, 1 * s);
		// slit pupils
		DrawRectangle(pos.x - 12 * s, pos.y - 6 * s, 4 * s, 10 * s, BLACK);
		DrawRectangle(pos.x + 8 * s, pos.y - 6 * s, 4 * s, 10 * s, BLACK);
		DrawTriangle({ pos.x,      pos.y + 4 * s },
			{ pos.x - 4 * s, pos.y + 1 * s },
			{ pos.x + 4 * s, pos.y + 1 * s }, { 180, 100, 120, 255 });
		DrawLineEx({ pos.x, pos.y + 4 * s }, { pos.x - 5 * s, pos.y + 8 * s }, 1.5f, DARKGRAY);
		DrawLineEx({ pos.x, pos.y + 4 * s }, { pos.x + 5 * s, pos.y + 8 * s }, 1.5f, DARKGRAY);
		DrawWhiskers(pos, 26 * s, { 140, 140, 160, 255 });
	}

	// E: cat chonky — warm orange tabby
	void DrawBossE(Vector2 pos, float s) {
		DrawCircleV(pos, 34 * s, { 210, 140, 60, 255 });
		DrawCircle(pos.x - 26 * s, pos.y + 6 * s, 14 * s, { 225, 160, 80, 255 });
		DrawCircle(pos.x + 26 * s, pos.y + 6 * s, 14 * s, { 225, 160, 80, 255 });
		// tabby stripes
		for (int i = 0; i < 3; i++)
			DrawLineEx({ pos.x - 16 * s + i * 14 * s, pos.y - 34 * s },
				{ pos.x - 12 * s + i * 14 * s, pos.y - 10 * s }, 3 * s, { 170, 100, 30, 255 });
		DrawCatEar({ pos.x - 24 * s, pos.y - 32 * s }, 22 * s, -0.9f, { 210, 140, 60, 255 }, { 240, 180, 180, 255 });
		DrawCatEar({ pos.x + 24 * s, pos.y - 32 * s }, 22 * s, 3.7f, { 210, 140, 60, 255 }, { 240, 180, 180, 255 });
		DrawEye({ pos.x - 12 * s, pos.y - 6 * s }, 10 * s, -2 * s, 2 * s);
		DrawEye({ pos.x + 12 * s, pos.y - 6 * s }, 10 * s, 2 * s, 2 * s);
		DrawCircle(pos.x, pos.y + 6 * s, 4 * s, { 240, 180, 180, 255 });
		DrawWhiskers(pos, 34 * s, WHITE);
	}

	// F: cat feral — muted green/grey, scarred
	void DrawBossF(Vector2 pos, float s) {
		DrawEllipse(pos.x - 2 * s, pos.y, 28 * s, 25 * s, { 100, 115, 90, 255 });
		DrawCatEar({ pos.x - 20 * s, pos.y - 26 * s }, 16 * s, -0.5f, { 100, 115, 90, 255 }, { 140, 60, 70, 255 });
		DrawCatEar({ pos.x + 22 * s, pos.y - 24 * s }, 12 * s, 3.9f, { 100, 115, 90, 255 }, { 180, 130, 140, 255 });
		DrawEye({ pos.x - 11 * s, pos.y - 5 * s }, 8 * s, -2 * s, 1 * s);
		DrawCircleV({ pos.x + 10 * s, pos.y - 5 * s }, 5 * s, WHITE);
		DrawRectangle(pos.x + 7 * s, pos.y - 7 * s, 6 * s, 9 * s, BLACK);
		// scar
		DrawLineEx({ pos.x + 4 * s,  pos.y - 14 * s },
			{ pos.x + 14 * s, pos.y + 2 * s }, 2.0f, { 140, 60, 70, 255 });
		DrawTriangle({ pos.x - 8 * s,  pos.y + 8 * s },
			{ pos.x - 2 * s,  pos.y + 14 * s },
			{ pos.x - 14 * s, pos.y + 14 * s }, WHITE);
		DrawWhiskers({ pos.x - 2 * s, pos.y + 2 * s }, 28 * s, { 160, 170, 150, 255 });
	}

	// G: skull — bone white with green glow
	void DrawBossG(Vector2 pos, float s) {
		DrawCircleV(pos, 28 * s, { 235, 230, 215, 255 });
		DrawRectangle(pos.x - 22 * s, pos.y + 8 * s, 44 * s, 20 * s, { 235, 230, 215, 255 });
		DrawCircleV({ pos.x - 11 * s, pos.y - 6 * s }, 9 * s, BLACK);
		DrawCircleV({ pos.x + 11 * s, pos.y - 6 * s }, 9 * s, BLACK);
		DrawCircleV({ pos.x - 11 * s, pos.y - 6 * s }, 4 * s, { 0, 255, 100, 180 });
		DrawCircleV({ pos.x + 11 * s, pos.y - 6 * s }, 4 * s, { 0, 255, 100, 180 });
		for (int i = 0; i < 5; i++)
			DrawRectangle(pos.x - 18 * s + i * 9 * s, pos.y + 10 * s, 6 * s, 12 * s, { 235, 230, 215, 255 });
		DrawLineEx({ pos.x + 4 * s,  pos.y - 28 * s }, { pos.x + 10 * s, pos.y - 8 * s }, 1.5f, DARKGRAY);
		DrawLineEx({ pos.x - 8 * s,  pos.y - 28 * s }, { pos.x - 4 * s,  pos.y - 10 * s }, 1.5f, DARKGRAY);
	}

	// H: slime — toxic green/yellow
	void DrawBossH(Vector2 pos, float s) {
		DrawCircle(pos.x, pos.y, 30 * s, { 120, 200, 60, 255 });
		DrawCircle(pos.x - 20 * s, pos.y + 8 * s, 20 * s, { 120, 200, 60, 255 });
		DrawCircle(pos.x + 22 * s, pos.y + 6 * s, 18 * s, { 120, 200, 60, 255 });
		DrawCircle(pos.x - 8 * s, pos.y + 22 * s, 16 * s, { 120, 200, 60, 255 });
		DrawCircle(pos.x + 10 * s, pos.y + 20 * s, 14 * s, { 120, 200, 60, 255 });
		DrawCircleV({ pos.x - 8 * s, pos.y - 12 * s }, 7 * s, { 200, 255, 150, 100 });
		// eye stalks
		DrawLineEx({ pos.x - 10 * s, pos.y - 30 * s }, { pos.x - 14 * s, pos.y - 48 * s }, 3 * s, { 90, 160, 40, 255 });
		DrawLineEx({ pos.x + 10 * s, pos.y - 30 * s }, { pos.x + 16 * s, pos.y - 46 * s }, 3 * s, { 90, 160, 40, 255 });
		DrawEye({ pos.x - 14 * s, pos.y - 50 * s }, 7 * s, -1 * s, 2 * s);
		DrawEye({ pos.x + 16 * s, pos.y - 48 * s }, 7 * s, 2 * s, 2 * s);
	}

	void DrawEye(Vector2 center, float r, float pupilOffX, float pupilOffY) {
		DrawCircleV(center, r, WHITE);
		DrawCircleV({ center.x + pupilOffX, center.y + pupilOffY }, r * 0.45f, BLACK);
		DrawCircleV({ center.x + pupilOffX + r * 0.12f, center.y + pupilOffY - r * 0.12f }, r * 0.15f, WHITE);
	}

	void DrawCatEar(Vector2 tip, float size, float angle, Color outer, Color inner) {
		Vector2 base1 = { tip.x - cosf(angle) * size,      tip.y - sinf(angle) * size };
		Vector2 base2 = { tip.x - cosf(angle + 0.9f) * size, tip.y - sinf(angle + 0.9f) * size };
		DrawTriangle(tip, base1, base2, outer);
		Vector2 itip = { tip.x - cosf(angle + 0.45f) * size * 0.25f, tip.y - sinf(angle + 0.45f) * size * 0.25f };
		Vector2 ib1 = { base1.x + cosf(angle + 0.45f) * size * 0.2f, base1.y + sinf(angle + 0.45f) * size * 0.2f };
		Vector2 ib2 = { base2.x + cosf(angle + 0.45f) * size * 0.2f, base2.y + sinf(angle + 0.45f) * size * 0.2f };
		DrawTriangle(itip, ib1, ib2, inner);
	}

	void DrawWhiskers(Vector2 center, float s, Color c) {
		DrawLineEx({ center.x - s * 0.4f, center.y + s * 0.05f }, { center.x - s * 1.1f, center.y - s * 0.05f }, 1.5f, c);
		DrawLineEx({ center.x - s * 0.4f, center.y + s * 0.12f }, { center.x - s * 1.1f, center.y + s * 0.18f }, 1.5f, c);
		DrawLineEx({ center.x - s * 0.4f, center.y + s * 0.00f }, { center.x - s * 1.1f, center.y + s * 0.28f }, 1.5f, c);
		DrawLineEx({ center.x + s * 0.4f, center.y + s * 0.05f }, { center.x + s * 1.1f, center.y - s * 0.05f }, 1.5f, c);
		DrawLineEx({ center.x + s * 0.4f, center.y + s * 0.12f }, { center.x + s * 1.1f, center.y + s * 0.18f }, 1.5f, c);
		DrawLineEx({ center.x + s * 0.4f, center.y + s * 0.00f }, { center.x + s * 1.1f, center.y + s * 0.28f }, 1.5f, c);
	}

}