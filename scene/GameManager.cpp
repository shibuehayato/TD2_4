#include "GameManager.h"

GameManager::GameManager()
{
	// 各シーンの配列
	sceneArr_[kTitle] = std::make_unique<TitleScene>();
	sceneArr_[kStageSelect] = std::make_unique<StageSelect>();

	for (int i = 0; i < kSceneMax; i++) {
		// ゲームマネージャーのキーを全てのシーンに反映させる
		sceneArr_[i]->SetKeys(joyState_, prejoyState_);
	}

	// 初期シーン
	currentSceneNo_ = kTitle;
}

GameManager::~GameManager()
{}

int GameManager::Run()
{
	// シーンチェック
	preSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	// シーン変更チェック
	if (preSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
	}

	// 更新処理
	sceneArr_[currentSceneNo_]->Update();

	// 描画処理
	sceneArr_[currentSceneNo_]->Draw();

	return 0;
}