#include "player_laser.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <math.h>

#pragma region データ

constexpr int kLaserLimit = 30;

struct Laser {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 30, 30 };

	//<移動関係>
	float moveSpeed = 20.0f;
	float moveTheta = 0.0f;

	//射撃フラグ
	bool isShoot = false;

	//自壊タイマー
	Timer breakTimer = { 120, 0 };

};
Laser laser[kLaserLimit];

Texture laserTexture = {};

#pragma endregion

#pragma region 関数

static void MoveLaser(void) {
	for (int i = 0; i < kLaserLimit; i++) {
		if (!laser[i].isShoot) {
			continue;
		}

		laser[i].pos.x += cosf(laser[i].moveTheta) * laser[i].moveSpeed;
		laser[i].pos.y += sinf(laser[i].moveTheta) * laser[i].moveSpeed;
	}
}

static void BreakLaser(int index) {
	laser[index].isShoot = false;
	laser[index].breakTimer.count = 0;
}

static void BreakLaserItSelf(void) {
	for (int i = 0; i < kLaserLimit; i++) {
		if (!laser[i].isShoot) {
			continue;
		}

		laser[i].breakTimer.count++;
		if (laser[i].breakTimer.count >= laser[i].breakTimer.time) {
			BreakLaser(i);
		}
	}
}

#pragma endregion

void InitPlayerLaser(void) {
	for (int i = 0; i < kLaserLimit; i++) {
		laser[i] = {};
	}
	if (laserTexture.tHandle == -1) {
		laserTexture = GetTexture(TextureType::PlayerLaser1);
	}
}

void UpdatePlayerLaser(void) {
	MoveLaser();
	BreakLaserItSelf();
}

void DrawPlayerLaser(void) {
	for (int i = 0; i < kLaserLimit; i++) {
		if (!laser[i].isShoot) continue;

		DrawTextureRotateObj(laserTexture, laser[i].pos, laser[i].size, laser[i].moveTheta);
	}
}

void ShootPlayerLaser(Vector2 pos, float moveTheta) {
	for (int i = 0; i < kLaserLimit; i++) {
		if (laser[i].isShoot) continue;

		laser[i].isShoot = true;

		laser[i].pos = pos;
		laser[i].moveTheta = moveTheta;

		break;
	}
}