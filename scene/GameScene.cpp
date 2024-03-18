#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
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

	//複数の壁やギミックを読み込むための関数
	//複数の壁
	LoadWallPopData();
	Stage1LoadWallPopData();
	//ギミック
	LoadFlamePopData();
	LoadWindPopData();
	LoadBarrierPopData();
	LoadBarrier2PopData();
	LoadPitfallPopData();

	//--------------------//
	
	

	//小さいスイッチの生成と初期化
	smallswitch_ = std::make_unique<SmallSwitch>();
	smallswitch_->Initialize(model_);

	//普通のスイッチの生成と初期化
	normalswitch_ = std::make_unique<NormalSwitch>();
	normalswitch_->Initialize(model_);

	

	warp_ = std::make_unique<Warp>();
	warp_->Initialize(model_);


	warp2_ = std::make_unique<Warp2>();
	warp2_->Initialize(model_);

	
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

	

	//右矢印の生成と初期化
	rightarrow_ = std::make_unique<RightArrow>();
	rightarrow_->Initialize(model_);
	//---------------------------------//

	//左矢印の生成と初期化
	leftarrow_ = std::make_unique<LeftArrow>();
	leftarrow_->Initialize(model_);
	//

	//上矢印の生成と初期化
	uparrow_ = std::make_unique<UpArrow>();
	uparrow_->Initialize(model_);
	//下方向の生成と初期化
	downarrow_ = std::make_unique_for_overwrite<DownArrow>();
	downarrow_->Initialize(model_);

	rotatingarrow_ = std::make_unique<RotatingArrow>();
	rotatingarrow_->Initialize(model_);

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


	//チュートリアルのフラグがたったら実行する
	if (istutorial_)
	{
		//ステージの更新
		for (const std::unique_ptr<Tutorial>& stage : tutorials_) {
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
		//複数の炎ギミックを出すための関数
		UpdateFlamePopCommands();

		for (const std::unique_ptr<Flame>& flame : flames_) {
			flame->Update();
		}
		//小スイッチの更新
		smallswitch_->Update();
		//中スイッチの更新
		normalswitch_->Update();
		//風のギミックの更新
		for (const std::unique_ptr<Wind>& wind : winds_) {
			wind->Update();
		}
		UpdateWindPopCommands();
		//落とし穴の更新
		for (const std::unique_ptr<Pitfall>& pitfall : pitfalls_) {
			pitfall->Update();
		}
		UpdatePitfallPopCommands();
		//バリアの更新
		for (const std::unique_ptr<Barrier>& barrier : barriers_) {
			barrier->Update();

		}
		UpdateBarrierPopCommands();
		//2つめのバリアの更新
		for (const std::unique_ptr<Barrier2>& barrier2 : barriers2_) {
			barrier2->Update();
		}
		UpdateBarrier2PopCommands();


		//ワープの更新
		warp_->Update();
		//2つめのワープの更新
		warp2_->Update();
		//右矢印の更新
		rightarrow_->Update();
		//左矢印の更新
		leftarrow_->Update();
		//上矢印の更新
		uparrow_->Update();
		//下矢印の更新
		downarrow_->Update();
		//回転矢印の更新
		rotatingarrow_->Update();



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
	if (istutorial_ || isstage1_)
	{
		player_->Draw(viewProjection_);
	}
	
	//チュートリアルのフラグがたったら実行する
	if (istutorial_)
	{
		//ステージの描画
		for (const auto& stage : tutorials_) {

			stage->Draw(viewProjection_);

		}
		
	}

	if (isstage1_)
	{
		//ステージの描画
		for (const auto& stage1 : stages1_) {

			stage1->Draw(viewProjection_);

		}

		//炎の描画
		for (const auto& flame : flames_) {
			flame->Draw(viewProjection_);
		}


		//小スイッチの描画
		smallswitch_->Draw(viewProjection_);

		//中スイッチの描画
		normalswitch_->Draw(viewProjection_);

		//風のギミックの描画
		for (const auto& wind : winds_) {
			wind->Draw(viewProjection_);
		}
		//落とし穴の描画
		for (const auto& pitfall : pitfalls_) {
			pitfall->Draw(viewProjection_);
		}
		if (ball_) {
			ball_->Draw(viewProjection_);
		}
		//回復
		if (recovery_) {
			recovery_->Draw(viewProjection_);
		}
	}

		//バリアの描画
		for (const auto& barrier : barriers_) {
			barrier->Draw(viewProjection_);
		}

		//2つめのバリアの描画
		for (const auto& barrier2 : barriers2_) {
			barrier2->Draw(viewProjection_);
		}

		//ワープの描画
		warp_->Draw(viewProjection_);
		//2つめのワープの描画
		warp2_->Draw(viewProjection_);
		//右矢印の描画
		rightarrow_->Draw(viewProjection_);
		//左矢印の描画
		leftarrow_->Draw(viewProjection_);
		//上矢印の描画
		uparrow_->Draw(viewProjection_);
		//下矢印の描画
		downarrow_->Draw(viewProjection_);
		//回転矢印の描画
		rotatingarrow_->Draw(viewProjection_);

	
	

	
	

	

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

void GameScene::LoadFlamePopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//FlamePop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	flamePopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateFlamePopCommands()
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
	while (getline(flamePopCommands, line2)) {
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
			FlameGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::FlameGeneration(const Vector3& position)
{
	// 敵の生成
	Flame* flame = new Flame();



	flame->Initialize(model_, position);
	flame->SetGameScene(this);

	flames_.push_back(static_cast<std::unique_ptr<Flame>>(flame));
}

void GameScene::LoadWindPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//WindPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	windPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateWindPopCommands()
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
	while (getline(windPopCommands, line2)) {
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
			WindGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::WindGeneration(const Vector3& position)
{
	// 敵の生成
	Wind* wind = new Wind();



	wind->Initialize(model_, position);
	wind->SetGameScene(this);

	winds_.push_back(static_cast<std::unique_ptr<Wind>>(wind));
}

void GameScene::LoadBarrierPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//BarrierPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	barrierPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateBarrierPopCommands()
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
	while (getline(barrierPopCommands, line2)) {
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
			BarrierGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::BarrierGeneration(const Vector3& position)
{
	// 敵の生成
	Barrier* barrier = new Barrier();



	barrier->Initialize(model_, position);
	barrier->SetGameScene(this);

	barriers_.push_back(static_cast<std::unique_ptr<Barrier>>(barrier));
}

void GameScene::WallGeneration(const Vector3& position) {
	// 敵の生成
	Tutorial* tutorial = new Tutorial();



	tutorial->Initialize(modelwall_.get(), position);
	tutorial->SetGameScene(this);

	tutorials_.push_back(static_cast<std::unique_ptr<Tutorial>>(tutorial));
}

void GameScene::Barrier2Generation(const Vector3& position)
{
	// 敵の生成
	Barrier2* barrier2 = new Barrier2();



	barrier2->Initialize(modelwall_.get(), position);
	barrier2->SetGameScene(this);

	barriers2_.push_back(static_cast<std::unique_ptr<Barrier2>>(barrier2));
}

void GameScene::LoadPitfallPopData()
{
	// ファイルを開く
	std::ifstream file;
	std::string filename = "Resources//PitfallPop.csv";
	file.open(filename);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	pitfallPopCommands << file.rdbuf();


	// ファイルを閉じる
	file.close();
}

void GameScene::UpdatePitfallPopCommands()
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
	while (getline(pitfallPopCommands, line2)) {
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
			PitfallGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::PitfallGeneration(const Vector3& position)
{
	// 敵の生成
	Pitfall* pitfall = new Pitfall();



	pitfall->Initialize(modelwall_.get(), position);
	pitfall->SetGameScene(this);

	pitfalls_.push_back(static_cast<std::unique_ptr<Pitfall>>(pitfall));
}

void GameScene::LoadBarrier2PopData()
{
	// ファイルを開く
	std::ifstream file;
	std::string filename = "Resources//Barrier2Pop.csv";
	file.open(filename);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	barrier2PopCommands << file.rdbuf();


	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateBarrier2PopCommands()
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
	while (getline(barrier2PopCommands, line2)) {
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
			Barrier2Generation(Vector3(x, y, z));
		}
	}
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
			player_->RecoveryOnCollision();
		}
	}
#pragma endregion

#pragma region プレイヤーと風
	if (recovery_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//風の座標
		for (const std::unique_ptr<Wind>& wind : winds_) {
			PosB = wind->GetWorldPosition();
			RadiusB = wind->GetRadius();
			// 座標AとBの距離を求める
			PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
				(PosB.y - PosA.y) * (PosB.y - PosA.y) +
				(PosB.z - PosA.z) * (PosB.z - PosA.z);
			RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
			// 弾と弾の交差判定
			if (PositionMeasure <= RadiusMeasure) {
				player_->WindOnCollision();
			}
		}
	}
#pragma endregion
}

