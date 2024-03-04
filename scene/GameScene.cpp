#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <ImGuiManager.h>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();

	viewProjection_.translation_ = { 0.0f,130.0f,0.0f };
	viewProjection_.rotation_ = { -11.0f,0.0f,0.0f };
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	modelPlayerHead_.reset(Model::CreateFromOBJ("Player", true));
	// 自キャラの初期化
	player_->Initialize(modelPlayerHead_.get());

	debugCamera_ = std::make_unique<DebugCamera>(1280,720);

	//ステージの生成と初期化
	stage_ = std::make_unique<Stage>();
	stage_->Initialize(model_);
	//-------------------------//



	// 3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("Skydome", true));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get());

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	debugCamera_->Update();

	ImGui::Begin("viewprojection");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x);
	ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x);
	ImGui::End();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraAcctive_ = true;
	}
	if (isDebugCameraAcctive_) {

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {

		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
#endif

// 自キャラの更新
	player_->Update();

	//ステージの更新
	stage_->Update();

	
	// 天球の更新
	skydome_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// 自キャラの描画
	player_->Draw(viewProjection_);

	//ステージの描画
	stage_->Draw(viewProjection_);

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
