#include "player.h"
#include "player_laser.h"
#include "stage.h"

#include "vector2.h"
#include "tool.h"
#include "input.h"
#include "texture.h"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma region データ
struct Player {
	Vector2 pos = { 400, 400 };
	Vector2 size = { 32, 32 };

	//<生存関係>
	bool isAlive = true;

	//<移動関係>
	Vector2 velocity = { 0.0f, 0.0f };
	//加速度
	float accleretionSpeed = 0.1f;
	//スピード上限
	float moveSpeedLimit = 9.0f;
	//減速度
	float decelerationSpeed = 0.12f;

	Vector2 inputVec = { 0, 0 };

	//<回転関係>
	float rotateTheta = 0.0f;
	//回転速度
	float rotateSpeed = 0.05f;
	//ロック状態フラグ
	bool isLock = false;

	//<射撃関係>
	Timer shootTimer = { 2, 0 };

	Texture texture = {};
};
Player player;

#pragma endregion

#pragma region データ: 可動域
Vector2 movablePos[2] = { {0, 0}, {0, 0} };

#pragma endregion

#pragma region 関数

static void CheckInput(void) {

#pragma region 移動関係
	//横
	if (CheckInputAction(InputAction::MoveRight)) {
		player.inputVec.x = 1;
	} else if (CheckInputAction(InputAction::MoveLeft)) {
		player.inputVec.x = -1;
	} else {
		player.inputVec.x = 0;
	}
	//縦
	if (CheckInputAction(InputAction::MoveUp)) {
		player.inputVec.y = 1;
	} else if (CheckInputAction(InputAction::MoveDown)) {
		player.inputVec.y = -1;
	} else {
		player.inputVec.y = 0;
	}

#pragma endregion

#pragma region ロック
	if (CheckInputAction(InputAction::Lock)) {
		player.isLock = true;
	}
	else {
		player.isLock = false;
	}

#pragma endregion

}

static void MovePlayer(void) {
	if (!player.isAlive) {
		return;
	}

	//右入力
	if (player.inputVec.x >= 1.0f) {
		player.velocity.x += player.accleretionSpeed;
	}
	//左入力
	else if (player.inputVec.x <= -1.0f) {
		player.velocity.x -= player.accleretionSpeed;
	}
	//入力なし(=減速)
	else {
		if (player.velocity.x > 0.0f) {
			player.velocity.x -= player.decelerationSpeed;
			if (player.velocity.x < 0.0f) {
				player.velocity.x = 0.0f;
			}
		} else if (player.velocity.x < 0.0f) {
			player.velocity.x += player.decelerationSpeed;
			if (player.velocity.x > 0.0f) {
				player.velocity.x = 0.0f;
			}
		}
	}

	//上入力
	if (player.inputVec.y >= 1.0f) {
		player.velocity.y += player.accleretionSpeed;
	}
	//下入力
	else if (player.inputVec.y <= -1.0f) {
		player.velocity.y -= player.accleretionSpeed;
	}
	//入力なし(=減速)
	else {
		if (player.velocity.y > 0.0f) {
			player.velocity.y -= player.decelerationSpeed;
			if (player.velocity.y < 0.0f) {
				player.velocity.y = 0.0f;
			}
		} else if (player.velocity.y < 0.0f) {
			player.velocity.y += player.decelerationSpeed;
			if (player.velocity.y > 0.0f) {
				player.velocity.y = 0.0f;
			}
		}
	}

	//速度調整
	float speed = sqrtf(player.velocity.x * player.velocity.x + player.velocity.y * player.velocity.y);
	if (speed > player.moveSpeedLimit) {
		float scale = player.moveSpeedLimit / speed;
		player.velocity.x *= scale;
		player.velocity.y *= scale;
	}

	//位置更新
	player.pos.x += player.velocity.x;
	player.pos.y += player.velocity.y;

#pragma region 通常移動
	//Vector2 moveVec = player.inputVec;
	//float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
	//if (moveVecLength != 0) {
	//	moveVec.x /= moveVecLength;
	//	moveVec.y /= moveVecLength;
	//}

	//player.pos.x += player.moveSpeed * moveVec.x;
	//player.pos.y += player.moveSpeed * moveVec.y;
#pragma endregion
}

static void ClampPlayerPos(void) {
	//横
	if (player.pos.x < movablePos[0].x) {
		player.pos.x = movablePos[0].x;
	}
	else if (player.pos.x > movablePos[1].x) {
		player.pos.x = movablePos[1].x;
	}

	//縦
	if (player.pos.y < movablePos[0].y) {
		player.pos.y = movablePos[0].y;
	} else if (player.pos.y > movablePos[1].y) {
		player.pos.y = movablePos[1].y;
	}
}

static void RotatePlayer(void) {
	if (player.isLock) return;

	//入力なしならスキップ
	if (player.inputVec.x == 0.0f && player.inputVec.y == 0.0f) return;

	//入力方向の角度
	float inputTheta = atan2f(player.inputVec.y, player.inputVec.x);
	//現在の方向との差を求める
	float diff = inputTheta - player.rotateTheta;

	//差を-π~πに正規化
	while (diff > ToFloat(M_PI)) {
		diff -= ToFloat(M_PI) * 2;
	} 
	while (diff < -ToFloat(M_PI)) {
		diff += ToFloat(M_PI) * 2;
	}

	player.rotateTheta += diff * player.rotateSpeed;
}

static void ShootLaser(void) {
	if (!player.isAlive) return;

	player.shootTimer.count++;
	if (player.shootTimer.count >= player.shootTimer.time) {
		player.shootTimer.count = 0;

		ShootPlayerLaser(player.pos, player.rotateTheta);
	}
}

#pragma endregion

void InitPlayer(void) {
	player = {};
	player.texture = GetTexture(TextureType::Player);

	GetMovablePos(movablePos);
}

void UpdatePlayer(void) {
	CheckInput();

	MovePlayer();
	ClampPlayerPos();
	RotatePlayer();
	ShootLaser();
}

void DrawPlayer(void) {
	DrawTextureRotateObj(player.texture, player.pos, player.size, player.rotateTheta);
}

#pragma region 関数: 外部参照関係

Vector2 GetPlayerPos(void) {
	return player.pos;
}

float GetPlayerRotateTheta(void) {
	return player.rotateTheta;
}

#pragma endregion