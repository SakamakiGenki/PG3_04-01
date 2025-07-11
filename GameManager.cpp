#include "GameManager.h"
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#include <Novice.h>

GameManager::GameManager() {

	//シーン配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	//シーン設定
	currentSceneNo_ = TITLE;
	prevSceneNo_ = currentSceneNo_;
}

GameManager::~GameManager(){}

int GameManager::Run() {

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 前のシーンに現在のシーンを代入
		prevSceneNo_ = currentSceneNo_;

		// 現在のシーンの番号を受け取る
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		// 前のシーンと現在のシーンが違ったら現在シーンの初期化
		if (prevSceneNo_ != currentSceneNo_)
			sceneArr_[currentSceneNo_]->Initialize();
		
		// 現在シーンの更新
		sceneArr_[currentSceneNo_]->Update(keys, preKeys);

		// 現在シーンの描画
		sceneArr_[currentSceneNo_]->Draw();

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	return 0;
}