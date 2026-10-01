#include "stage.h"
#include "vector2.h"

struct Camera {
	Vector2 pos = { 0, 0 };

	float zoom = 1.0f;
	float zoomMax = 1.0f;
	float zoomMin = 0.5f;

};
Camera camera;

void InitStage(void) {

}

void UpdateStage(void) {

}

void DrawStage(void) {

}

Vector2 GetCameraPos(void) {
	return camera.pos;
}

float GetCameraZoom(void) {
	return camera.zoom;
}