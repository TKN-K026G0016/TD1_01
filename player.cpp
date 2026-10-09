#include "player.h"
#include "player_laser.h"
#include "elec_bullet.h"
#include "stage.h"
#include "enemy.h"
#include "gem.h"
#include "boss_enemy.h"

#include "vector2.h"
#include "tool.h"
#include "input.h"
#include "texture.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>


//自動ロックオンスイッチ
static constexpr bool kAutoLockOnSwitch = true;
//加速度移動スイッチ
static constexpr bool kAcceleratingSwitch = false;
//真ん中に維持するゲーム性にするスイッチ
static constexpr bool kModeKeepCenterSwitch = true;
//デンゲキ弾射撃スイッチ
static constexpr bool kShootElecBulletSwitch = true;

//ギリ避けスイッチ
constexpr bool kDodgeCloseSwitch = true;

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

	//<ギリ避け関係>
	float dodgeCloseRadius = 50.0f;

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

	//移動速度(等速)
	float moveSpeed[2] = {6.0f, 1.3f};

	Vector2 inputVec = { 0, 0 };

	//<回転関係>
	float rotateTheta = 0.0f;
	//回転速度
	float rotateSpeed = 0.05f;

	//<ロックオン関係>
	//ロック状態フラグ
	bool isLock = false;

	//回転速度
	float lockOnRotateSpeed = 0.2f;

	//<射撃関係>
	//射撃スパン
	Timer shootTimer = { 2, 0 };

	//<エネルギー関係>
	bool isEmptyEnergy = false;
	//エネルギー残量
	float remainEnergy = 0.0f;

	//故障時のenergy回復量
	float recoveryEnergyDuringEmpty = 0.3f;

	//<レベル関係>
	//レーザーのレベル
	PlayerLaserLevel laserLevel = PlayerLaserLevel::Level0;
	//レベルアップのライン
	float levelUpLine[ToInt(PlayerLaserLevel::Count)] = { 0.0f, 1.0f, 80.0f };

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
	Enemy2 = 1,
};
LockTargetType nowTargetType = LockTargetType::None;
//ターゲットの番号
int targetIndex = -1;

struct LockOnSign {
	Vector2 pos = {};
	Vector2 size = { 64, 64 };

	Texture texture = {};
};
LockOnSign lockOnSign;

#pragma endregion

#pragma region データ: 射撃関係

//プレイヤーの中心からの発射位置の距離
constexpr float kShootDisLength = 15.0f;

//Lv2レーザーの持続時間
int sustainLv2Count = 0;
//Lv2レーザーの攻撃力倍率
float laserPowRate = 1.0f;
constexpr float kLaserPowRateMin = 1.0f;
constexpr float kLaserPowRateMax = 3.0f;
//火力の上昇速度
constexpr float kLaserPowRateIncreaseSpeed = 0.01f;

#pragma endregion

#pragma region データ: エネルギー関係

//エネルギー上限
constexpr float kEnergyMax = 100.0f;
//エネルギー下限
constexpr float kEnergyMin = 0.0f;
//エネルギーゲージの中間
constexpr float kEnergyCenter = (kEnergyMax - kEnergyMin) / 2;

//エネルギー消費量
constexpr float kConsumptionEnergy = 0.1f;
//エネルギー消費量(レベルごと)
constexpr float kConsumptionEnergys[ToInt(PlayerLaserLevel::Count)] = { 0.0f, 0.1f, 0.7f };

#pragma endregion

#pragma region データ: ギリ避け関係

//ギリ避け時のボーナススパン
static Timer dodgeCloseBonusTimer = { 8, 0 };

#pragma endregion

/// <summary>
/// 入力検知
/// </summary>
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

	if (!kAutoLockOnSwitch) {
		if (CheckInputAction(InputAction::Lock)) {
			player.isLock = true;
		} else {
			player.isLock = false;
		}
	}

#pragma endregion

}

#pragma region 関数: 基本動作

static void MovePlayer(void) {
	if (!player.isAlive) {
		return;
	}

	//加速度移動
	if (kAcceleratingSwitch) {

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
	}
	//通常移動
	else {
		Vector2 moveVec = player.inputVec;
		float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
		if (moveVecLength != 0) {
			moveVec.x /= moveVecLength;
			moveVec.y /= moveVecLength;
		}

		float moveSpeed = (!player.isEmptyEnergy) ? player.moveSpeed[0] : player.moveSpeed[1];

		player.pos.x += moveSpeed * moveVec.x;
		player.pos.y += moveSpeed * moveVec.y;
	}
}

static void ClampPlayerPos(void) {
	//横
	if (player.pos.x < movablePos[0].x) {
		player.pos.x = movablePos[0].x;
	} else if (player.pos.x > movablePos[1].x) {
		player.pos.x = movablePos[1].x;
	}

	//縦
	if (player.pos.y < movablePos[0].y) {
		player.pos.y = movablePos[0].y;
	} else if (player.pos.y > movablePos[1].y) {
		player.pos.y = movablePos[1].y;
	}
}

#pragma endregion

#pragma region 関数: 回転&ロックオン関係

/// <summary>
/// 通常時の回転処理
/// </summary>
/// <param name=""></param>
static void RotatePlayer(void) {
	if (player.isLock) return;

	//手動ロックオン
	if (!kAutoLockOnSwitch) {
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
	//オートロックオン
	else {

		float closestDis = 99999.0f;
		nowTargetType = LockTargetType::None;
		targetIndex = -1;

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
				targetIndex = i;
				nowTargetType = LockTargetType::Enemy1;
			}
		}

		Enemy2* enemy2 = GetEnemy2Array();
		int enemy2Limit = GetEnemy2Limit();

		for (int i = 0; i < enemy2Limit; i++) {
			if (!enemy2[i].isAlive) continue;

			float dx = enemy2[i].pos.x - player.pos.x;
			float dy = enemy2[i].pos.y - player.pos.y;
			float dis = sqrtf(dx * dx + dy * dy);

			if (dis < closestDis) {
				closestDis = dis;
				targetIndex = i;
				nowTargetType = LockTargetType::Enemy2;
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
				targetIndex = 0;
				nowTargetType = LockTargetType::Boss;
			}
		}

		// ターゲットが死んだらロック解除
		if (nowTargetType == LockTargetType::None) return;

		Vector2 targetPos = {};

		//ボス
		if (nowTargetType == LockTargetType::Boss) {
			if (!boss->isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = boss->pos;
		} else if (nowTargetType == LockTargetType::Enemy1) {
			if (!enemy1[targetIndex].isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = enemy1[targetIndex].pos;
		} else if (nowTargetType == LockTargetType::Enemy2) {
			if (!enemy2[targetIndex].isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = enemy2[targetIndex].pos;
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

}

/// <summary>
/// ロックオン時の回転処理
/// </summary>
/// <param name=""></param>
static void RotatePlayerByLockOn(void) {
	if (!kAutoLockOnSwitch) {
		if (!player.isLock) return;

		Enemy1* enemy1 = GetEnemy1Array();
		Enemy2* enemy2 = GetEnemy2Array();
		BossEnemy* boss = GetBossEnemy();

		// ターゲットが死んだらロック解除
		if (nowTargetType == LockTargetType::None) return;

		Vector2 targetPos = {};

		//ボス
		if (nowTargetType == LockTargetType::Boss) {
			if (!boss->isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = boss->pos;
		} else if (nowTargetType == LockTargetType::Enemy1) {
			if (!enemy1[targetIndex].isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = enemy1[targetIndex].pos;
		} else if (nowTargetType == LockTargetType::Enemy2) {
			if (!enemy2[targetIndex].isAlive) {
				nowTargetType = LockTargetType::None;
				return;
			}
			targetPos = enemy2[targetIndex].pos;
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
}

/// <summary>
/// ターゲットの決定処理
/// </summary>
/// <param name=""></param>
static void LockOn(void) {
	if (!kAutoLockOnSwitch) {

		if (!CheckInputAction(InputAction::TriggerLock)) return;
		if (!player.isAlive) return;

		float closestDis = 99999.0f;
		nowTargetType = LockTargetType::None;
		targetIndex = -1;

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
				targetIndex = i;
				nowTargetType = LockTargetType::Enemy1;
			}
		}

		Enemy2* enemy2 = GetEnemy2Array();
		int enemy2Limit = GetEnemy2Limit();

		for (int i = 0; i < enemy2Limit; i++) {
			if (!enemy2[i].isAlive) continue;

			float dx = enemy2[i].pos.x - player.pos.x;
			float dy = enemy2[i].pos.y - player.pos.y;
			float dis = sqrtf(dx * dx + dy * dy);

			if (dis < closestDis) {
				closestDis = dis;
				targetIndex = i;
				nowTargetType = LockTargetType::Enemy2;
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
				targetIndex = 0;
				nowTargetType = LockTargetType::Boss;
			}
		}
	}
}


/// <summary>
/// ロックオンマークの初期化処理
/// </summary>
/// <param name=""></param>
static void InitLockSign(void) {
	lockOnSign = {};
	lockOnSign.texture = GetTexture(TextureType::LockOnSign);
}

static void UpdateLockOnSign(void) {
	if (!player.isLock || nowTargetType == LockTargetType::None) return;

	//ボスの場合、
	if (nowTargetType == LockTargetType::Boss) {
		BossEnemy* boss = GetBossEnemy();

		lockOnSign.pos = boss->pos;
	}
	//enemy1
	else if (nowTargetType == LockTargetType::Enemy1) {
		Enemy1* enemy1 = GetEnemy1Array();
		lockOnSign.pos = enemy1[targetIndex].pos;
	}
	//enemy2
	else if (nowTargetType == LockTargetType::Enemy2){
		Enemy2* enemy2 = GetEnemy2Array();
		lockOnSign.pos = enemy2[targetIndex].pos;
	}

}

static void DrawLockOnSign(void) {
	if (!player.isLock || nowTargetType == LockTargetType::None) return;

	DrawTextureObj(lockOnSign.texture, lockOnSign.pos, lockOnSign.size);
}

#pragma endregion

#pragma region 関数: 射撃・エネルギー関係

/// <summary>
/// レーザーの射撃処理
/// </summary>
static void ShootLaser(void) {
	if (!player.isAlive) return;

	if (kShootElecBulletSwitch) return;

	if (!kModeKeepCenterSwitch) {
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
	else {
		if (player.isEmptyEnergy) return;

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
}

static void ShootElecBullet(void) {
	if (!player.isAlive) return;

	if (!kShootElecBulletSwitch) return;

	if (player.isEmptyEnergy) return;

	if (CheckInputAction(InputAction::Shoot)) {
		//射撃位置設定
		Vector2 shootPos;
		shootPos.x = player.pos.x + kShootDisLength * cosf(player.rotateTheta);
		shootPos.y = player.pos.y + kShootDisLength * sinf(player.rotateTheta);

		ShootElecBullet(shootPos, player.rotateTheta);
	}

}

/// <summary>
/// レベルアップ処理
/// </summary>
static void LevelUp(void) {
	if (!player.isAlive) return;

	if (!kModeKeepCenterSwitch) {
		int level = ToInt(player.laserLevel);

		//レベル2以上はレベルアップなし
		if (level >= ToInt(PlayerLaserLevel::Level2)) return;

		float levelUpLine = player.levelUpLine[level + 1];
		if (player.remainEnergy >= levelUpLine) {
			player.laserLevel = static_cast<PlayerLaserLevel>(level + 1);
		}
	}
}

/// <summary>
/// エネルギーの消費処理
/// </summary>
static void ConsumptionEnergy(void) {
	if (!player.isAlive) return;

	if (!kModeKeepCenterSwitch) {
		int level = ToInt(player.laserLevel);

		//レベル0は消費なし
		if (level == 0) return;

		player.remainEnergy -= kConsumptionEnergys[level];

		if (player.remainEnergy <= 0.0f) {
			player.laserLevel = PlayerLaserLevel::Level0;
			player.remainEnergy = 0.0f;
		}
	} 
	//中心に保つゲーム性
	else {
		player.remainEnergy -= kConsumptionEnergy;

		if (player.remainEnergy <= 0.0f) {
			player.remainEnergy = 0.0f;
		}
	}
}

/// <summary>
/// Lv2継続ボーナス関係の処理
/// </summary>
/// <param name=""></param>
static void UpdateSustainLv2Bonus(void) {
	if (!player.isAlive) return;

	if (!kModeKeepCenterSwitch) {
		//Lv2状態時のみ実行
		if (player.laserLevel == PlayerLaserLevel::Level2) {
			sustainLv2Count++;

			//初期値 + (maxまでの数値 * (1.0f - 指数関数(0に近づく)))
			laserPowRate = kLaserPowRateMin +
				(kLaserPowRateMax - kLaserPowRateMin) * (1.0f - expf(-kLaserPowRateIncreaseSpeed * sustainLv2Count));

		}
		//それ以外の状態時、攻撃力の倍率を戻す
		else {
			sustainLv2Count = 0;
			laserPowRate = kLaserPowRateMin;
		}
	}
}

/// <summary>
/// ゲージ維持失敗の確認処理
/// </summary>
/// <param name=""></param>
static void CheckFailedMaintenance(void) {
	if (!player.isAlive) return;

	if (kModeKeepCenterSwitch) {
		if (player.isEmptyEnergy) return;

		if (player.remainEnergy <= kEnergyMin || player.remainEnergy >= kEnergyMax) {
			player.isEmptyEnergy = true;
			player.remainEnergy = 0.0f;
		}
	}
}

/// <summary>
/// 故障中のエネルギー回復処理
/// </summary>
/// <param name=""></param>
static void RecoveryEnergyDuringEmptyEnergy(void) {
	if (!player.isAlive) return;

	if (kModeKeepCenterSwitch) {
		if (!player.isEmptyEnergy) return;

		player.remainEnergy += player.recoveryEnergyDuringEmpty;
		if (player.remainEnergy >= kEnergyCenter) {
			player.isEmptyEnergy = false;
		}
	}
}

#pragma endregion

#pragma region 関数: 死亡判定など

/// <summary>
/// 無敵時の処理
/// </summary>
/// <param name=""></param>
static void UpdateInvinciblePlayer(void) {
	if (!player.isInvincible) return;

#pragma region 点滅処理

	invisibleTimer.count++;
	if (invisibleTimer.count >= invisibleTimer.time) {
		invisibleTimer.count = 0;
		if (!isInvisible) {
			isInvisible = true;
		} else {
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
	player.remainEnergy = kEnergyCenter;

	GetMovablePos(movablePos);
	InitLockSign();
}

void UpdatePlayer(void) {
	CheckInput();

	MovePlayer();
	ClampPlayerPos();

	LockOn();
	RotatePlayer();
	RotatePlayerByLockOn();
	UpdateLockOnSign();

	ShootLaser();
	ShootElecBullet();

	CheckFailedMaintenance();
	RecoveryEnergyDuringEmptyEnergy();

	ConsumptionEnergy();
	LevelUp();
	UpdateSustainLv2Bonus();


	UpdateInvinciblePlayer();
	DeadPlayer();

	ReplenishmentEnergy();
}

void DrawPlayer(void) {
	if (!player.isAlive || isInvisible) return;

	DrawTextureRotateObj(player.texture, player.pos, player.size, player.rotateTheta);


	DrawLockOnSign();

#ifdef _DEBUG
	DrawHitEllipse(player.pos, player.dodgeCloseRadius, BLUE, false);

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
	return kEnergyMax;
}

float GetPlayerLaserPowRate(void) {
	return laserPowRate;
}

void RecoveryEnergy(float recoveryValue) {
	player.remainEnergy += recoveryValue;
	if (player.remainEnergy > kEnergyMax) {
		player.remainEnergy = kEnergyMax;
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

bool GetDodgeCloseSwitch(void) {
	return kDodgeCloseSwitch;
}

float GetPlayerDodgeCloseRadius(void) {
	return player.dodgeCloseRadius;
}

void TriggerDodgeClose(Vector2 eBulletPos) {
	if (!player.isAlive || player.isInvincible) return;

	dodgeCloseBonusTimer.count++;
	if (dodgeCloseBonusTimer.count >= dodgeCloseBonusTimer.time) {
		dodgeCloseBonusTimer.count = 0;

		SpawnGem(eBulletPos, GemType::S);
	}
}

#pragma endregion