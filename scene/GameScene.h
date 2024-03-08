#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include"Stage.h"
#include<memory>
#include"DebugCamera.h"
#include<list>
#include <sstream>
#include"Stage1.h"
#include"Ball.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void LoadWallPopData();

	void UpdateWallPopCommands();

	void WallGeneration(const Vector3& position);

	void Stage1LoadWallPopData();

	void Stage1UpdateWallPopCommands();

	void Stage1WallGeneration(const Vector3& position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isDebugCameraAcctive_=false;


	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデルデータ
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	// 3Dモデルの生成
	std::unique_ptr<Model> modelPlayerHead_;

	//ステージの壁についての変数
	std::unique_ptr<Stage> stage_;
	std::list<std::unique_ptr<Stage>> stages_;
	std::unique_ptr<Model> modelwall_;

	std::list<std::unique_ptr<Stage1>> stages1_;

	//玉
	std::unique_ptr<Model> modelBall_;
	std::unique_ptr<Model> modelCube_;
	std::unique_ptr <Ball> ball_;

	// 壁発生コマンド
	std::stringstream wallPopCommands;
	// 壁発生コマンド
	std::stringstream stage1wallPopCommands;
	//ステージを分けるためのフラグ
	bool istutorial_ = false;
	bool isstage1_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
