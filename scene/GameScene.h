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
#include"Flame.h"
#include"SmallSwitch.h"
#include"NormalSwitch.h"
#include"Wind.h"
#include"Pitfall.h"

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
	//複数化に必要な関数
	void LoadWallPopData();

	void UpdateWallPopCommands();

	void WallGeneration(const Vector3& position);

	void Stage1LoadWallPopData();

	void Stage1UpdateWallPopCommands();

	void Stage1WallGeneration(const Vector3& position);

	void LoadFlamePopData();

	void UpdateFlamePopCommands();

	void FlameGeneration(const Vector3& position);

	void LoadWindPopData();

	void UpdateWindPopCommands();

	void WindGeneration(const Vector3& position);

	//--------------------------------------------//

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

	//ギミックの宣言
	//炎の壁の宣言
	std::list<std::unique_ptr<Flame>> flames_;
	//小スイッチの宣言
	std::unique_ptr<SmallSwitch> smallswitch_;
	//中スイッチの宣言
	std::unique_ptr<NormalSwitch> normalswitch_;
	//風のギミックの宣言
	std::list<std::unique_ptr<Wind>> winds_;
	//落とし穴の宣言
	std::unique_ptr<Pitfall> pitfall_;

	// 壁発生コマンド
	std::stringstream wallPopCommands;
	// 壁発生コマンド
	std::stringstream stage1wallPopCommands;
	//炎発生コマンド
	std::stringstream flamePopCommands;
	//風のギミックの発生コマンド
	std::stringstream windPopCommands;

	//ステージを分けるためのフラグ
	bool istutorial_ = false;
	bool isstage1_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
