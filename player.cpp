#include "player.h"
#include "player_laser.h"
#include "stage.h"
#include "enemy.h"
#include "boss_enemy.h"

#include "vector2.h"
#include "tool.h"
#include "input.h"
#include "texture.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

//ロックオンスイッチ
bool lockOnSwitch = true;

#pragma region データ: player本体
struct Player {
	Vector2 pos = { 600, 600 };
	Vector2 size = { 32, 32 };

	//<生存関係>
	bool isAlive = true;
	//当たり判定の大きさ
	float hitRadius = 10.0f;
	int hp = 5;
	int hpMax = 5;

	//<無敵関係>
	//無敵フラグ
	bool isInvincible = false;
	//無敵の持続時間
	Timer invincibleTimer = { 100, 0 };


	//<移動関係>
	Vector2 velocity = { 0.0f, 0.0f };
	//加速度
	float accleretionSpeed = 0.14f;
	//スピード上限
	float moveSpeedLimit = 9.0f;
	//減速度
	float decelerationSpeed = 0.15f;

	Vector2 inputVec = { 0, 0 };

	//<回転関係>
	float rotateTheta = 0.0f;
	//回転速度
	float rotateSpeed = 0.05f;

	//<ロックオン関係>
	//ロック状態フラグ
	bool isLock = false;
	//ターゲットの番号
	int targetIndex = -1;
	//回転速度
	float lockOnRotateSpeed = 0.1f;

	//<射撃関係>
	//射撃スパン
	Timer shootTimer = { 2, 0 };

	//<エネルギー関係>
	//エネルギー残量
	float remainEnergy = 0.0f;
	//エネルギー上限
	float energyLimit = 100.0f;
	//エネルギー消費量(レベルごと)
	float consumptionEnergy[ToInt(PlayerLaserLevel::Count)] = { 0.0f, 0.15f, 0.55f };

	//<レベル関係>
	//レーザーのレベル
	PlayerLaserLevel laserLevel = PlayerLaserLevel::Level0;
	//レベルアップのライン
	float levelUpLine[ToInt(PlayerLaserLevel::Count)] = { 0.0f, 1.0f, 100.0f };

	Texture texture = {};
};
Player player;

//<無敵時の点滅関係>
//点滅のスパン
static bool isInvisible = false;
static Timer invisibleTimer = { 5, 0 };

#pragma endregion

#pragma region データ: 可動域
Vector2 movablePos[2] = { {0, 0}, {0, 0} };

#pragma endregion

#pragma region データ: ロックオン関係

enum class LockTargetType {
	None = -1,
	Boss = -2,
	Enemy1 = 0,
};

#pragma endregion

#pragma region データ: 射撃関係

//プレイヤーの中心からの発射位置の距離
constexpr float kShootDisLength = 15.0f;

#pragma endregion

#pragma region 関数: 基本動作

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

		//反対側に勢いがあれば、減速度も加える
		if (player.velocity.x < 0.0f) {
			player.velocity.x += player.decelerationSpeed;
		}

	}
	//左入力
	else if (player.inputVec.x <= -1.0f) {
		player.velocity.x -= player.accleretionSpeed;

		//反対側に勢いがあれば、減速度も加える
		if (player.velocity.x > 0.0f) {
			player.velocity.x -= player.decelerationSpeed;
		}
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

		//反対側に勢いがあれば、減速度も加える
		if (player.velocity.y < 0.0f) {
			player.velocity.y += player.decelerationSpeed;
		}
	}
	//下入力
	else if (player.inputVec.y <= -1.0f) {
		player.velocity.y -= player.accleretionSpeed;

		//反対側に勢いがあれば、減速度も加える
		if (player.velocity.y > 0.0f) {
			player.velocity.y -= player.decelerationSpeed;
		}
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

/// <summary>
/// 通常時の回転処理
/// </summary>
/// <param name=""></param>
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

/// <summary>
/// ロックオン時の回転処理
/// </summary>
/// <param name=""></param>
static void RotatePlayerByLockOn(void) {
	if (!player.isLock) return;

	Enemy1* enemy1 = GetEnemy1Array();
	BossEnemy* boss = GetBossEnemy();

	// ターゲットが死んだらロック解除
	if (player.targetIndex == ToInt(LockTargetType::None)) return;

	Vector2 targetPos;

	if (player.targetIndex == ToInt(LockTargetType::Boss)) {
		if (!boss->isAlive) {
			player.targetIndex = ToInt(LockTargetType::None);
			return;
		}
		targetPos = boss->pos;
	} else {
		if (!enemy1[player.targetIndex].isAlive) {
			player.targetIndex = ToInt(LockTargetType::None);
			return;
		}
		targetPos = enemy1[player.targetIndex].pos;
	}

	// 回転処理
	float dx = targetPos.x - player.pos.x;
	float dy = targetPos.y - player.pos.y;

	float targetTheta = atan2f(dy, dx);
	float diff = targetTheta - player.rotateTheta;

	//差を-π~πに正規化
	while (diff > ToFloat(M_PI)) {
		diff -= ToFloat(M_PI) * 2;
	}
	while (diff < -ToFloat(M_PI)) {
		diff += ToFloat(M_PI) * 2;
	}

	player.rotateTheta += diff * player.lockOnRotateSpeed;
}

/// <summary>
/// ターゲットの決定処理
/// </summary>
/// <param name=""></param>
static void LockOn(void) {
	if (!CheckInputAction(InputAction::TriggerLock)) return;
	if (!player.isAlive) return;

	float closestDis = 99999.0f;
	player.targetIndex = ToInt(LockTargetType::None);

	Enemy1* enemy1 = GetEnemy1Array();
	int enemy1Limit = GetEnemy1Limit();

	// enemy1探索
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		float dx = enemy1[i].pos.x - player.pos.x;
		float dy = enemy1[i].pos.y - player.pos.y;
		float dis = sqrtf(dx * dx + dy * dy);

		if (dis < closestDis) {
			closestDis = dis;
			player.targetIndex = i;
		}
	}

	// boss探索
	BossEnemy* boss = GetBossEnemy();
	if (boss->isAlive) {
		float dx = boss->pos.x - player.pos.x;
		float dy = boss->pos.y - player.pos.y;
		float dis = sqrtf(dx * dx + dy * dy);

		if (dis < closestDis) {
			closestDis = dis;
			player.targetIndex = ToInt(LockTargetType::Boss);
		}
	}

}

#pragma endregion

#pragma region 関数: 射撃・エネルギー関係

static void ShootLaser(void) {
	if (!player.isAlive) return;

	player.shootTimer.count++;
	if (player.shootTimer.count >= player.shootTimer.time) {
		player.shootTimer.count = 0;

		//射撃位置設定
		Vector2 shootPos;
		shootPos.x = player.pos.x + kShootDisLength * cosf(player.rotateTheta);
		shootPos.y = player.pos.y + kShootDisLength * sinf(player.rotateTheta);

		ShootPlayerLaser(shootPos, player.rotateTheta, kShootDisLength);
	}
}

static void LevelUp(void) {
	if (!player.isAlive) return;

	int level = ToInt(player.laserLevel);

	//レベル2以上はレベルアップなし
	if (level >= 2) return;

	float levelUpLine = player.levelUpLine[level + 1];
	if (player.remainEnergy >= levelUpLine) {
		player.laserLevel = static_cast<PlayerLaserLevel>(level + 1);
	}
}

/// <summary>
/// エネルギーの消費処理
/// </summary>
static void ConsumptionEnergy(void) {
	if (!player.isAlive) return;

	int level = ToInt(player.laserLevel);

	//レベル0は消費なし
	if (level == 0) return;

	player.remainEnergy -= player.consumptionEnergy[level];

	if (player.remainEnergy <= 0.0f) {
		player.laserLevel = PlayerLaserLevel::Level0;
		player.remainEnergy = 0.0f;
	}
}

#pragma endregion

#pragma region 関数: その他

/// <summary>
/// 無敵時の処理
/// </summary>
/// <param name=""></param>
static void UpdateInvinciblePlayer(void) {
	if(!player.isInvincible) return;
		
#pragma region 点滅処理

	invisibleTimer.count++;
	if (invisibleTimer.count >= invisibleTimer.time) {
		invisibleTimer.count = 0;
		if (!isInvisible) {
			isInvisible = true;
		}
		else {
			isInvisible = false;
		}
	}

#pragma endregion

	//制限時間のカウント処理
	player.invincibleTimer.count++;
	if (player.invincibleTimer.count >= player.invincibleTimer.time) {
		player.invincibleTimer.count = 0;

		//無敵フラグを折る
		player.isInvincible = false;
		
		//透明処理をリセット
		isInvisible = false;
		invisibleTimer.count = 0;
	}
}

/// <summary>
/// 死亡判定の処理
/// </summary>
static void DeadPlayer(void) {
	if (player.hp <= 0) {
		player.hp = 0;

		player.isAlive = false;
	}
}

#pragma endregion

#pragma region 関数: デバッグ

static void ReplenishmentEnergy(void) {
#ifdef _DEBUG

	//if (CheckInputAction(InputAction::Lock)) {
	//	player.remainEnergy += 0.9f;
	//}

#endif // _DEBUG

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

	LockOn();
	RotatePlayer();
	RotatePlayerByLockOn();

	ShootLaser();

	LevelUp();
	ConsumptionEnergy();

	UpdateInvinciblePlayer();
	DeadPlayer();

	ReplenishmentEnergy();
}

void DrawPlayer(void) {
	if (!player.isAlive || isInvisible) return;

	DrawTextureRotateObj(player.texture, player.pos, player.size, player.rotateTheta);

#ifdef _DEBUG
	//Novice::ScreenPrintf(20, 60, "NowLevel: %d", ToInt(player.laserLevel));
	//Novice::ScreenPrintf(20, 80, "RemainEnergy: %.1f/%.1f", player.remainEnergy, player.energyLimit);

#endif // _DEBUG

}

#pragma region 関数: 外部参照関係

Vector2 GetPlayerPos(void) {
	return player.pos;
}

float GetPlayerHitRadius(void) {
	return player.hitRadius;
}

int GetPlayerHp(void) {
	return player.hp;
}

bool GetPlayerIsAlive(void) {
	return player.isAlive;
}

float GetPlayerRemainEnergy(void) {
	return player.remainEnergy;
}

float GetPlayerEnergyLimit(void) {
	return player.energyLimit;
}

void RecoveryEnergy(float recoveryValue) {
	player.remainEnergy += recoveryValue;
	if (player.remainEnergy > player.energyLimit) {
		player.remainEnergy = player.energyLimit;
	}
}

float GetPlayerRotateTheta(void) {
	return player.rotateTheta;
}

PlayerLaserLevel GetPlayerNowLaserLevel(void) {
	return player.laserLevel;
}

void PlayerDamage(void) {
	if (!player.isAlive || player.isInvincible) return;
	
	//体力減らす
	player.hp--;

	if (player.hp > 0) {
		//無敵開始
		player.isInvincible = true;

	}
}

#pragma endregion