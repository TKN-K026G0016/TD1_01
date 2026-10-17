#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"

#include <Novice.h>

#pragma region データ: 背景

struct Background {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	unsigned int color = 0x1A020AFF;

	Texture texture = {};
};
Background background;

#pragma endregion

#pragma region 関数: 背景

static void InitBackground(void) {
	background = {};
	background.texture = GetTexture(TextureType::White1x1);
}

static void UpdateBackground(void) {

}

static void DrawBackground(void) {
	//背景
	Texture texture = background.texture;
	texture.color = background.color;
	DrawTextureUI(texture, background.pos, background.size);
}

#pragma endregion

#pragma region 関数: 項目決定

static void Confirm(void) {
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

void InitTitle(void) {
	InitBackground();
}

void UpdateTitle(void) {
	Confirm();


	UpdateBackground();
}

void DrawTitle(void) {
	DrawBackground();



}