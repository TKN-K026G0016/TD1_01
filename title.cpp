#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"

#include <Novice.h>

Vector2 titleLogoPos = kWindowCenter;
Texture titleLogoTexture = {};

void InitTitle(void) {
	titleLogoTexture = GetTexture(TextureType::TitleLogo);
}

void UpdateTitle(void) {
	if (CheckInputAction(InputAction::Confirm)) {
		if (GetNowModeFade() != ModeFade::Standby) {
			return;
		}

		ChangeModeFade(ModeFade::FadeOut);
		ChangeFadeTarget(FadeTarget::Play);
		PlaySe(SeType::Sample);
	}
}

void DrawTitle(void) {

	DrawTextureUI(titleLogoTexture, titleLogoPos, { 800, 450 });

#ifdef _DEBUG

	Novice::ScreenPrintf(20, 20, "TitleScene");

#endif // _DEBUG


}