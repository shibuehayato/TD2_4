#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"Mymath.h"
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

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	modelPlayerHead_.reset(Model::CreateFromOBJ("cube", true));
	// 自キャラの初期化
	player_->Initialize(modelPlayerHead_.get());

	//仮自キャラの生成
	debugPlayer_ = std::make_unique<DebugPlayer>();
	//仮自キャラモデルの生成
	modelDebugPlayer_.reset(Model::CreateFromOBJ("Ice", true));
	//仮自キャラの初期化
	debugPlayer_->Initialize(modelDebugPlayer_.get());

	//壁の生成
	wall_ = std::make_unique<Wall>();
	//壁モデルの生成
	modelWall_.reset(Model::CreateFromOBJ("Wall", true));
	//壁の初期化
	wall_->Initialize(modelWall_.get());
}

void GameScene::Update() {
// 自キャラの更新
	player_->Update();
	//仮自キャラの更新
	debugPlayer_->Update();
	//壁の更新
	wall_->Update();


	//当たり判定
	CheckAllCollision();
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

    wall_->Draw(viewProjection_);
	
	debugPlayer_->Draw(viewProjection_);
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

void GameScene::CheckAllCollision()
{

	//球体だからかずれまくってる
	//// 判定対象AとBの座標
	//Vector3 PosA, PosB;
	//Vector3 RadiusA, RadiusB;
	//float PositionMeasure;
	//float RadiusMeasure;
	//// 壁座標
	//PosA = wall_->GetWorldPosition();
	//RadiusA = wall_->GetRadius();
	////仮のプレイヤー全ての当たり判定
	//PosB = debugPlayer_->GetWorldPosition();
	//RadiusB = debugPlayer_->GetRadius();
	//// 座標AとBの距離を求める
	//PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
	//	(PosB.y - PosA.y) * (PosB.y - PosA.y) +
	//	(PosB.z - PosA.z) * (PosB.z - PosA.z);
	//RadiusMeasure = (Dot(RadiusA, RadiusB));
	//// 弾と弾の交差判定
	//if (PositionMeasure <= RadiusMeasure) {
	//	debugPlayer_->WallOnCollision();
	//}


	//立方体で作る
	// 壁座標
	Vector3 PosA = wall_->GetWorldPosition();
	// プレイヤー座標
	Vector3 PosB = debugPlayer_->GetWorldPosition();

	Vector3 HalfSizeA = wall_->GetRadius();
	Vector3 HalfSizeB = debugPlayer_->GetRadius();

	// 各軸に沿った当たり判定
	bool collisionX = abs(PosA.x - PosB.x) <= (HalfSizeA.x + HalfSizeB.x);
	bool collisionY = abs(PosA.y - PosB.y) <= (HalfSizeA.y + HalfSizeB.y);
	bool collisionZ = abs(PosA.z - PosB.z) <= (HalfSizeA.z + HalfSizeB.z);

	// 当たり判定
	if (collisionX && collisionY && collisionZ) {
		debugPlayer_->WallOnCollision();
	}

}