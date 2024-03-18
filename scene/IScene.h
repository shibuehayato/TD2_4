#pragma once

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
	char* keys_;
	char* preKeys_;
public:
	// 継承先で実装される関数
	// 抽象クラスなので純粋仮想関数とする
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 仮想デストラクタを用意しないと警告される
	virtual ~IScene();

	// シーン番号のゲッター
};