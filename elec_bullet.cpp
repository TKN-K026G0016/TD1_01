#include "elec_bullet.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <math.h>

//レベルごとの性能
struct ElecBulletSpec {
	Vector2 size;
	float hitRadius;

	int pow;

	Texture texture = {};
};

ElecBulletSpec elecBulletSpec[ToInt(ElecBulletType::Count)] = {
	//Normal弾
	{ {32, 32}, 20, 1, {}},
	//Charge弾
	{ {64, 64}, 32, 3, {} },
};

//レベルごとの性能
struct ElecShockSpec {
	float hitRadius;

	int pow;

	Texture texture = {};
};

ElecShockSpec elecShockSpec[ToInt(ElecBulletType::Count)] = {
	//Normal弾
	{ 120, 1, {}},
	//Charge弾
	{ 200, 3, {} },
};

constexpr static int kElecBulletLimit = 5;
ElecBullet elecBullet[kElecBulletLimit] = {};


constexpr static int kElecShockLimit = 50;
ElecShock elecShock[kElecShockLimit] = {};

#pragma region 関数: デンゲキ弾関係

static void InitBullet(void) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		elecBullet[i] = {};
	}

	elecBulletSpec[ToInt(ElecBulletType::Normal)].texture = GetTexture(TextureType::PlayerLaser0);
	elecBulletSpec[ToInt(ElecBulletType::Charge)].texture = GetTexture(TextureType::PlayerLaser1);
}

/// <summary>
/// デンゲキ弾の移動処理
/// </summary>
static void MoveBullet(void) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		if (!elecBullet[i].isShoot) continue;

		elecBullet[i].pos.x += cosf(elecBullet[i].moveTheta) * elecBullet[i].moveSpeed;
		elecBullet[i].pos.y += sinf(elecBullet[i].moveTheta) * elecBullet[i].moveSpeed;
	}
}

/// <summary>
/// デンゲキ弾の自壊タイマー進める処理
/// </summary>
static void BreakBulletItSelf(void) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		if (!elecBullet[i].isShoot) continue;

		elecBullet[i].breakTimer.count++;
		if (elecBullet[i].breakTimer.count >= elecBullet[i].breakTimer.time) {
			elecBullet[i].breakTimer.count = 0;
			BreakElecBullet(i);
		}
	}
}

/// <summary>
/// デンゲキ弾の描画処理
/// </summary>
static void DrawBullet(void) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		if (!elecBullet[i].isShoot) continue;

		DrawTextureRotateObj(elecBullet[i].texture, elecBullet[i].pos, elecBullet[i].size, elecBullet[i].moveTheta);
	}
}

#pragma endregion

#pragma region 関数: デンゲキショック関係

static void InitElecShock(void) {
	for (int i = 0; i < kElecShockLimit; i++) {
		elecShock[i] = {};
	}
}

static void BreakElecShockItSelf(void) {
	for (int i = 0; i < kElecShockLimit; i++) {
		if (!elecShock[i].isShoot) continue;

		elecShock[i].breakTimer.count++;
		if (elecShock[i].breakTimer.count >= elecShock[i].breakTimer.time) {
			elecShock[i].breakTimer.count = 0;
			BreakElecShock(i);
		}
	}
}

static void DrawElecShock(void) {
	for (int i = 0; i < kElecShockLimit; i++) {
		if (!elecShock[i].isShoot) continue;

		DrawHitEllipse(elecShock[i].pos, elecShock[i].hitRadius, elecShock[i].color, true);
	}
}

#pragma endregion

void InitElecBullet(void) {
	InitBullet();

	InitElecShock();
}

void UpdateElecBullet(void) {
	MoveBullet();
	BreakBulletItSelf();

	BreakElecShockItSelf();
}

void DrawElecBullet(void) {
	DrawBullet();

	DrawElecShock();
}

#pragma region 関数: 外部参照関係

void ShootElecBullet(Vector2 pos, float moveTheta, ElecBulletType type) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		if (elecBullet[i].isShoot) continue;

		elecBullet[i].isShoot = true;

		elecBullet[i].pos = pos;
		elecBullet[i].moveTheta = moveTheta;


		elecBullet[i].type = type;
		elecBullet[i].size = elecBulletSpec[ToInt(type)].size;
		elecBullet[i].hitRadius = elecBulletSpec[ToInt(type)].hitRadius;
		elecBullet[i].pow = elecBulletSpec[ToInt(type)].pow;

		elecBullet[i].texture = elecBulletSpec[ToInt(type)].texture;

		break;
	}
}

void BreakElecBullet(int index) {
	elecBullet[index].isShoot = false;
	elecBullet[index].breakTimer.count = 0;
}

ElecBullet* GetElecBulletArray(void) {
	return elecBullet;
}

int GetElecBulletLimit(void) {
	return kElecBulletLimit;
}

void SpawnElecShock(Vector2 pos, ElecBulletType type) {
	for (int i = 0; i < kElecShockLimit; i++) {
		if (elecShock[i].isShoot) continue;

		elecShock[i].isShoot = true;
		elecShock[i].pos = pos;

		elecShock[i].type = type;
		elecShock[i].hitRadius = elecShockSpec[ToInt(type)].hitRadius;
		elecShock[i].pow = elecShockSpec[ToInt(type)].pow;

		break;
	}
}

void BreakElecShock(int index) {
	elecShock[index].isShoot = false;
	elecShock[index].breakTimer.count = 0;
}

ElecShock* GetElecShockArray(void) {
	return elecShock;
}

int GetElecShockLimit(void) {
	return kElecShockLimit;
}

#pragma endregion