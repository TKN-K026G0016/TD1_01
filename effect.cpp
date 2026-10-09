
#include "effect.h"
#include "player.h"
#include "vector2.h"
#include "texture.h"
#include "timer.h"


#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

//データ
const int JemEffectNumber=10;
struct JemEffect
{
	Vector2 pos = { };
	Vector2 newPos = { };
	float theta = { };
	Vector2  size = { };
	unsigned int color = {0xffffffff };
	bool isShot = {false};
	Texture texture = {};
};
JemEffect jemEffect[JemEffectNumber];

const int GageEffectNumber = 20;
struct GageEffect
{
	Vector2 pos = { };
	Vector2 acceleration = { };
	Vector2 size = { };
	bool appear = { };
	unsigned int coler = { };
};
GageEffect gageEffect[GageEffectNumber];
bool isGage = false;
Timer GageTimer = {5};
#pragma region 関数: ジェム取得effect



void CreateJemEffect() {
	for (int i = 0; i < JemEffectNumber; i++)
	{
		if (jemEffect[i].isShot == false) {
			jemEffect[i].isShot = true;
			jemEffect[i].pos = GetPlayerPos();
			jemEffect[i].theta = 0.0f;
			jemEffect[i].size.x = 0;
			jemEffect[i].size.y = 0;
			jemEffect[i].color = 0xffffffff;
			break;
		}
	}
}

static void InitGemEffect(void) {
	for (int i = 0; i < JemEffectNumber; i++) {
		jemEffect[i].texture = GetTexture(TextureType::EffectSquare);
	}
}

static void UpdateGemEffect(void) {
	for (int i = 0; i < JemEffectNumber; i++)
	{
		if (jemEffect[i].isShot == true) {
			if (jemEffect[i].size.x <= 64&&jemEffect[i].size.y<=64) {
				jemEffect[i].size.y += 10;
				jemEffect[i].size.x += 10;
			}
			jemEffect[i].theta += 0.1f;

			jemEffect[i].newPos.x = jemEffect[i].pos.x + (-(jemEffect[i].size.x / 2)) * cosf(jemEffect[i].theta) - (-(jemEffect[i].size.x / 2)) * sinf(jemEffect[i].theta);
			jemEffect[i].newPos.y = jemEffect[i].pos.y + (-(jemEffect[i].size.y / 2)) * cosf(jemEffect[i].theta) + (-(jemEffect[i].size.y / 2)) * sinf(jemEffect[i].theta);
			
			if (jemEffect[i].color >= 0xffffff10) {
				if (jemEffect[i].size.x < 64&&jemEffect[i].size.y) {
					jemEffect[i].color -= 0x00000005;
				}
				else {
					jemEffect[i].color -= 0x00000015;
				}
			}
			else {
				jemEffect[i].color = 0xffffffff;
				jemEffect[i].isShot = false;
			}
		}
	}
}
void DrawGetEffect() {
	for (int i = 0; i < JemEffectNumber; i++)
	{
		if(jemEffect[i].isShot==true){
		DrawTextureRotateObj(jemEffect[i].texture, jemEffect[i].pos, jemEffect[i].size,jemEffect[i].theta);
		}
	}
}

#pragma endregion

#pragma region 関数: ゲージeffect
void CreateGage() {

}
#pragma endregion

#pragma region



void InitEffect() {
	InitGemEffect();
	CreateJemEffect();

}
void UpdateEffect() {
	
	UpdateGemEffect();

}
void DrawEffect() {
	DrawGetEffect();
}
