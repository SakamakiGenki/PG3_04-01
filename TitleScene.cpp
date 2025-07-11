#include "TitleScene.h"
#include <Novice.h>

void TitleScene::Initialize() {}

void TitleScene::Update(char* keys, char* preKeys) {

	// スペースキーで次のシーンへ
	if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
		
		sceneNo = STAGE;
	}
}

void TitleScene::Draw() {

	Novice::ScreenPrintf(0, 0, "Curernt Scene : Title");
	Novice::ScreenPrintf(0, 30, "Push Space : Next Scene");
}