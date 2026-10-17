#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "title_background.h"
#include "title_player.h"
#include "elec_bullet.h"
#include "effect.h"

#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"
#include "tool.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum class ModeTitle {
	MovePlayer,
	TriggerLightUp,
	WaitInput,
};
ModeTitle nowMode = ModeTitle::MovePlayer;

#pragma region データ: ロゴ

struct Logo {
	Vector2 pos = { 640, 580 };
	Vector2 size = { 640, 256 };

	Texture texture = {};
};
static Logo logo;

#pragma endregion

#pragma region 関数: 項目決定

static void Confirm(void) {
	if (nowMode != ModeTitle::WaitInput) return;

	if (CheckInputAction(InputAction::Confirm)) {
		if (GetNowModeFade() != ModeFade::Standby) {
			return;
		}

		ChangeModeFade(ModeFade::FadeOut);
		ChangeFadeTarget(FadeTarget::Play);
		//PlaySe(SeType::Sample);
	}
}

#pragma endregion

#pragma region ロゴ関係

static void InitLogo(void) {
	logo = {};
	logo.texture = GetTexture(TextureType::TitleLogo);
}

static void UpdateLogo(void) {

}

static void DrawLogo(void) {
	DrawTextureUI(logo.texture, logo.pos, logo.size);
}

#pragma endregion

void InitTitle(void) {
	InitTitleBackground();
	InitTitlePlayer();
	InitElecBullet();
	InitEffect();

	InitLogo();
}

void UpdateTitle(void) {
	UpdateTitlePlayer();
	UpdateElecBullet();
	UpdateEffect();

	Confirm();
	UpdateLogo();

	UpdateTitleBackground();
}

void DrawTitle(void) {
	DrawTitleBackground();
	DrawElecBullet();
	DrawEffect();

	DrawLogo();

	DrawTitlePlayer();
}