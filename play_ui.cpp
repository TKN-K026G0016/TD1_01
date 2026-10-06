#include "play_ui.h"
#include "player.h"
#include "boss_enemy.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#pragma region データ: HPアイコン
struct LifeIcon {
	Vector2 startPos = { 30, 690 };
	Vector2 size = { 40, 40 };

	float intervalX = 10;

	Texture texture = {};
};
LifeIcon lifeIcon;

#pragma endregion

#pragma region データ: エネルギーゲージ

struct EnergyGaugeFrame {
	Vector2 pos = { 140, 630 };
	Vector2 size = { 256, 64 };

	Texture texture = {};
};
EnergyGaugeFrame energyGaugeFrame;

struct EnergyGaugeMeter {
	float gaugeRate = 0.0f;

	Texture texture = {};
};
EnergyGaugeMeter energyGaugeMeter;

#pragma endregion

#pragma region データ: ボスHPゲージ

struct BossHpGaugeFrame {
	Vector2 pos = { 640, 40 };
	Vector2 size = { 768, 32 };

	Texture texture = {};
};
BossHpGaugeFrame bossHpGaugeFrame;

struct BossHpGaugeMeter {
	float gaugeRate = 1.0f;

	Texture texture = {};
};
BossHpGaugeMeter bossHpGaugeMeter;

#pragma endregion

#pragma region 関数: ライフアイコン

static void InitLifeIcon(void) {
	lifeIcon.texture = GetTexture(TextureType::LifeIcon);
}

static void DrawLifeIcon(void) {
	int remainLife = GetPlayerRemainLife();

	for (int i = 0; i < remainLife; i++) {
		Vector2 posS = { lifeIcon.startPos.x  + (lifeIcon.size.x + lifeIcon.intervalX) * i, lifeIcon.startPos.y };

		DrawTextureUI(lifeIcon.texture, posS, lifeIcon.size);
	}
}

#pragma endregion

#pragma region 関数: エネルギーゲージ

static void InitEnergyGauge(void) {
	energyGaugeFrame.texture = GetTexture(TextureType::EnergyGaugeFrame);
	energyGaugeMeter.texture = GetTexture(TextureType::EnergyGaugeMeter);
}

static void UpdateEnergyGauge(void) {
	energyGaugeMeter.gaugeRate = GetPlayerRemainEnergy() / GetPlayerEnergyLimit();
}

static void DrawEnergyGauge(void) {
	//フレーム
	DrawTextureUI(energyGaugeFrame.texture, energyGaugeFrame.pos, energyGaugeFrame.size);
	//メーター
	DrawGaugeAsUI(energyGaugeMeter.texture, energyGaugeFrame.pos, energyGaugeFrame.size, energyGaugeMeter.gaugeRate);
}

#pragma endregion

#pragma region 関数: ボスHPゲージ

static void InitBossHpGauge(void) {
	bossHpGaugeFrame.texture = GetTexture(TextureType::BossHpGaugeFrame);
	bossHpGaugeMeter.texture = GetTexture(TextureType::BossHpGaugeMeter);
}

static void UpdateBossHpGauge(void) {
	bossHpGaugeMeter.gaugeRate = ToFloat(GetBossEnemyRemainLife()) / ToFloat(GetBossEnemyRemainLifeMax());
}

static void DrawBossHpGauge(void) {
	DrawTextureUI(bossHpGaugeFrame.texture, bossHpGaugeFrame.pos, bossHpGaugeFrame.size);
	DrawGaugeAsUI(bossHpGaugeMeter.texture, bossHpGaugeFrame.pos, bossHpGaugeFrame.size, bossHpGaugeMeter.gaugeRate);

#ifdef _DEBUG

	DrawTextureUI(lifeIcon.texture, GetBossHpGaugeEndPos(), lifeIcon.size);

#endif // _DEBUG


}

#pragma endregion

void InitPlayUI(void) {
	InitLifeIcon();
	InitEnergyGauge();
	InitBossHpGauge();
}

void UpdatePlayUI(void){
	UpdateEnergyGauge();
	UpdateBossHpGauge();
}

void DrawPlayUI(void) {
	DrawLifeIcon();
	DrawEnergyGauge();
	DrawBossHpGauge();
}

#pragma region 関数: 外部参照関係

Vector2 GetEnergyGaugeEndPos(void) {
	float posX = energyGaugeFrame.pos.x - energyGaugeFrame.size.x / 2 + energyGaugeFrame.size.x * energyGaugeMeter.gaugeRate;
	float posY = energyGaugeFrame.pos.y - energyGaugeFrame.size.y / 2;

	return { posX, posY };
}

Vector2 GetBossHpGaugeEndPos(void) {
	float posX = bossHpGaugeFrame.pos.x - bossHpGaugeFrame.size.x / 2 + bossHpGaugeFrame.size.x * bossHpGaugeMeter.gaugeRate;
	float posY = bossHpGaugeFrame.pos.y - bossHpGaugeFrame.size.y / 2;

	return { posX, posY };
}

#pragma endregion