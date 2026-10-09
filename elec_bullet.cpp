#include "elec_bullet.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <math.h>

constexpr static int kElecBulletLimit = 5;

//デンゲキ弾
struct ElecBullet {
	Vector2 pos;
	Vector2 size = { 32, 32 };

	//<移動関係>
	float moveSpeed = 20.0f;
	float moveTheta = 0.0f;

	//<射撃フラグ>
	bool isShoot = false;
	Timer breakTimer = { 60, 0 };

	//当たり判定
	float hitRadius = 20;

	//攻撃力
	int pow = 1;

	Texture texture = {};
};
ElecBullet elecBullet[kElecBulletLimit] = {};

//デンゲキショック
struct ElecShock {
	float hitRadius = 30.0f;
	int pow = 1;
};
ElecShock elecShock = {};

#pragma region 関数: デンゲキ弾関係

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

void InitElecBullet(void) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		elecBullet[i] = {};
		elecBullet[i].texture = GetTexture(TextureType::PlayerLaser0);
	}
}

void UpdateElecBullet(void) {
	MoveBullet();
}

void DrawElecBullet(void) {
	DrawBullet();
}

#pragma region 関数: 外部参照関係

void ShootElecBullet(Vector2 pos, float moveTheta) {
	for (int i = 0; i < kElecBulletLimit; i++) {
		if (elecBullet[i].isShoot) continue;

		elecBullet[i].isShoot = true;

		elecBullet[i].pos = pos;
		elecBullet[i].moveTheta = moveTheta;


		break;
	}
}

void BreakElecBullet(int index) {
	elecBullet[index].isShoot = false;
	elecBullet[index].breakTimer.count = 0;
}

#pragma endregion