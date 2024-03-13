#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "AxisIndicator.h"
#include <ImGuiManager.h>
#include"Mymath.h"

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

	// 自キャラの生成 bz
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	modelPlayerHead_.reset(Model::CreateFromOBJ("Player", true));
	// 自キャラの初期化
	player_->Initialize(modelPlayerHead_.get());

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	modelwall_.reset(Model::CreateFromOBJ("Block", true));

	//玉の生成
	ball_ = std::make_unique<Ball>();
	//3Dモデルの生成
	modelBall_.reset(Model::CreateFromOBJ("Ball", true));
	//玉の初期化
	ball_->Initialize(modelBall_.get());
	
	//回復の生成
	recovery_ = std::make_unique<Recovery>();
	//3Dモデルの生成
	modelRecovery_.reset(Model::CreateFromOBJ("Recovery", true));
	//回復の初期化
	recovery_->Initialize(modelRecovery_.get());
	recoveryTime_ = 0;

	//複数の壁を読み込むための関数
	LoadWallPopData();
	Stage1LoadWallPopData();

	//ステージの生成と初期化
	/*stage_ = std::make_unique<Stage>();
	stage_->Initialize(model_);*/
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

	// 天球の更新
	skydome_->Update();

	//チュートリアルのフラグを立てるためのif文
	if (input_->TriggerKey(DIK_A))
	{

		istutorial_ = true;
		isstage1_ = false;
	}
	//ステージ1のフラグを立てるためのif文
	if (input_->TriggerKey(DIK_B))
	{

		isstage1_ = true;
		istutorial_ = false;
	}
	//仮自キャラの更新
	debugPlayer_->Update();
	//壁の更新
	wall_->Update();


	//当たり判定
	CheckAllCollision();
}


	//チュートリアルのフラグがたったら実行する
	if (istutorial_)
	{
		//ステージの更新
		for (const std::unique_ptr<Stage>& stage : stages_) {
			if (stage != nullptr) {
				stage->Update();

			}
		}
		//複数の壁を出すための関数
		UpdateWallPopCommands();
	}

	if (isstage1_)
	{
		//ステージの更新
		for (const std::unique_ptr<Stage1>& stage1 : stages1_) {
			if (stage1 != nullptr) {
				stage1->Update();

			}
		}
		//複数の壁を出すための関数
		Stage1UpdateWallPopCommands();

		//玉
		if (ball_) {
			ball_->Update();
		}
		if (ball_ && ball_->IsDead()) {
			ball_.reset();
		}
		
		//回復
		if (recovery_) {
			recovery_->Update();
		//消す
			if (recovery_->IsDead()) {
				recovery_.reset();
			}
		}

		if (!recovery_) {
			recoveryTime_++;

			if (recoveryTime_ >= 180) {
				//回復の生成
				recovery_ = std::make_unique<Recovery>();
				//回復の初期化
				recovery_->Initialize(modelRecovery_.get());
				recoveryTime_ = 0;
			}
		}
	}

	//当たり判定
	CheckAllCollisions();
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
	

	//チュートリアルのフラグがたったら実行する
	if (istutorial_)
	{
		//ステージの描画
		for (const auto& stage : stages_) {

			stage->Draw(viewProjection_);

		}
	}

	if (isstage1_)
	{
		//ステージの描画
		for (const auto& stage1 : stages1_) {

			stage1->Draw(viewProjection_);

		}
		//玉
		if (ball_) {
			ball_->Draw(viewProjection_);
		}
		//回復
		if (recovery_) {
			recovery_->Draw(viewProjection_);
		}
	}

	// 天球の描画
	skydome_->Draw(viewProjection_);


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

void GameScene::WallGeneration(const Vector3& position) {
	// 敵の生成
	Stage* stage = new Stage();



	stage->Initialize(modelwall_.get(), position);
	stage->SetGameScene(this);

	stages_.push_back(static_cast<std::unique_ptr<Stage>>(stage));
}

void GameScene::Stage1LoadWallPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//wallPop2.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage1wallPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::Stage1UpdateWallPopCommands()
{
	bool iswait = false;
	int32_t waitTimer = 0;

	// 待機処理
	if (iswait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			iswait = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line2;

	// コマンド実行ループ
	while (getline(stage1wallPopCommands, line2)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line2);

		std::string word2;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word2, ',');
		//"//"から始まる行はコメント
		if (word2.find("//") == 0) {
			// コメント行は飛ばす
			continue;
		}

		// POPコマンド
		if (word2.find("POP") == 0) {
			// x座標
			getline(line_stream, word2, ',');
			float x = (float)std::atof(word2.c_str());

			// y座標
			getline(line_stream, word2, ',');
			float y = (float)std::atof(word2.c_str());

			// z座標
			getline(line_stream, word2, ',');
			float z = (float)std::atof(word2.c_str());

			// 敵を発生させる
			Stage1WallGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage1WallGeneration(const Vector3& position)
{
	// 敵の生成
	Stage1* stage1 = new Stage1();



	stage1->Initialize(modelwall_.get(), position);
	stage1->SetGameScene(this);

	stages1_.push_back(static_cast<std::unique_ptr<Stage1>>(stage1));
}

void GameScene::LoadWallPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//wallPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	wallPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateWallPopCommands()
{

	bool iswait = false;
	int32_t waitTimer = 0;

	// 待機処理
	if (iswait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			iswait = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line2;

	// コマンド実行ループ
	while (getline(wallPopCommands, line2)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line2);

		std::string word2;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word2, ',');
		//"//"から始まる行はコメント
		if (word2.find("//") == 0) {
			// コメント行は飛ばす
			continue;
		}

		// POPコマンド
		if (word2.find("POP") == 0) {
			// x座標
			getline(line_stream, word2, ',');
			float x = (float)std::atof(word2.c_str());

			// y座標
			getline(line_stream, word2, ',');
			float y = (float)std::atof(word2.c_str());

			// z座標
			getline(line_stream, word2, ',');
			float z = (float)std::atof(word2.c_str());

			// 敵を発生させる
			WallGeneration(Vector3(x, y, z));
		}
	}

}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 PosA, PosB;
	Vector3 RadiusA, RadiusB;
	float PositionMeasure;
	float RadiusMeasure;

#pragma region プレイヤーと玉
	if (ball_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//玉の座標
		PosB = ball_->GetWorldPosition();
		RadiusB = ball_->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			ball_->OnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤーと回復
	if (recovery_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//回復の座標
		PosB = recovery_->GetWorldPosition();
		RadiusB = recovery_->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			recovery_->OnCollision();
		}
	}
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