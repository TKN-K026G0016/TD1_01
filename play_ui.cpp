#include "play_ui.h"
#include "player.h"
#include "elec_bullet.h"
#include "boss_enemy.h"
#include "stage.h"
#include "effect.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <Novice.h>

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
	Vector2 pos = { 640, 80 };
	Vector2 size = { 512, 100 };

	//デンリョク不足警告表示のタイミング
	float warnDeadEnergy = 30.0f;
	float dangerEnergy = 15.0f;

	////過ジュウデン警告表示のタイミング
	//float warnOverHeat = 75.0f;

	//ケムリの生成範囲(frameのposを中心に)
	Vector2i spawnSmokeRange = { 250, 60 };
	//警告中の煙エフェクト生成スパン
	Timer createSmokeWarningTimer = { 20, 0 };
	//オーバーヒート中の煙エフェクト生成スパン
	Timer createSmokeOverHeatTimer = { 7, 0 };

	//ゲージエフェクトの生成スパン
	Timer createGaugeEffectTimer = { 3, 0 };

	Texture texture = {};
};
EnergyGaugeFrame energyGaugeFrame;

struct EnergyGaugeMeter {
	float gaugeRate = 0.0f;

	Texture texture = {};
};
EnergyGaugeMeter energyGaugeMeter;

struct EnergyGaugeDelayMeter {
	float gaugeRate = 0.0f;

	//追従速度
	float followSpeed = 0.007f;

	//イージングを使うかどうかの判定値
	float threshold = 0.01f;

	//イージング用
	float easingT = 0.0f;
	float startRate = 0.0f;
	float endRate = 0.0f;

	Texture texture = {};
};
EnergyGaugeDelayMeter energyGaugeDelayMeter;

struct MiniEnergyGauge {
	Vector2 pos = {};
	Vector2 size = { 64, 16 };
	Vector2 offset = { 0, 40 };

	Texture frameTexture = {};
	Texture meterTexture = {};
	Texture delayMeterTexture = {};
};
MiniEnergyGauge miniEnergyGauge;

#pragma endregion

#pragma region データ: ボスHPゲージ

struct BossHpGaugeFrame {
	Vector2 pos = { 640, 680 };
	Vector2 size = { 768, 32 };

	Texture texture = {};
};
BossHpGaugeFrame bossHpGaugeFrame;

struct BossHpGaugeMeter {
	float gaugeRate = 1.0f;

	Texture texture = {};
};
BossHpGaugeMeter bossHpGaugeMeter;

struct BossHpGaugeDelayMeter {
	float gaugeRate = 1.0f;

	//追従速度
	float followSpeed = 0.007f;

	//イージングを使うかどうかの判定値
	float threshold = 0.01f;

	//イージング用
	float easingT = 0.0f;
	float startRate = 0.0f;
	float endRate = 0.0f;

	Texture texture = {};
};
BossHpGaugeDelayMeter bossHpGaugeDelayMeter;

#pragma endregion

#pragma region データ: 持続ボーナス

struct LaserPowRate {
	Vector2i startPos = { 1000, 700 };

	float powRate = 1.0f;
};
LaserPowRate laserPowRate;



#pragma endregion

#pragma region データ: 進捗度ゲージ

static constexpr bool kShowProgressGaugeSwitch = true;
struct ProgressGauge {
	Vector2 pos = { 640, 680 };
	Vector2 size = { 768, 32 };

	//ボスラウンドまでの時間
	int bossRoundTime = 0;
	float gaugeRate = 0;

	Texture frameTexture = {};
	Texture meterTexture = {};
};
ProgressGauge progressGauge;

struct ProgressGaugeIcon {
	Vector2 pos = {0.0f, progressGauge.pos.y };
	Vector2 size = { 32, 32 };

	Vector2 startPos = { progressGauge.pos.x - progressGauge.size.x / 2, progressGauge.pos.y };
	Vector2 endPos = { progressGauge.pos.x + progressGauge.size.x / 2, progressGauge.pos.y };

	Texture texture = {};
};
ProgressGaugeIcon progressGaugeIcon;

#pragma endregion

#pragma region データ: ショックコンボ

constexpr int kComboNumbersLimit = 3;
struct ShockComboNumbers {
	Vector2 size = { 68, 128 };

	Vector2 startPos = { 1200, 630 };
	Vector2 interval = { -80.0f, 0.0f };

	int animNum = 0;
};
ShockComboNumbers comboNumbers[kComboNumbersLimit];



struct ShockCombo {
	//コンボ数
	int combo = 0;

	//前のコンボを見せている状態
	bool isShowPreCombo = false;
	Timer showPreComboTimer = { 60, 0 };

	//透明の時間
	bool isInvisible = false;
	Timer invisibleTimer = { 7, 0 };
};
ShockCombo shockCombo;


Texture comboNumbersTexture = {};


#pragma endregion

#pragma region 関数: ライフアイコン

static void InitLifeIcon(void) {
	lifeIcon.texture = GetTexture(TextureType::LifeIcon);
}

static void DrawLifeIcon(void) {
	if (GetLifeGaugeSwitch()) return;

	int remainLife = GetPlayerHp();

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
	energyGaugeDelayMeter.texture = GetTexture(TextureType::EnergyGaugeDelayMeter);

	miniEnergyGauge.frameTexture = GetTexture(TextureType::MiniEnergyGaugeFrame);
	miniEnergyGauge.meterTexture = GetTexture(TextureType::MiniEnergyGaugeMeter);
	miniEnergyGauge.delayMeterTexture = GetTexture(TextureType::MiniEnergyGaugeDelayMeter);
}

static void UpdateEnergyGauge(void) {
	float remainEnergy = GetPlayerRemainEnergy();
	float energyLimit = GetPlayerEnergyLimit();

#pragma region 残量の変化処理

	energyGaugeMeter.gaugeRate = remainEnergy / energyLimit;

#pragma endregion

#pragma region DelayMeterの変化処理

	float nowRate = energyGaugeMeter.gaugeRate;
	float diff = energyGaugeDelayMeter.gaugeRate - nowRate;

	//delayゲージが本ゲージより大きい(減少した際)
	if (energyGaugeDelayMeter.gaugeRate > nowRate) {

		//エージングでの更新
		if (diff > energyGaugeDelayMeter.threshold) {
			//始点と終点を更新
			energyGaugeDelayMeter.startRate = energyGaugeDelayMeter.gaugeRate;
			energyGaugeDelayMeter.endRate = nowRate;

			//easingTを更新
			energyGaugeDelayMeter.easingT += energyGaugeDelayMeter.followSpeed;
			if (energyGaugeDelayMeter.easingT > 1.0f) {
				energyGaugeDelayMeter.easingT = 1.0f;
			}

			//更新
			energyGaugeDelayMeter.gaugeRate =
				UpdateValueByEasing(
					energyGaugeDelayMeter.startRate,
					energyGaugeDelayMeter.endRate,
					energyGaugeDelayMeter.easingT,
					EasingMode::EaseIn
				);
		}
		else {
			energyGaugeDelayMeter.gaugeRate = nowRate;
			energyGaugeDelayMeter.easingT = 0.0f;
		}

	} else {
		energyGaugeDelayMeter.gaugeRate = nowRate;
		energyGaugeDelayMeter.easingT = 0.0f;
	}

#pragma endregion

#pragma region GaugeEffectの生成

	//energyGaugeFrame.createGaugeEffectTimer.count++;
	//if (energyGaugeFrame.createGaugeEffectTimer.count >= energyGaugeFrame.createGaugeEffectTimer.time) {
	//	energyGaugeFrame.createGaugeEffectTimer.count = 0;

	//	CreateGageEffect(GetEnergyGaugeEndPos());
	//}

#pragma endregion

#pragma region 点滅処理

	//警告表示のラインを下回るor故障状態時
	if (remainEnergy <= energyGaugeFrame.warnDeadEnergy /*|| GetPlayerNowState() != PlayerState::Normal*/) {
		UpdateAnimation(energyGaugeFrame.texture);
		UpdateAnimation(miniEnergyGauge.frameTexture);
	}
	else {
		energyGaugeFrame.texture.animNum = 0;
		energyGaugeFrame.texture.animChangeTimer.count = 0;

		miniEnergyGauge.frameTexture.animNum = 0;
		miniEnergyGauge.frameTexture.animChangeTimer.count = 0;
	}

#pragma endregion

#pragma region ケムリ生成処理

	if (remainEnergy <= energyGaugeFrame.warnDeadEnergy) {
		energyGaugeFrame.createSmokeWarningTimer.count++;
	} else if (remainEnergy <= energyGaugeFrame.dangerEnergy) {
		energyGaugeFrame.createSmokeOverHeatTimer.count++;
	}

	if (energyGaugeFrame.createSmokeWarningTimer.count >= energyGaugeFrame.createSmokeWarningTimer.time
		|| energyGaugeFrame.createSmokeOverHeatTimer.count >= energyGaugeFrame.createSmokeOverHeatTimer.time) {
		energyGaugeFrame.createSmokeWarningTimer.count = 0;
		energyGaugeFrame.createSmokeOverHeatTimer.count = 0;

		//生成座標を設定
		float posX = energyGaugeFrame.pos.x + ToFloat(GetRand(-energyGaugeFrame.spawnSmokeRange.x, energyGaugeFrame.spawnSmokeRange.x));
		float posY = energyGaugeFrame.pos.y + ToFloat(GetRand(0, energyGaugeFrame.spawnSmokeRange.y));

		CreateSmokeEffect({ posX, posY }, TypeDrawEffect::UI);
	}

#pragma endregion

#pragma region ミニゲージの座標更新
	//miniGaugeの座標を更新
	Vector2 playerPos = GetPlayerPos();
	miniEnergyGauge.pos = { playerPos.x + miniEnergyGauge.offset.x, playerPos.y + miniEnergyGauge.offset.y };

#pragma endregion

}

static void DrawEnergyGauge(void) {
	//通常ゲージ
	//フレーム
	DrawTextureUI(energyGaugeFrame.texture, energyGaugeFrame.pos, energyGaugeFrame.size);
	//delayメーター
	DrawGaugeAsUI(energyGaugeDelayMeter.texture, energyGaugeFrame.pos, energyGaugeFrame.size, energyGaugeDelayMeter.gaugeRate);
	//メーター
	DrawGaugeAsUI(energyGaugeMeter.texture, energyGaugeFrame.pos, energyGaugeFrame.size, energyGaugeMeter.gaugeRate);

	//ミニゲージ
	//フレーム
	DrawTextureObj(miniEnergyGauge.frameTexture, miniEnergyGauge.pos, miniEnergyGauge.size);
	//delayメーター
	DrawGaugeAsObj(miniEnergyGauge.delayMeterTexture, miniEnergyGauge.pos, miniEnergyGauge.size, energyGaugeDelayMeter.gaugeRate);
	//メーター
	DrawGaugeAsObj(miniEnergyGauge.meterTexture, miniEnergyGauge.pos, miniEnergyGauge.size, energyGaugeMeter.gaugeRate);
}

#pragma endregion

#pragma region 関数: ボスHPゲージ

static void InitBossHpGauge(void) {
	bossHpGaugeFrame.texture = GetTexture(TextureType::BossHpGaugeFrame);
	bossHpGaugeMeter.texture = GetTexture(TextureType::BossHpGaugeMeter);
	bossHpGaugeDelayMeter.texture = GetTexture(TextureType::BossHpGaugeMeter);
}

static void UpdateBossHpGauge(void) {
	if (!GetBossEnemyIsAlive()) return;

	bossHpGaugeMeter.gaugeRate = ToFloat(GetBossEnemyRemainLife()) / ToFloat(GetBossEnemyRemainLifeMax());

#pragma region delayMeterの変化処理


	float nowRate = bossHpGaugeMeter.gaugeRate;
	float diff = bossHpGaugeDelayMeter.gaugeRate - nowRate;

	//delayゲージが本ゲージより大きい(減少した際)
	if (bossHpGaugeDelayMeter.gaugeRate > nowRate) {

		//エージングでの更新
		if (diff > bossHpGaugeDelayMeter.threshold) {
			//始点と終点を更新
			bossHpGaugeDelayMeter.startRate = bossHpGaugeDelayMeter.gaugeRate;
			bossHpGaugeDelayMeter.endRate = nowRate;

			//easingTを更新
			bossHpGaugeDelayMeter.easingT += bossHpGaugeDelayMeter.followSpeed;
			if (bossHpGaugeDelayMeter.easingT > 1.0f) {
				bossHpGaugeDelayMeter.easingT = 1.0f;
			}

			//更新
			bossHpGaugeDelayMeter.gaugeRate =
				UpdateValueByEasing(
					bossHpGaugeDelayMeter.startRate,
					bossHpGaugeDelayMeter.endRate,
					bossHpGaugeDelayMeter.easingT,
					EasingMode::EaseIn
				);
		} else {
			bossHpGaugeDelayMeter.gaugeRate = nowRate;
			bossHpGaugeDelayMeter.easingT = 0.0f;
		}

	} else {
		bossHpGaugeDelayMeter.gaugeRate = nowRate;
		bossHpGaugeDelayMeter.easingT = 0.0f;
	}

#pragma endregion

}

static void DrawBossHpGauge(void) {
	if (!GetBossEnemyIsAlive()) return;

	//フレーム
	DrawTextureUI(bossHpGaugeFrame.texture, bossHpGaugeFrame.pos, bossHpGaugeFrame.size);
	//delayMeter
	DrawGaugeAsUI(bossHpGaugeDelayMeter.texture, bossHpGaugeFrame.pos, bossHpGaugeFrame.size, bossHpGaugeDelayMeter.gaugeRate);
	//メーター
	DrawGaugeAsUI(bossHpGaugeMeter.texture, bossHpGaugeFrame.pos, bossHpGaugeFrame.size, bossHpGaugeMeter.gaugeRate);

#ifdef _DEBUG

	//DrawTextureUI(lifeIcon.texture, GetEnergyGaugeEndPos(), lifeIcon.size);

#endif // _DEBUG


}

#pragma endregion

#pragma region 関数: 持続ボーナス

static void UpdatePowRate(void) {
	if (GetPlayerNowLaserLevel() == PlayerLaserLevel::Level2) {
		laserPowRate.powRate = GetPlayerLaserPowRate();
	}
}

static void DrawPowRate(void) {
	if (GetPlayerNowLaserLevel() != PlayerLaserLevel::Level2) return;

	if (!GetPlayerIsAlive()) return;

	/*Vector2i posS = laserPowRate.startPos;*/
	Vector2 playerPos = GetPlayerPos();
	Vector2i posS = ConvertPosWToS({ playerPos.x, playerPos.y + 40 });

	Novice::ScreenPrintf(posS.x, posS.y, "DAMAGE RATE: %.3f!!!", laserPowRate.powRate);
}

#pragma endregion

#pragma region 関数: 進捗度ゲージ

static void InitProgressGauge(void) {
	if (!kShowProgressGaugeSwitch) return;
	//ボスが出たら出現しない
	if (GetBossEnemyIsAlive()) return;

	progressGauge.frameTexture = GetTexture(TextureType::ProgressGauge);
	progressGaugeIcon.texture = GetTexture(TextureType::ProgressGaugeIcon);

	progressGauge.bossRoundTime = ConvertTimeDataToInt(GetBossRoundTimeData());
}

static void UpdateProgressGauge(void) {
	if (!kShowProgressGaugeSwitch) return;
	//ボスが出たら出現しない
	if (GetBossEnemyIsAlive()) return;

	progressGauge.gaugeRate = ToFloat(ConvertTimeDataToInt(GetGameTimeData())) / ToFloat( progressGauge.bossRoundTime);

	//アイコンの移動
	progressGaugeIcon.pos.x = UpdateValueByEasing(progressGaugeIcon.startPos.x, progressGaugeIcon.endPos.x, progressGauge.gaugeRate, EasingMode::Linear);
}

static void DrawProgressGauge(void) {
	if (!kShowProgressGaugeSwitch) return;
	//ボスが出たら出現しない
	if (GetBossEnemyIsAlive()) return;

	DrawTextureUI(progressGauge.frameTexture, progressGauge.pos, progressGauge.size);
	DrawTextureUI(progressGaugeIcon.texture, progressGaugeIcon.pos, progressGaugeIcon.size);

	//DrawGaugeAsUI(progressGauge.meterTexture, progressGauge.pos, progressGauge.size, progressGauge.gaugeRate);

}

#pragma endregion

#pragma region 関数: 連鎖表示

static void InitShockCombo(void) {
	for (int i = 0; i < kComboNumbersLimit; i++) {
		comboNumbers[i] = {};
	}
	comboNumbersTexture = GetTexture(TextureType::ComboNumbers);
}

static void UpdateShockCombo(void) {
	//表示の更新
	if (!shockCombo.isShowPreCombo) {
		shockCombo.combo = GetShockCombo();

		for (int i = 0; i < kComboNumbersLimit; i++) {
			comboNumbers[i].animNum = GetValueAtDigit(shockCombo.combo, i);
		}
	}
	else {
		//点滅処理
		shockCombo.invisibleTimer.count++;
		if (shockCombo.invisibleTimer.count >= shockCombo.invisibleTimer.time) {
			shockCombo.invisibleTimer.count = 0;

			if (!shockCombo.isInvisible) {
				shockCombo.isInvisible = true;
			} else {
				shockCombo.isInvisible = false;
			}
		}

		shockCombo.showPreComboTimer.count++;
		if (shockCombo.showPreComboTimer.count >= shockCombo.showPreComboTimer.time) {
			shockCombo.showPreComboTimer.count = 0;
			shockCombo.isShowPreCombo = false;

			shockCombo.isInvisible = false;
			shockCombo.invisibleTimer.count = 0;
		}
	}
}

static void DrawShockCombo(void) {

	if (shockCombo.isInvisible) return;

	int digits = 0;
	if (shockCombo.combo < 3) {
		return;
	}
	else if (shockCombo.combo < 10) {
		digits = 1;
	} 
	else if(shockCombo.combo < 100){
		digits = 2;
	} else {
		digits = 3;
	}

	//scoreNumbers
	for (int i = 0; i < digits; i++) {
		Vector2 pos = { comboNumbers[i].startPos.x + comboNumbers[i].interval.x * i, comboNumbers[i].startPos.y };

		Texture texture = comboNumbersTexture;
		texture.animNum = comboNumbers[i].animNum;

		DrawTextureUI(texture, pos, comboNumbers[i].size);
	}

}

#pragma endregion

void InitPlayUI(void) {
	InitLifeIcon();
	InitEnergyGauge();
	InitBossHpGauge();
	InitProgressGauge();
	InitShockCombo();
}

void UpdatePlayUI(void){
	UpdateEnergyGauge();
	UpdateBossHpGauge();
	UpdatePowRate();
	UpdateProgressGauge();
	UpdateShockCombo();
}

void DrawPlayUI(void) {
	DrawLifeIcon();
	DrawEnergyGauge();
	DrawBossHpGauge();
	DrawPowRate();
	DrawProgressGauge();
	DrawShockCombo();
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

void TriggerEndCombo(void) {
	shockCombo.isShowPreCombo = true;
	shockCombo.showPreComboTimer.count = 0;
}

#pragma endregion