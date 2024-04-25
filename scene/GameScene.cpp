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

	TitleTexture_ = TextureManager::Load("scene/title.png");
	OperationTexture_ = TextureManager::Load("scene/operation.png");
	ClearTexture_ = TextureManager::Load("scene/clear.png");
	GameOverTexture_ = TextureManager::Load("scene/GameOver.png");

	TitleSprite_ = std::make_unique<Sprite>();
	OperationSprite_ = std::make_unique<Sprite>();
	ClearSprite_ = std::make_unique<Sprite>();
	GameOverSprite_ = std::make_unique<Sprite>();

	TitleSprite_.reset(Sprite::Create(TitleTexture_, { 0, 0 }));
	OperationSprite_.reset(Sprite::Create(OperationTexture_, { 0, 0 }));
	ClearSprite_.reset(Sprite::Create(ClearTexture_, { 0, 0 }));
	GameOverSprite_.reset(Sprite::Create(GameOverTexture_, { 0, 0 }));

	//大きさ
	BigTexture_ = TextureManager::Load("Size/Big.png");
	MediumTexture_ = TextureManager::Load("Size/Medium.png");
	SmallTexture_ = TextureManager::Load("Size/Small.png");

	BigSprite_ = std::make_unique<Sprite>();
	MediumSprite_ = std::make_unique<Sprite>();
	SmallSprite_ = std::make_unique<Sprite>();

	BigSprite_.reset(Sprite::Create(BigTexture_, { 0, 0 }));
	MediumSprite_.reset(Sprite::Create(MediumTexture_, { 0, 0 }));
	SmallSprite_.reset(Sprite::Create(SmallTexture_, { 0, 0 }));

	viewProjection_.translation_ = { 0,167.0f,8.0f };
	viewProjection_.rotation_ = { -11.0f,0,0 };
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
	ball_->SetGameScene(this);
	//回復の生成
	recovery_ = std::make_unique<Recovery>();
	//3Dモデルの生成
	modelRecovery_.reset(Model::CreateFromOBJ("life", true));
	//回復の初期化
	recovery_->Initialize(modelRecovery_.get());
	recovery_->SetGameScene(this);
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
	LoadArrowPopData();
	LoadGoalWhitePopData();
	LoadGoalBlackPopData();
	LoadTutorialGoalWhitePopData();
	LoadTutorialGoalBlackPopData();

	Stage2LoadWallPopData();
	LoadStage2FlamePopData();
	LoadStage2BarrierPopData();
	LoadSpeedDownPopData();
	//--------------------//
	
	modelbarrier_.reset(Model::CreateFromOBJ("barrier", true));

	//小さいスイッチの生成と初期化
	smallswitch_ = std::make_unique<SmallSwitch>();
	modelsmallswitch_.reset(Model::CreateFromOBJ("switch_small", true));
	modelsmallbutton_.reset(Model::CreateFromOBJ("switch_push", true));
	smallswitch_->Initialize(modelsmallswitch_.get(),modelsmallbutton_.get());
	smallswitch_->SetGameScene(this);

	//普通のスイッチの生成と初期化
	normalswitch_ = std::make_unique<NormalSwitch>();
	modelnormalswitch_.reset(Model::CreateFromOBJ("switch_normal", true));
	modelnormalbutton_.reset(Model::CreateFromOBJ("switch_push", true));
	normalswitch_->Initialize(modelnormalswitch_.get(),modelnormalbutton_.get());
	normalswitch_->SetGameScene(this);
	

	warp_ = std::make_unique<Warp>();
	modelwarp_.reset(Model::CreateFromOBJ("warp", true));
	warp_->Initialize(modelwarp_.get());


	warp2_ = std::make_unique<Warp2>();
	warp2_->Initialize(modelwarp_.get());

	
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

	//ゴールのモデル
	modelGoalWhite_.reset(Model::CreateFromOBJ("White", true)); 
	modelGoalBlack_.reset(Model::CreateFromOBJ("Black", true));

	modelArrow_.reset(Model::CreateFromOBJ("Arrow", true));

	//右矢印の生成と初期化
	rightarrow_ = std::make_unique<RightArrow>();
	rightarrow_->Initialize(modelArrow_.get());
	//---------------------------------//

	//左矢印の生成と初期化
	leftarrow_ = std::make_unique<LeftArrow>();
	leftarrow_->Initialize(modelArrow_.get());
	//

	//上矢印の生成と初期化
	uparrow_ = std::make_unique<UpArrow>();
	uparrow_->Initialize(modelArrow_.get());
	//下方向の生成と初期化
	downarrow_ = std::make_unique_for_overwrite<DownArrow>();
	downarrow_->Initialize(modelArrow_.get());

	//rotatingarrow_ = std::make_unique<RotatingArrow>();
	//rotatingarrow_->Initialize(modelRotationArrow_.get());
	
	//rotatingarrow_->Initialize(modelRotationArrow_.get());	
	stage2recovery_ = std::make_unique<Stage2Recovery>();
	stage2recovery_->Initialize(modelRecovery_.get());
	stage2recovery_->SetGameScene(this);

	stage2rotatingarrow_ = std::make_unique<Stage2RotatingArrow>();
	stage2rotatingarrow_->Initialize(modelArrow_.get());
	
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

}

void GameScene::Update() {

	switch (scene)
	{
	case GameScene::TITLE: // タイトルシーン
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {

					scene = OPERATION;
				}
			}
		}
		break;
	case GameScene::OPERATION: // 操作説明シーン
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					scene = GAME;
				}
			}
		}
		break;
	case GameScene::GAME:

		debugCamera_->Update();

		ImGui::Begin("viewprojection");
		ImGui::DragFloat3("translation", &viewProjection_.translation_.x);
		ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x);
		ImGui::DragInt("rotation", &warpcooltime_);
		ImGui::Checkbox("isstage2", &isstage2_);
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
			isstage2_ = false;
		}

		//ステージ1のフラグを立てるためのif文
		if (input_->TriggerKey(DIK_B))
		{

			isstage1_ = true;
			istutorial_ = false;
			isstage2_ = false;
		}
		if (input_->TriggerKey(DIK_C))
		{
			istutorial_ = false;
			isstage1_ = false;
			isstage2_ = true;
			isballdead_ = false;
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
			//ゴール
			for (const std::unique_ptr<Goal>& goalW : TutorialGoalWhites_) {
				goalW->Update();
			}
			UpdateTutorialGoalWhitePopCommands();
			for (const std::unique_ptr<Goal>& goalB : TutorialGoalBlacks_) {
				goalB->Update();
			}
			UpdateTutorialGoalBlackPopCommands();

			//Xボタンを押すと全マップが見える処理
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
						if (IsFullMapCamera == false) {
							IsFullMapCamera = true;
						}
					}
				}
			}

			if (Input::GetInstance()->GetJoystickState(0, prejoyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
						if (IsFullMapCamera == true) {
							IsFullMapCamera = false;
						}
					}
				}
			}

			if (IsFullMapCamera == true) {
				viewProjection_.translation_ = { 0,167.0f,8.0f };
				viewProjection_.rotation_ = { -11.0f,0,0 };
			}

		}

		ImGui::Begin("FullMap");
		ImGui::Checkbox("FullMap", &IsFullMapCamera);
		ImGui::End();

		if (isstage1_ || isstage2_)
		{
			//中スイッチの更新
			normalswitch_->Update();
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

			for (const std::unique_ptr<Fire>& fire : fires_) {
				fire->Update();
			}
			//小スイッチの更新
			smallswitch_->Update();

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
			for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
				arrow->Update();
			}
			UpdateArrowPopCommands();
			//ゴール
			for (const std::unique_ptr<Goal>& goalW : GoalWhites_) {
				goalW->Update();
			}
			UpdateGoalWhitePopCommands();
			for (const std::unique_ptr<Goal>& goalB : GoalBlacks_) {
				goalB->Update();
			}
			UpdateGoalBlackPopCommands();

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
			//rotatingarrow_->Update();
		}

		if (isstage2_)
		{
			for (const std::unique_ptr<Stage2>& stage2 : stages2_) {
				if (stage2 != nullptr) {
					stage2->Update();
				}
			}
			Stage2UpdateWallPopCommands();
			for (const std::unique_ptr<Fire2>& fire2 : fires2_) {
				fire2->Update();
			}
			//複数の炎ギミックを出すための関数
			UpdateStage2FlamePopCommands();
			for (const std::unique_ptr<Stage2Barrier>& stage2barrier : stage2barriers_)
			{
				stage2barrier->Update();
			}
			UpdateStage2BarrierPopCommands();

			stage2rotatingarrow_->Update();
			for (const std::unique_ptr<SpeedDown>& speeddown : speeddowns_)
			{
				speeddown->Update();
			}
			UpdateSpeedDownPopCommands();

		}

		//回復
		if (isstage1_ && recovery_ || recovery_ && isstage2_) {
			recovery_->Update();
			//消す
			if (recovery_->IsDead()) {
				recovery_.reset();
			}
		}
		//回復
		if (isstage1_ && stage2recovery_ || stage2recovery_ && isstage2_) {
			stage2recovery_->Update();
			//消す
			if (stage2recovery_->IsDead()) {
				stage2recovery_.reset();
			}
		}


		if (!stage2recovery_) {
			recoveryTime_++;

			if (recoveryTime_ >= 180) {
				//回復の生成
				stage2recovery_ = std::make_unique<Stage2Recovery>();
				//回復の初期化
				stage2recovery_->Initialize(modelRecovery_.get());
				recoveryTime_ = 0;
			}
		}

		if (!recovery_) {
			recoveryTime_++;

			if (recoveryTime_ >= 180) {
				//回復の生成
				recovery_ = std::make_unique<Recovery>();
				//回復の初期化
				recovery_->SetGameScene(this);
				recovery_->Initialize(modelRecovery_.get());
				recoveryTime_ = 0;
			}
		}

		if (!ball_)
		{
			if (isstage2_ && isballdead_ == false) {
				//回復の生成
				ball_ = std::make_unique<Ball>();
				//回復の初期化
				ball_->SetGameScene(this);
				ball_->Initialize(modelBall_.get());
			}
		}
		//玉
		if (isstage1_ && ball_ || isstage2_ && ball_) {
			ball_->Update();
		}
		if (ball_ && ball_->IsDead()) {
			ball_.reset();
		}
		/*if (player_->IsMove()&&warpcooltime_<=10)
		{
			warpcooltime_++;
		}*/
		if (player_->IsMove() == false && movestoptime <= 10)
		{
			movestoptime++;
		}

		//バリアが解除されたかを確認する関数
		BarrierRemoved();
		//当たり判定
		CheckAllCollisions();

		////玉をとって位置に来たらクリア
		//if (player_->GetTransformZ() >= 71&&ball_==nullptr) {
		//		scene = CLEAR;
		//		player_->Initialize(modelPlayerHead_.get());
		//}

		//大きさ合わせ
		if (player_->GetPlayerScaleX() == 1.0) {
			size_ = Small_;
		}
		if (player_->GetPlayerScaleX() == 1.5) {
			size_ = Medium_;
		}
		if (player_->GetPlayerScaleX() == 2.0) {
			size_ = Big_;
		}

		if (IsFullMapCamera == false) {
			// 追従カメラの更新
			followCamera_->Update();
			viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
			viewProjection_.matView = followCamera_->GetViewProjection().matView;
			viewProjection_.TransferMatrix();
		}

			// コントローラーのAボタンを押すとクリア
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
						scene = CLEAR;
					}
				}
			}


			// コントローラーのBボタンを押すとゲームオーバー
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
						scene = GAMEOVER;
					}
				}
			}
			break;
	case GameScene::CLEAR:
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					scene = TITLE;
				}
			}
		}
		break;
	case GameScene::GAMEOVER:
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					scene = TITLE;
				}
			}
		}
		break;
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

	
	if (scene == TITLE) {
		TitleSprite_->Draw();
	}
	if (scene == OPERATION) {
		OperationSprite_->Draw();
	}
	if (scene == CLEAR) {
		ClearSprite_->Draw();
	}
	if (scene == GAMEOVER) {
		GameOverSprite_->Draw();
	}
	
	

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

	if (scene == GAME) {

		// 自キャラの描画
		if (istutorial_ || isstage1_||isstage2_)
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
			//ゴール
			for (const std::unique_ptr<Goal>& goalW : TutorialGoalWhites_) {
				goalW->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Goal>& goalB : TutorialGoalBlacks_) {
				goalB->Draw(viewProjection_);
			}
		}

		if (isstage1_)
		{
			//ステージの描画
			for (const auto& stage1 : stages1_) {

				stage1->Draw(viewProjection_);

			}

		//炎の描画
		for (const auto& fire : fires_) {
			fire->Draw(viewProjection_);
		}


			//小スイッチの描画
			smallswitch_->Draw(viewProjection_);

			//中スイッチの描画
			normalswitch_->Draw(viewProjection_);

		//風のギミックの描画消す
		/*for (const auto& wind : winds_) {
			wind->Draw(viewProjection_);
		}*/
		//落とし穴の描画
		for (const auto& pitfall : pitfalls_) {
			pitfall->Draw(viewProjection_);
		}
		
		//回転矢印
		for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
			arrow->Draw(viewProjection_);
		}
		//ゴール
		for (const std::unique_ptr<Goal>& goalW : GoalWhites_) {
			goalW->Draw(viewProjection_);
		}
		for (const std::unique_ptr<Goal>& goalB : GoalBlacks_) {
			goalB->Draw(viewProjection_);
		}
	}

	if (isstage2_)
	{
		//ステージの描画
		for (const auto& stage2 : stages2_) {
			stage2->Draw(viewProjection_);
		}
		//炎の描画
		for (const auto& fire2 : fires2_) {
			fire2->Draw(viewProjection_);
		}
		//ステージ2の回復の描画
		if (stage2recovery_)
		{
			stage2recovery_->Draw(viewProjection_);
		}
		//回転矢印
		for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
			arrow->Draw(viewProjection_);
		}
		////ゴール
		//for (const std::unique_ptr<Goal>& goalW : GoalWhites_) {
		//	goalW->Draw(viewProjection_);
		//}
		//for (const std::unique_ptr<Goal>& goalB : GoalBlacks_) {
		//	goalB->Draw(viewProjection_);
		//}
	}
		if (isstage2_)
		{
			//ステージの描画
			for (const auto& stage2 : stages2_) {
				stage2->Draw(viewProjection_);
			}
			//炎の描画
			for (const auto& fire2 : fires2_) {
				fire2->Draw(viewProjection_);
			}
			//ステージ2の回復の描画
			if (stage2recovery_)
			{
				stage2recovery_->Draw(viewProjection_);
			}
			for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
				arrow->Draw(viewProjection_);
			}
			for (const auto& stage2barrier : stage2barriers_)
			{
				stage2barrier->Draw(viewProjection_);
			}
			stage2rotatingarrow_->Draw(viewProjection_);
			for (const auto& speeddown : speeddowns_)
			{
				speeddown->Draw(viewProjection_);
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
		if (isstage1_ || isstage2_)
		{
			//中スイッチの描画
			normalswitch_->Draw(viewProjection_);
		}
		//回復
		if (isstage1_&&recovery_ || isstage2_&&recovery_) {
			recovery_->Draw(viewProjection_);
		}

		if (isstage1_&&ball_||isstage2_&&ball_) {
			ball_->Draw(viewProjection_);
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
		//rotatingarrow_->Draw(viewProjection_);
		
		// 天球の描画
		skydome_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//大きさの描画
	//これで場所替え
	
	Vector2 position = { 670,240 };
	//position = { 605,200 };
	
	if (istutorial_ == true && scene == GAME || isstage1_ == true && scene == GAME || isstage2_ == true&&scene==GAME) {
		if (size_ == Big_) {
			BigSprite_->SetPosition(position);
			BigSprite_->Draw();
		}
		if (size_ == Medium_) {
			MediumSprite_->SetPosition(position);
			MediumSprite_->Draw();
		}
		if (size_ == Small_) {
			SmallSprite_->SetPosition(position);
			SmallSprite_->Draw();	
		}
	}

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

void GameScene::Stage2LoadWallPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage2wallPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage2wallPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::Stage2UpdateWallPopCommands()
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
	while (getline(stage2wallPopCommands, line2)) {
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
			Stage2WallGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage2WallGeneration(const Vector3& position)
{
	// 敵の生成
	Stage2* stage2 = new Stage2();



	stage2->Initialize(modelwall_.get(), position);
	stage2->SetGameScene(this);

	stages2_.push_back(static_cast<std::unique_ptr<Stage2>>(stage2));
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
	Fire* fire = new Fire();



	fire->Initialize(model_, position);
	fire->SetGameScene(this);

	fires_.push_back(static_cast<std::unique_ptr<Fire>>(fire));
}

void GameScene::LoadStage2FlamePopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage2FlamePop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage2flamePopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage2FlamePopCommands()
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
	while (getline(stage2flamePopCommands, line2)) {
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
			Stage2FlameGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage2FlameGeneration(const Vector3& position)
{
	// 敵の生成
	Fire2* fire2 = new Fire2();



	fire2->Initialize(model_, position);
	fire2->SetGameScene(this);

	fires2_.push_back(static_cast<std::unique_ptr<Fire2>>(fire2));
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

	
	
	barrier->Initialize(modelbarrier_.get(), position);
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



	barrier2->Initialize(modelbarrier_.get(), position);
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

void GameScene::BarrierRemoved()
{
	for (const std::unique_ptr<Barrier2>& barrier2 : barriers2_) {
		if (smallswitch_->IsScale())
		{
			barrier2->OnCollision();
		}
	}
	for (const std::unique_ptr<Barrier>& barrier : barriers_) {
		if (normalswitch_->IsScale())
		{
			barrier->OnCollision();
		}
	}
}

void GameScene::LoadArrowPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//ArrowPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	arrowPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateArrowPopCommands()
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
	while (getline(arrowPopCommands, line2)) {
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
			ArrowGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::ArrowGeneration(const Vector3& position)
{
	// 敵の生成
	RotatingArrow* arrow = new RotatingArrow();

	arrow->Initialize(modelArrow_.get(), position);
	arrow->SetGameScene(this);

	Arrows_.push_back(static_cast<std::unique_ptr<RotatingArrow>>(arrow));
}

//ゴールステージ１
void GameScene::LoadGoalWhitePopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//WhitePop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalWhitePopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalWhitePopCommands()
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
	while (getline(GoalWhitePopCommands, line2)) {
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
			GoalWhiteGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalWhiteGeneration(const Vector3& position)
{
	// 敵の生成
	Goal*  goal= new Goal();

	goal->Initialize(modelGoalWhite_.get(), position);
	goal->SetGameScene(this);

	GoalWhites_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

void GameScene::LoadGoalBlackPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//BlackPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalBlackPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalBlackPopCommands()
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
	while (getline(GoalBlackPopCommands, line2)) {
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
			GoalBlackGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalBlackGeneration(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalBlack_.get(), position);
	goal->SetGameScene(this);

	GoalBlacks_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

//ゴールチュートリアル
void GameScene::LoadTutorialGoalWhitePopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//TutorialWhitePop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	TutorialGoalWhitePopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateTutorialGoalWhitePopCommands()
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
	while (getline(TutorialGoalWhitePopCommands, line2)) {
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
			TutorialGoalWhiteGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::TutorialGoalWhiteGeneration(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalWhite_.get(), position);
	goal->SetGameScene(this);

	TutorialGoalWhites_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

void GameScene::LoadTutorialGoalBlackPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//TutorialBlackPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	TutorialGoalBlackPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateTutorialGoalBlackPopCommands()
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
	while (getline(TutorialGoalBlackPopCommands, line2)) {
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
			TutorialGoalBlackGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::TutorialGoalBlackGeneration(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalBlack_.get(), position);
	goal->SetGameScene(this);

	TutorialGoalBlacks_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}



//当たり判定
void GameScene::LoadStage2BarrierPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage2BarrierPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage2barrierPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage2BarrierPopCommands()
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
	while (getline(stage2barrierPopCommands, line2)) {
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
			Stage2BarrierGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage2BarrierGeneration(const Vector3& position)
{
	// 敵の生成
	Stage2Barrier* stage2barrier = new Stage2Barrier();

	stage2barrier->Initialize(model_, position);
	stage2barrier->SetGameScene(this);

	stage2barriers_.push_back(static_cast<std::unique_ptr<Stage2Barrier>>(stage2barrier));
}

void GameScene::LoadSpeedDownPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//SpeedDownPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	speedDownPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateSpeedDownPopCommands()
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
	while (getline(speedDownPopCommands, line2)) {
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
			SpeedDownGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::SpeedDownGeneration(const Vector3& position)
{
	// 敵の生成
	SpeedDown* speeddown = new SpeedDown();

	speeddown->Initialize(model_, position);
	speeddown->SetGameScene(this);

	speeddowns_.push_back(static_cast<std::unique_ptr<SpeedDown>>(speeddown));
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 PosA, PosB;
	Vector3 PosB2;
	Vector3 RadiusA, RadiusB;
	Vector3 RadiusB2;
	float PositionMeasure;
	float RadiusMeasure;
	float PositionMeasure2;
	float RadiusMeasure2;

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
			isballdead_ = true;
		}
	}
#pragma endregion

#pragma region プレイヤーと回復
	if (isstage1_&&recovery_||isstage2_&&recovery_) {
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
#pragma endregion

#pragma region プレイヤーとチュートリアルの壁
	for (const std::unique_ptr<Tutorial>& tutorial : tutorials_) {
		if (tutorial && istutorial_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//チュートリアルの座標
			PosB = tutorial->GetPosition();
			RadiusB = tutorial->GetScale();
			//右端
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision2();
			}
			//左端
			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision3();
			}
			//上
			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
			{
				player_->OnCollision4();
			}
			//下
			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
			{
				player_->OnCollision5();
			}

		}
	}
#pragma endregion

#pragma region プレイヤーと1つめのステージの壁
	for (const std::unique_ptr<Stage1>& stage1 : stages1_) {
		if (stage1 && isstage1_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//1つめのステージの座標
			PosB = stage1->GetPosition();
			RadiusB = stage1->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision2();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision3();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
			{
				player_->OnCollision4();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
			{
				player_->OnCollision5();
			}

		}
	}
#pragma endregion

#pragma region プレイヤーと1つめのバリア
	for (const std::unique_ptr<Barrier>& barrier : barriers_) {
		if (barrier && isstage1_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//1つめのバリアの座標
			PosB = barrier->GetPosition();
			RadiusB = barrier->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && barrier->IsDead() == false)
			{
				player_->OnCollision2();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && barrier->IsDead() == false)
			{
				player_->OnCollision3();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && barrier->IsDead() == false)
			{
				player_->OnCollision4();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && barrier->IsDead() == false)
			{
				player_->OnCollision5();
			}

		}
	}
#pragma endregion

#pragma region プレイヤーと2つめのバリア
	for (const std::unique_ptr<Barrier2>& barrier2 : barriers2_) {
		if (barrier2 && isstage1_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//2つめのバリアの座標
			PosB = barrier2->GetPosition();
			RadiusB = barrier2->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && barrier2->IsDead() == false)
			{
				player_->OnCollision2();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && barrier2->IsDead() == false)
			{
				player_->OnCollision3();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && barrier2->IsDead() == false)
			{
				player_->OnCollision4();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && barrier2->IsDead() == false)
			{
				player_->OnCollision5();
			}

		}
	}
#pragma endregion

#pragma region プレイヤーと小さいスイッチ
	
		if (isstage1_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//小さいスイッチの座標
			PosB = smallswitch_->GetPosition();
			RadiusB = smallswitch_->GetScale();
			//もしプレイヤーのサイズが中以上だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z&&player_->GetRadius().x>=1.5f)
			{
				player_->OnCollision2();
			
			}
			//もしプレイヤーのサイズが小だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x <= 1.0f)
			{
				player_->OnCollision2();
				smallswitch_->OnCollision();
			}

		if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
		{
			player_->OnCollision3();

		}

		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
		{
			player_->OnCollision4();
		}

		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
		{
			player_->OnCollision5();
		}

	}


#pragma endregion

#pragma region プレイヤーと普通のスイッチ

	if (isstage1_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//普通のスイッチの座標
		PosB = normalswitch_->GetPosition();
		RadiusB = normalswitch_->GetScale();
		if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x >= 2.0f)
			{
				player_->OnCollision2();

			}
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x <= 1.0f)
			{
				player_->OnCollision2();

			}
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x == 1.5f)
			{
				player_->OnCollision2();
				normalswitch_->OnCollision();
			}

		if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
		{
			player_->OnCollision3();
		}

		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
		{
			player_->OnCollision4();
		}

		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

			PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
		{
			player_->OnCollision5();
		}

	}


#pragma endregion

#pragma region プレイヤーと炎
	for (const std::unique_ptr<Fire>& fire : fires_) {
		if (fire && isstage1_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//炎の座標
			PosB = fire->GetPosition();
			RadiusB = fire->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision2();
				player_->OnCollision7();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision3();
				player_->OnCollision7();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
			{
				player_->OnCollision4();
				player_->OnCollision7();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
			{
				player_->OnCollision5();
				player_->OnCollision7();
			}

			if (player_->GetRadius().x <= 0.5f)
			{
				scene = GAMEOVER;
			}

		}
	}
#pragma endregion

#pragma region プレイヤーと1つ目のワープ

	if (isstage1_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//回復の座標
		PosB = warp_->GetPosition();
		RadiusB = warp_->GetScale();
		//2つめのワープの座標
		PosB2 = warp2_->GetPosition();
		RadiusB2 = warp2_->GetScale();

		// 座標AとBの距離を求める
		PositionMeasure2 = (PosB2.x - PosA.x) * (PosB2.x - PosA.x) +
			(PosB2.y - PosA.y) * (PosB2.y - PosA.y) +
			(PosB2.z - PosA.z) * (PosB2.z - PosA.z);
		RadiusMeasure2 = (float)(Dot(RadiusA, RadiusB2));
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// プレイヤーと1つめのワープの交差判定
		if (PositionMeasure <= RadiusMeasure && warpcooltime_ >= 10) {
			player_->WarpOnCollision();
			warpcooltime_ = 0;
			movestoptime = 0;
		}
		else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
			player_->MoveStop();
		}
		if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && warpcooltime_ <= 10) {
			warpcooltime_++;
		}


	}

#pragma endregion

#pragma region プレイヤーと2つ目のワープ

	if (isstage1_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//2つめのワープの座標
		PosB = warp2_->GetPosition();
		RadiusB = warp2_->GetScale();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// プレイヤーと2つめのワープの交差判定
		if (PositionMeasure <= RadiusMeasure && warpcooltime_ >= 10) {
			player_->WarpOnCollision2();
			warpcooltime_ = 0;
			movestoptime = 0;
		}
		else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
			player_->MoveStop();
		}


	}

#pragma endregion

#pragma region プレイヤーと落とし穴
	if (isstage1_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		//落とし穴の座標
		for (const std::unique_ptr<Pitfall>& pitfall : pitfalls_) {
			PosB = pitfall->GetPosition();
			RadiusB = pitfall->GetScale();
			// 座標AとBの距離を求める
			PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
				(PosB.y - PosA.y) * (PosB.y - PosA.y) +
				(PosB.z - PosA.z) * (PosB.z - PosA.z);
			RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
			// プレイヤーと落とし穴の交差判定
			if (PositionMeasure <= RadiusMeasure) {
				scene = GAMEOVER;
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと回る矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
		PosB = arrow->GetWorldPosition();
		RadiusB = arrow->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			arrow->OnCollision(player_.get());
		}
	}

#pragma endregion

#pragma region プレイヤーと右矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	PosB = rightarrow_->GetWorldPosition();
	RadiusB = rightarrow_->GetRadius();
	// 座標AとBの距離を求める
	PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
		(PosB.y - PosA.y) * (PosB.y - PosA.y) +
		(PosB.z - PosA.z) * (PosB.z - PosA.z);
	RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
	// 弾と弾の交差判定
	if (PositionMeasure <= RadiusMeasure) {
		rightarrow_->OnCollision(player_.get());
	}

#pragma endregion

#pragma region プレイヤーと左矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	PosB = leftarrow_->GetWorldPosition();
	RadiusB = leftarrow_->GetRadius();
	// 座標AとBの距離を求める
	PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
		(PosB.y - PosA.y) * (PosB.y - PosA.y) +
		(PosB.z - PosA.z) * (PosB.z - PosA.z);
	RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
	// 弾と弾の交差判定
	if (PositionMeasure <= RadiusMeasure) {
		leftarrow_->OnCollision(player_.get());
	}

#pragma endregion

#pragma region プレイヤーと上矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	PosB = uparrow_->GetWorldPosition();
	RadiusB = uparrow_->GetRadius();
	// 座標AとBの距離を求める
	PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
		(PosB.y - PosA.y) * (PosB.y - PosA.y) +
		(PosB.z - PosA.z) * (PosB.z - PosA.z);
	RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
	// 弾と弾の交差判定
	if (PositionMeasure <= RadiusMeasure) {
		uparrow_->OnCollision(player_.get());
	}

#pragma endregion

#pragma region プレイヤーと下矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	PosB = downarrow_->GetWorldPosition();
	RadiusB = downarrow_->GetRadius();
	// 座標AとBの距離を求める
	PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
		(PosB.y - PosA.y) * (PosB.y - PosA.y) +
		(PosB.z - PosA.z) * (PosB.z - PosA.z);
	RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
	// 弾と弾の交差判定
	if (PositionMeasure <= RadiusMeasure) {
		downarrow_->OnCollision(player_.get());
	}

#pragma endregion

#pragma region プレイヤーとゴール白
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalWhites_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr&&isstage1_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール黒
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalBlacks_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr&&isstage1_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール白
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : TutorialGoalWhites_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure &&istutorial_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール黒
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : TutorialGoalBlacks_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure &&istutorial_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとスピードダウン
	for (const std::unique_ptr<SpeedDown>& speeddown : speeddowns_) {
		// プレイヤーの座標
		PosA = player_->GetWorldPosition();
		RadiusA = player_->GetRadius();
		PosB = speeddown->GetPosition();
		RadiusB = speeddown->GetScale();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure) {
			player_->SpeedDownOnCollision();
		}
	}

#pragma endregion
}