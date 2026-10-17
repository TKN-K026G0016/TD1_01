#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "title_background.h"

#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"
#include "tool.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>


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
	InitTitleBackground();
}

void UpdateTitle(void) {
	Confirm();

	UpdateTitleBackground();
}

void DrawTitle(void) {
	DrawTitleBackground();


}