#include "player.h"
#include "input.h"
#include "texture.h"

#include "vector2.h"

#include <math.h>

#pragma region データ
struct Player {
	Vector2 pos = { 400, 400 };
	Vector2 size = { 32, 32 };

	//<生存関係>
	bool isAlive = true;

	//<移動関係>
	float moveSpeed = 7.0f;

	Vector2 inputVec = { 0, 0 };

	//<回転関係>
	float rotateTheta = 0.0f;



	Texture texture = {};
};
Player player;

#pragma endregion

#pragma region 関数

static void CheckInput(void) {

#pragma region 移動関係
	//横
	if (CheckInputAction(InputAction::MoveRight)) {
		player.inputVec.x = 1;
	}
	else if (CheckInputAction(InputAction::MoveLeft)) {
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

}

static void MovePlayer(void) {
	if (!player.isAlive) {
		return;
	}

	Vector2 moveVec = player.inputVec;
	float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
	if (moveVecLength != 0) {
		moveVec.x /= moveVecLength;
		moveVec.y /= moveVecLength;
	}

	player.pos.x += player.moveSpeed * moveVec.x;
	player.pos.y += player.moveSpeed * moveVec.y;
}

#pragma endregion

void InitPlayer(void) {
	player = {};
	player.texture = GetTexture(TextureType::Player);
}

void UpdatePlayer(void) {
	CheckInput();

	MovePlayer();
}

void DrawPlayer(void) {
	DrawTextureRotateObj(player.texture, player.pos, player.size, player.rotateTheta);
}

#pragma region 関数: 外部参照関係

Vector2 GetPlayerPos(void) {
	return player.pos;
}

#pragma endregion