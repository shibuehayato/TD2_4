#pragma once
#include <memory>
#include "IScene.h"
#include "TitleScene.h"
#include "Operation.h"
#include "StageSelect.h"

class GameManager
{
private:
	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> sceneArr_[10];

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_;  // 現在のシーン
	int preSceneNo_;	  // 前のシーン

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState_ = {0};
	XINPUT_STATE prejoyState_ = {0};

public:
	GameManager();  // コンストラクタ
	~GameManager();	// デストラクタ

	int Run(); // この関数でゲームループを呼び出す
};