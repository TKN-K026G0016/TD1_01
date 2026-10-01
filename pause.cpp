#include "pause.h"
#include "input.h"
#include "scene_manager.h"

#include "texture.h"
#include "tool.h"
#include "common.h"

struct CoverPanel {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	unsigned int color = 0x00000099;

	Texture texture = {};
};
CoverPanel pauseCoverPanel = {};

void Resume(void) {
	if (CheckInputAction(InputAction::Pause)) {
		ChangeSceneImmediate(Scene::Play);
	}
}

void InitPause(void) {

	//テクスチャデータの取得
	if (pauseCoverPanel.texture.tHandle == -1) {
		pauseCoverPanel.texture = GetTexture(TextureType::White1x1);
		pauseCoverPanel.texture.color = pauseCoverPanel.color;
	}
}

void UpdatePause(void) {
	Resume();
}

void DrawPause(void) {
	DrawTextureUI(pauseCoverPanel.texture, pauseCoverPanel.pos, pauseCoverPanel.size);
}