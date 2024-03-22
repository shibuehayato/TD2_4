#pragma once
#include <Input.h>

// シーンクラスを作るたびにここに種類を追加
enum SceneType {
	kTitle,       // タイトルシーン
	kOperation,	  // 操作説明シーン
	kStageSelect, // ステージ選択シーン
	kGame,		  // ゲームシーン
	kClear,		  // クリアシーン
	kGameOver,	  // ゲームオーバーシーン
	kSceneMax	  // シーンの最大数
};

class IScene
{
protected:
	// シーン番号を管理する変数
	static int sceneNo;

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState_;
	XINPUT_STATE prejoyState_;

public:
	// 継承先で実装される関数
	// 抽象クラスなので純粋仮想関数とする
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 仮想デストラクタを用意しないと警告される
	virtual ~IScene();

	// シーン番号のゲッター
	int GetSceneNo();

	void SetKeys(XINPUT_STATE joyState, XINPUT_STATE prejoyState);
};