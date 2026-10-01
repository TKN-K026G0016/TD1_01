#include "play.h"
#include "input.h"
#include "scene_manager.h"

#include <Novice.h>

static void Pause(void) {
	if (CheckInputAction(InputAction::Pause)) {
		ChangeScene(Scene::Pause);
	}
}


void InitPlay(void) {

}

void UpdatePlay(void) {
	Pause();
}

void DrawPlay(void) {
#ifdef _DEBUG

	Novice::ScreenPrintf(20, 20, "PlayScene");

#endif // _DEBUG
}