#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

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

	debugCamera_ = std::make_unique<DebugCamera>(1280,720);

	modelwall_.reset(Model::CreateFromOBJ("Block", true));
	
	//複数の壁を読み込むための関数
	LoadWallPopData();
	Stage1LoadWallPopData();
	LoadFlamePopData();
	LoadWindPopData();
	
	//ステージの生成と初期化
	/*stage_ = std::make_unique<Stage>();
	stage_->Initialize(model_);*/
	//-------------------------//

	/*flame_ = std::make_unique<Flame>();
	flame_->Initialize(model_);*/

	smallswitch_ = std::make_unique<SmallSwitch>();
	smallswitch_->Initialize(model_);

	normalswitch_ = std::make_unique<NormalSwitch>();
	normalswitch_->Initialize(model_);

	//wind_ = std::make_unique<Wind>();
	//wind_->Initialize(model_);

	pitfall_ = std::make_unique<Pitfall>();
	pitfall_->Initialize(model_);

}

void GameScene::Update() {



	debugCamera_->Update();

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
	pitfall_->Update();

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
		//複数の炎ギミックを出すための関数
		UpdateFlamePopCommands();
	}

	
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

		//炎の描画
		for (const auto& flame : flames_) {
			flame->Draw(viewProjection_);
		}
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
	pitfall_->Draw(viewProjection_);
	//stage1_->Draw(viewProjection_);

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