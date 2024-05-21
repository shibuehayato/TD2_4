#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "AxisIndicator.h"
#include <ImGuiManager.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	for (WindParticle* wind :windParticles_) {
		delete wind;
	}
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
	ClearTexture_ = TextureManager::Load("scene/GameClear.png");
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
	ball_->SetGameScene(this);
	//回復の生成
	recovery_ = std::make_unique<Recovery>();
	//3Dモデルの生成
	modelRecovery_.reset(Model::CreateFromOBJ("life", true));
	//回復の初期化
	recovery_->Initialize(modelRecovery_.get());
	recovery_->SetGameScene(this);
	recoveryTime_ = 0;

	//風のパーティクルの生成
	//windParticles_ = std::make_unique<WindParticle>();
	//3Dモデルの生成
	modelWind_.reset(Model::CreateFromOBJ("WinPar", true));

	//風ファンの生成
	cyclone_ = std::make_unique<Cyclone>();
	//3Dモデルの生成
	modelCyclone_.reset(Model::CreateFromOBJ("Cyclone", true));
	cyclone_->Initialize(modelCyclone_.get());

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
	LoadGoalWhite2PopData();
	LoadGoalBlack2PopData();
	LoadGoalWhite3PopData();
	LoadGoalBlack3PopData();
	LoadTutorialGoalWhitePopData();
	LoadTutorialGoalBlackPopData();

	Stage2LoadWallPopData();
	LoadStage2FlamePopData();
	LoadStage2BarrierPopData();
	LoadSpeedDownPopData();
	LoadCannonPopData();
	LoadStage3WallPopData();
	LoadStage3BarrierPopData();
	LoadStage3Barrier2ndPopData();
	LoadStage3Barrier3rdPopData();
	LoadStage3Barrier4thPopData();
	LoadStage3FirePopData();
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

	//// 軸方向表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

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
	
	//ステージ2の回復
	stage2recovery_ = std::make_unique<Stage2Recovery>();
	stage2recovery_->Initialize(modelRecovery_.get());
	stage2recovery_->SetGameScene(this);

	//ステージ2の回転矢印
	stage2rotatingarrow_ = std::make_unique<Stage2RotatingArrow>();
	stage2rotatingarrow_->Initialize(modelArrow_.get());

	//回転大砲
	rotatecannon_ = std::make_unique<RotateCannon>();
	rotatecannon_->Initialize(model_, model_);
	rotatecannon_->SetGameScene(this);

	
	
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//isstage1_ = true;

	modelpitfall_.reset(Model::CreateFromOBJ("Pitfall", true));

	stage3warp_ = std::make_unique<Stage3Warp>();
	stage3warp_->Initialize(modelwarp_.get());

	stage3warp2_ = std::make_unique<Stage3Warp2>();
	stage3warp2_->Initialize(modelwarp_.get());

	stage3warp2nd_ = std::make_unique<Stage3Warp2nd>();
	stage3warp2nd_->Initialize(modelwarp_.get());

	stage3warp2nd2_ = std::make_unique<Stage3Warp2nd2>();
	stage3warp2nd2_->Initialize(modelwarp_.get());

	stage3warp3rd_ = std::make_unique<Stage3Warp3rd>();
	stage3warp3rd_->Initialize(modelwarp_.get());

	stage3warp3rd2_ = std::make_unique<Stage3Warp3rd2>();
	stage3warp3rd2_->Initialize(modelwarp_.get());

	stage3warp4th_ = std::make_unique<Stage3Warp4th>();
	stage3warp4th_->Initialize(modelwarp_.get());

	stage3warp4th2_ = std::make_unique<Stage3Warp4th2>();
	stage3warp4th2_->Initialize(modelwarp_.get());

	stage3warp5th_ = std::make_unique<Stage3Warp5th>();
	stage3warp5th_->Initialize(modelwarp_.get());

	stage3warp5th2_ = std::make_unique<Stage3Warp5th2>();
	stage3warp5th2_->Initialize(modelwarp_.get());

	bigswitch_ = std::make_unique<BigSwitch>();
	modelbigswitch_.reset(Model::CreateFromOBJ("switch_normal", true));
	modelbigbutton_.reset(Model::CreateFromOBJ("switch_push", true));
	bigswitch_->Initialize(modelbigswitch_.get(), modelbigbutton_.get());

	bigswitch2_ = std::make_unique<BigSwitch2>();
	modelbigbutton2_.reset(Model::CreateFromOBJ("switch_push", true));
	bigswitch2_->Initialize(modelbigswitch_.get(), modelbigbutton2_.get());

	stage3speeddown_ = std::make_unique<Stage3SpeedDown>();
	modelspeeddown_.reset(Model::CreateFromOBJ("ArrowDown", true));
	stage3speeddown_->Initialize(modelspeeddown_.get());

	speedup_ = std::make_unique<SpeedUp>();
	modelspeedup_.reset(Model::CreateFromOBJ("ArrowUp", true));
	speedup_->Initialize(modelspeedup_.get());

	//ステージ3の回転大砲
	stage3rotatecannon_ = std::make_unique<Stage3RotateConnon>();
	stage3rotatecannon_->Initialize(model_, model_);
	stage3rotatecannon_->SetGameScene(this);

	stageselect_ = std::make_unique<StageSelect>();
	stageselect_->Initialize();
	uint32_t StageSelecttexture_ = TextureManager::Load("choice.png");

	StageSelectsprite_ = std::make_unique<Sprite>();
	StageSelectsprite_.reset(Sprite::Create(StageSelecttexture_, {1280,720}, {1.0f,1.0f,1.0f,1.0f}, {1.0f,1.0f}));

	

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
					isblackout = true;
				}
			}
		}
		if (isblackout)
		{

		}
		break;
	case GameScene::OPERATION: // 操作説明シーン
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					GameReset();
					stageselect_->Reset();
					scene = GAME;
					
				}
			}
		}
		break;
	case GameScene::GAME:

		debugCamera_->Update();
#ifdef _DEBUG
		/*ImGui::Begin("viewprojection");
		ImGui::DragFloat3("translation", &viewProjection_.translation_.x);
		ImGui::DragFloat3("rotation", &viewProjection_.rotation_.x);
		ImGui::DragInt("rotation", &warpcooltime_);
		ImGui::Checkbox("isstage2", &isstage2_);
		ImGui::End();*/
#endif

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

	if (stageselect_->IsTutorial() || stageselect_->IsStage1() || stageselect_->IsStage2() || stageselect_->IsStage3())
	{
		isselect_ = false;
	}
	else
	{
		isselect_ = true;
	}



		// 天球の更新
		skydome_->Update();

		//チュートリアルのフラグを立てるためのif文
		if (Input::GetInstance()->GetJoystickState(0, prejoyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
				if (stageselect_->IsTutorial() &&stageselect_->IsCleck())
				{


					player_->SetPlayerPosition2();

					for (Cannonbullet* cannonbullet : cannonbullets_)
					{
						cannonbullet->OnCollision();
					}
					for (RotateCannonBullet* rotatecannonbullet : rotatecannonbullets_)
					{
						rotatecannonbullet->OnCollision();
					}
					for (Stage3RotateCannonBullet* stage3rotatecannonbullet : stage3rotatecannonbullets_)
					{
						stage3rotatecannonbullet->OnCollision();
					}

				}
				//ステージ1のフラグを立てるためのif文
				else if (stageselect_->IsStage1() && stageselect_->IsCleck())
				{


					player_->SetPlayerPosition2();
					for (Cannonbullet* cannonbullet : cannonbullets_)
					{
						cannonbullet->OnCollision();
					}
					for (RotateCannonBullet* rotatecannonbullet : rotatecannonbullets_)
					{
						rotatecannonbullet->OnCollision();
					}
					for (Stage3RotateCannonBullet* stage3rotatecannonbullet : stage3rotatecannonbullets_)
					{
						stage3rotatecannonbullet->OnCollision();
					}
				}
				else if (stageselect_->IsStage2() && stageselect_->IsCleck())
				{

					player_->SetPlayerPosition2();
				}
				else if (stageselect_->IsStage3() && stageselect_->IsCleck())
				{

					player_->SetPlayerPosition();
				}
			}
		}

		if (stageselect_->IsTutorial())
		{
			istutorial_ = true;
			isstage1_ = false;
			isstage2_ = false;
			isstage3_ = false;
		}
		else if (stageselect_->IsStage1())
		{
			istutorial_ = false;
			isstage1_ = true;
			isstage2_ = false;
			isstage3_ = false;
		}
		else if (stageselect_->IsStage2())
		{
			istutorial_ = false;
			isstage2_ = true;
			isstage1_ = false;
			isstage3_ = false;
		}
		else if (stageselect_->IsStage3())
		{
			istutorial_ = false;
			isstage3_ = true;
			isstage1_ = false;
			isstage2_ = false;
		}

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)&&stageselect_->IsTutorial()) {
					player_->SetPlayerPosition2();
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && stageselect_->IsStage1())
				{
					player_->SetPlayerPosition2();
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && stageselect_->IsStage2())
				{
					player_->SetPlayerPosition2();
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && stageselect_->IsStage3())
				{
					player_->SetPlayerPosition();
				}
			}
		}
		if (istutorial_ || isstage1_ || isstage2_ || isstage3_)
		{
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
						if (IsFullMapCamera == false) {
							IsFullMapCamera = true;
							fullmapcameracooltimer_ = 0;
						}
					}
				}
			}

			if (Input::GetInstance()->GetJoystickState(0, prejoyState)) {
				if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
					if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
						!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
						if (IsFullMapCamera == true && fullmapcameracooltimer_ >= 10) {
							IsFullMapCamera = false;
						}
					}
				}
			}

			if (IsFullMapCamera == true&&player_->GetWorldPosition().z<=20.0f&& player_->GetWorldPosition().z >= 0.1f) {
				viewProjection_.translation_ = { 0,130.0f,0 };
				viewProjection_.rotation_ = { -11.0f,0,0 };
			}
			else if(IsFullMapCamera == true && player_->GetWorldPosition().z >= 20.1f) {
				viewProjection_.translation_ = { 0,130.0f,20 };
				viewProjection_.rotation_ = { -11.0f,0,0 };
			}
			else if (IsFullMapCamera == true && player_->GetWorldPosition().z <= 0.0f) {
				viewProjection_.translation_ = { 0,130.0f,-20 };
				viewProjection_.rotation_ = { -11.0f,0,0 };
			}
			if (IsFullMapCamera == true && fullmapcameracooltimer_ <= 10)
			{
				fullmapcameracooltimer_++;
			}
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
			

		}

		/*ImGui::Begin("FullMap");
		ImGui::Checkbox("FullMap", &IsFullMapCamera);
		ImGui::End();*/

		
		stageselect_->Update();
		

		if (isstage1_ || isstage2_ || isstage3_)
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
			
				//風のギミックの更新
				for (const std::unique_ptr<Wind>& wind : winds_) {
					wind->Update();
				}
				UpdateWindPopCommands();

			//風のパーティクル
			for (WindParticle* wind : windParticles_) {
				wind->Update();
			}
			UpdateWindParticlePopCommands();
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
			//風ファンの更新
			cyclone_->Update();
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
		
		for (const std::unique_ptr<SpeedDown>& speeddown : speeddowns_)
		{
			speeddown->Update();
		}
		UpdateSpeedDownPopCommands();
		for (const std::unique_ptr<Cannon>& cannon : cannons_)
		{
			cannon->Update();
		}
		UpdateCannonPopCommands();

		

	}

	if (isstage3_)
	{
		for (const std::unique_ptr<Stage3Wall>& stage3wall : stage3walls_)
		{
			stage3wall->Update();
		}
		UpdateStage3WallPopCommands();
		for (const std::unique_ptr<Stage3Barrier>& stage3barrier : stage3barriers_)
		{
			stage3barrier->Update();
		}
		UpdateStage3BarrierPopCommands();
		for (const std::unique_ptr<Stage3Barrier2nd>& stage3barrier2nd : stage3barrier2nds_)
		{
			stage3barrier2nd->Update();
		}
		UpdateStage3Barrier2ndPopCommands();
		for (const std::unique_ptr<Stage3Barrier3rd>& stage3barrier3rd : stage3barrier3rds_)
		{
			stage3barrier3rd->Update();
		}
		UpdateStage3Barrier3rdPopCommands();
		for (const std::unique_ptr<Stage3Barrier4th>& stage3barrier4th : stage3barrier4ths_)
		{
			stage3barrier4th->Update();
		}
		UpdateStage3Barrier4thPopCommands();
		//ゴール3
		for (const std::unique_ptr<Goal>& goalW : GoalWhites3_) {
			goalW->Update();
		}
		UpdateGoalWhite3PopCommands();
		for (const std::unique_ptr<Goal>& goalB : GoalBlacks3_) {
			goalB->Update();
		}
		UpdateGoalBlack3PopCommands();

		//ステージ3でのワープ
		stage3warp_->Update();
		stage3warp2_->Update();
		stage3warp2nd_->Update();
		stage3warp2nd2_->Update();
		stage3warp3rd_->Update();
		stage3warp3rd2_->Update();
		stage3warp4th_->Update();
		stage3warp4th2_->Update();
		stage3warp5th_->Update();
		stage3warp5th2_->Update();

		for (const std::unique_ptr<Stage3Fire>& stage3fire : stage3fires_)
		{
			stage3fire->Update();
		}
		UpdateStage3FirePopCommands();

		bigswitch_->Update();
		bigswitch2_->Update();
		stage3speeddown_->Update();
		speedup_->Update();
	}
	if (isstage2_ )
	{
		//ゴール
		for (const std::unique_ptr<Goal>& goalW : GoalWhites2_) {
			goalW->Update();
		}
		UpdateGoalWhite2PopCommands();
		for (const std::unique_ptr<Goal>& goalB : GoalBlacks2_) {
			goalB->Update();
		}
		UpdateGoalBlack2PopCommands();

		rotatecannon_->Update();
		stage2rotatingarrow_->Update();
	}

	if (isstage3_)
	{
		stage3rotatecannon_->Update();
	}

	if (isstage1_  || isstage3_)
	{
		//小スイッチの更新
		smallswitch_->Update();
	}
		//回復
		if (isstage1_ && recovery_ || recovery_ && isstage2_ || recovery_ && isstage3_) {
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
		if (isstage1_ && ball_ || isstage2_ && ball_ || isstage3_ && ball_) {
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
	for (Cannonbullet* bullet : cannonbullets_)
	{
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	cannonbullets_.remove_if([](Cannonbullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});


	

	// デスフラグの立った弾を削除
	rotatecannonbullets_.remove_if([](RotateCannonBullet* rotatebullet) {
		if (rotatebullet->IsDead()) {
			delete rotatebullet;
			return true;
		}
		return false;
		});

	
	stage3rotatecannonbullets_.remove_if([](Stage3RotateCannonBullet* stage3rotatebullet) {
		if (stage3rotatebullet->IsDead()) {
			delete stage3rotatebullet;
			return true;
		}
		return false;
		});
	//-------------------------------//

	////玉をとって位置に来たらクリア
	//if (player_->GetTransformZ() >= 71&&ball_==nullptr) {
	//		scene = CLEAR;
	//		player_->Initialize(modelPlayerHead_.get());
	//}
	

		if (IsFullMapCamera == false) {
			// 追従カメラの更新
			followCamera_->Update();
			viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
			viewProjection_.matView = followCamera_->GetViewProjection().matView;
			viewProjection_.TransferMatrix();
		}

			//// コントローラーのAボタンを押すとクリア
			//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			//	if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
			//		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
			//			!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			//			scene = CLEAR;
			//		}
			//	}
			//}


			//// コントローラーのBボタンを押すとゲームオーバー
			//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			//	if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState)) {
			//		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
			//			!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			//			scene = GAMEOVER;
			//		}
			//	}
			//}
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
		for (RotateCannonBullet* rotatebullet : rotatecannonbullets_)
		{
			rotatebullet->Update();
		}
		for (Stage3RotateCannonBullet* stage3rotatebullet : stage3rotatecannonbullets_)
		{
			stage3rotatebullet->Update();
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
		if (istutorial_ || isstage1_||isstage2_||isstage3_)
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
			for (WindParticle* wind : windParticles_) {
				wind->Draw(viewProjection_);
			}
		
		
		
		//風ファンの描画
		cyclone_->Draw(viewProjection_);
			//風のギミックの描画消す
			/*for (const auto& wind : winds_) {
				wind->Draw(viewProjection_);
			}*/
			//落とし穴の描画
			for (const auto& pitfall : pitfalls_) {
				pitfall->Draw(viewProjection_);
			}
			if (isstage1_)
			{
				//回転矢印
				for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
					arrow->Draw(viewProjection_);
				}
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
		if (isstage1_)
		{
			//回転矢印
			for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
				arrow->Draw(viewProjection_);
			}
		}
		//ゴール
		for (const std::unique_ptr<Goal>& goalW : GoalWhites2_) {
			goalW->Draw(viewProjection_);
		}
		for (const std::unique_ptr<Goal>& goalB : GoalBlacks2_) {
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
			if (isstage1_)
			{
				for (const std::unique_ptr<RotatingArrow>& arrow : Arrows_) {
					arrow->Draw(viewProjection_);
				}
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
			for (const auto& cannon : cannons_)
			{
				cannon->Draw(viewProjection_);
			}
			
			for (Cannonbullet* bullet : cannonbullets_)
			{
				bullet->Draw(viewProjection_);
			}

			
		}
		if (isstage1_)
		{
			//バリアの描画
			for (const auto& barrier : barriers_) {
				barrier->Draw(viewProjection_);
			}

			//2つめのバリアの描画
			for (const auto& barrier2 : barriers2_) {
				barrier2->Draw(viewProjection_);
			}

		}
		if (isstage3_)
		{
			for (const std::unique_ptr<Stage3Wall>& stage3wall : stage3walls_)
			{
				stage3wall->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Stage3Barrier>& stage3barrier : stage3barriers_)
			{
				stage3barrier->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Stage3Barrier2nd>& stage3barrier2nd : stage3barrier2nds_)
			{
				stage3barrier2nd->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Stage3Barrier3rd>& stage3barrier3rd : stage3barrier3rds_)
			{
				stage3barrier3rd->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Stage3Barrier4th>& stage3barrier4th : stage3barrier4ths_)
			{
				stage3barrier4th->Draw(viewProjection_);
			}
			//ゴール
			for (const std::unique_ptr<Goal>& goalW : GoalWhites3_) {
				goalW->Draw(viewProjection_);
			}
			for (const std::unique_ptr<Goal>& goalB : GoalBlacks3_) {
				goalB->Draw(viewProjection_);
			}

			//ステージ3でのワープ
			stage3warp_->Draw(viewProjection_);
			stage3warp2_->Draw(viewProjection_);
			stage3warp2nd_->Draw(viewProjection_);
			stage3warp2nd2_->Draw(viewProjection_);
			stage3warp3rd_->Draw(viewProjection_);
			stage3warp3rd2_->Draw(viewProjection_);
			stage3warp4th_->Draw(viewProjection_);
			stage3warp4th2_->Draw(viewProjection_);
			stage3warp5th_->Draw(viewProjection_);
			stage3warp5th2_->Draw(viewProjection_);

			for (const std::unique_ptr<Stage3Fire>& stage3fire : stage3fires_)
			{
				stage3fire->Draw(viewProjection_);
			}
			bigswitch_->Draw(viewProjection_);
			bigswitch2_->Draw(viewProjection_);
			stage3speeddown_->Draw(viewProjection_);
			speedup_->Draw(viewProjection_);
		}
		if (isstage2_ )
		{
			rotatecannon_->Draw(viewProjection_);
			for (RotateCannonBullet* rotatebullet : rotatecannonbullets_)
			{
				rotatebullet->Draw(viewProjection_);
			}
		}
		if (isstage3_)
		{
			stage3rotatecannon_->Draw(viewProjection_);
			for (Stage3RotateCannonBullet* stage3rotatebullet : stage3rotatecannonbullets_)
			{
				stage3rotatebullet->Draw(viewProjection_);
			}
		}
		if (isstage1_ || isstage2_ || isstage3_)
		{
			//中スイッチの描画
			normalswitch_->Draw(viewProjection_);
		}
		if (isstage1_  || isstage3_)
		{
			//中スイッチの描画
			smallswitch_->Draw(viewProjection_);
		}
		//回復
		if (isstage1_&&recovery_ || isstage2_&&recovery_ || isstage3_ && recovery_) {
			recovery_->Draw(viewProjection_);
		}

		if (isstage1_&&ball_||isstage2_&&ball_ || isstage3_ && ball_) {
			ball_->Draw(viewProjection_);
		}
		if (isstage1_)
		{
			//ワープの描画
			warp_->Draw(viewProjection_);
			//2つめのワープの描画
			warp2_->Draw(viewProjection_);

			//右矢印の描画
			rightarrow_->Draw(viewProjection_);
			//左矢印の描画
			//leftarrow_->Draw(viewProjection_);
			//上矢印の描画
			//uparrow_->Draw(viewProjection_);
			//下矢印の描画
			downarrow_->Draw(viewProjection_);
		}
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
	if (isselect_ == true&&scene==GAME)
	{
		StageSelectsprite_->Draw();
		stageselect_->Draw();
	}
	if (istutorial_ == true && scene == GAME && isselect_ == false &&
		IsFullMapCamera == false || isstage1_ == true && scene == GAME && 
		isselect_ == false && IsFullMapCamera == false || isstage2_ == true&&scene==GAME&&
		isselect_==false&&IsFullMapCamera==false || isstage3_ == true && scene == GAME && 
		isselect_ == false && IsFullMapCamera == false) {
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



	pitfall->Initialize(modelpitfall_.get(), position);
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
	for (const std::unique_ptr<Stage2Barrier>& stage2barrier : stage2barriers_) {
		if (normalswitch_->IsScale() && isstage2_)
		{
			stage2barrier->OnCollision();
		}
	}
	for (const std::unique_ptr<Stage3Barrier>& stage3barrier : stage3barriers_) {
		if (smallswitch_->IsScale() && isstage3_)
		{
			stage3barrier->OnCollision();
		}
	}
	for (const std::unique_ptr<Stage3Barrier2nd>& stage3barrier2nd : stage3barrier2nds_) {
		if (normalswitch_->IsScale() && isstage3_)
		{
			stage3barrier2nd->OnCollision();
		}
	}
	for (const std::unique_ptr<Stage3Barrier3rd>& stage3barrier3rd : stage3barrier3rds_) {
		if (bigswitch_->IsScale() && isstage3_)
		{
			stage3barrier3rd->OnCollision();
		}
	}
	for (const std::unique_ptr<Stage3Barrier4th>& stage3barrier4th : stage3barrier4ths_) {
		if (bigswitch2_->IsScale() && isstage3_)
		{
			stage3barrier4th->OnCollision();
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

void GameScene::UpdateWindParticlePopCommands()
{
		WindParticleInitilize();

		windParticles_.remove_if([](WindParticle* wind) {
			if (wind->IsDead()) {
				delete wind;
				return true;
			}
			return false;
			});

}
void GameScene::WindParticleInitilize() {
	// ランダムな初期位置を生成する
	float startX, startY,startZ;
	WindParticleStartPosition(startX, startY, startZ);


	windtime_ -= 0.1f;

	if (windtime_ <= 0) {
	// WindParticle オブジェクトを生成し、初期位置を渡す
	WindParticle* wind = new WindParticle();
	windParticles_.push_back(wind);
	wind->Initialize(modelWind_.get(), startX, startY, startZ);

	windtime_ = 3;
	}
	
}

void GameScene::WindParticleStartPosition(float& startX, float& startY, float& startZ)
{
	
	// X座標とY座標を-10から10の範囲でランダムに生成する
	startX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * rangestart.x * 2.0f - rangeend.x;
	startY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * rangestart.y * 2.0f - rangeend.y;
	startZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * rangestart.z * 2.0f - rangeend.z;
	
	/*ImGui::Begin("WinPar");
	ImGui::DragFloat3("Start", &rangestart.x,0.1f);
	ImGui::DragFloat3("end", &rangeend.x,0.1f);;
	ImGui::End();*/
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

//ゴールステージ2
void GameScene::LoadGoalWhite2PopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//White2Pop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalWhite2PopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalWhite2PopCommands()
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
	while (getline(GoalWhite2PopCommands, line2)) {
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
			GoalWhite2Generation(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalWhite2Generation(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalWhite_.get(), position);
	goal->SetGameScene(this);

	GoalWhites2_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

void GameScene::LoadGoalBlack2PopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Black2Pop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalBlack2PopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalBlack2PopCommands()
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
	while (getline(GoalBlack2PopCommands, line2)) {
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
			GoalBlack2Generation(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalBlack2Generation(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalBlack_.get(), position);
	goal->SetGameScene(this);

	GoalBlacks2_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

//ゴールステージ3
void GameScene::LoadGoalWhite3PopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//White3Pop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalWhite3PopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalWhite3PopCommands()
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
	while (getline(GoalWhite3PopCommands, line2)) {
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
			GoalWhite3Generation(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalWhite3Generation(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalWhite_.get(), position);
	goal->SetGameScene(this);

	GoalWhites3_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
}

void GameScene::LoadGoalBlack3PopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Black3Pop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	GoalBlack3PopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateGoalBlack3PopCommands()
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
	while (getline(GoalBlack3PopCommands, line2)) {
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
			GoalBlack3Generation(Vector3(x, y, z));
		}
	}
}

void GameScene::GoalBlack3Generation(const Vector3& position)
{
	// 敵の生成
	Goal* goal = new Goal();

	goal->Initialize(modelGoalBlack_.get(), position);
	goal->SetGameScene(this);

	GoalBlacks3_.push_back(static_cast<std::unique_ptr<Goal>>(goal));
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

void GameScene::LoadCannonPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//CannonPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	CannonPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateCannonPopCommands()
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
	while (getline(CannonPopCommands, line2)) {
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
		// ROTATIONコマンド
		if (word2.find("ROT") == 0) {
			// x座標
			getline(line_stream, word2, ',');
			rotation_.x = (float)std::atof(word2.c_str());

			// y座標
			getline(line_stream, word2, ',');
			rotation_.y = (float)std::atof(word2.c_str());

			// z座標
			getline(line_stream, word2, ',');
			rotation_.z = (float)std::atof(word2.c_str());
		}

		// ROTATIONコマンド
		if (word2.find("POP2") == 0) {
			// x座標
			getline(line_stream, word2, ',');
			position_.x = (float)std::atof(word2.c_str());

			// y座標
			getline(line_stream, word2, ',');
			position_.y = (float)std::atof(word2.c_str());

			// z座標
			getline(line_stream, word2, ',');
			position_.z = (float)std::atof(word2.c_str());
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
			CannonGenerate(Vector3(x, y, z),position_,rotation_);
		}
	}
}

void GameScene::CannonGenerate(const Vector3& position,const Vector3& Headposition,const Vector3& rotation)
{
	// 敵の生成
	Cannon* cannon = new Cannon();

	cannon->Initialize(model_, model_,position,Headposition,rotation);
	cannon->SetGameScene(this);

	cannons_.push_back(static_cast<std::unique_ptr<Cannon>>(cannon));
}

void GameScene::LoadStage3WallPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3WallPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3wallPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3WallPopCommands()
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
	while (getline(stage3wallPopCommands, line2)) {
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
			Stage3WallGenerate(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3WallGenerate(const Vector3& position)
{
	// 敵の生成
	Stage3Wall* stage3wall = new Stage3Wall();



	stage3wall->Initialize(modelwall_.get(), position);
	stage3wall->SetGameScene(this);

	stage3walls_.push_back(static_cast<std::unique_ptr<Stage3Wall>>(stage3wall));
}

void GameScene::LoadStage3BarrierPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3BarrierPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3barrierPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3BarrierPopCommands()
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
	while (getline(stage3barrierPopCommands, line2)) {
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
			Stage3BarrierGenerate(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3BarrierGenerate(const Vector3& position)
{
	// 敵の生成
	Stage3Barrier* stage3barrier = new Stage3Barrier();



	stage3barrier->Initialize(modelbarrier_.get(), position);
	stage3barrier->SetGameScene(this);

	stage3barriers_.push_back(static_cast<std::unique_ptr<Stage3Barrier>>(stage3barrier));
}

void GameScene::LoadStage3Barrier2ndPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3Barrier2ndPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3barrier2ndPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3Barrier2ndPopCommands()
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
	while (getline(stage3barrier2ndPopCommands, line2)) {
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
			Stage3Barrier2ndGenerate(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3Barrier2ndGenerate(const Vector3& position)
{
	// 敵の生成
	Stage3Barrier2nd* stage3barrier2nd = new Stage3Barrier2nd();



	stage3barrier2nd->Initialize(modelbarrier_.get(), position);
	stage3barrier2nd->SetGameScene(this);

	stage3barrier2nds_.push_back(static_cast<std::unique_ptr<Stage3Barrier2nd>>(stage3barrier2nd));
}

void GameScene::LoadStage3Barrier3rdPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3Barrier3rdPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3barrier3rdPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3Barrier3rdPopCommands()
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
	while (getline(stage3barrier3rdPopCommands, line2)) {
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
			Stage3Barrier3rdGenerate(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3Barrier3rdGenerate(const Vector3& position)
{
	// 敵の生成
	Stage3Barrier3rd* stage3barrier3rd = new Stage3Barrier3rd();



	stage3barrier3rd->Initialize(modelbarrier_.get(), position);
	stage3barrier3rd->SetGameScene(this);

	stage3barrier3rds_.push_back(static_cast<std::unique_ptr<Stage3Barrier3rd>>(stage3barrier3rd));
}

void GameScene::LoadStage3Barrier4thPopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3Barrier4thPop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3barrier4thPopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3Barrier4thPopCommands()
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
	while (getline(stage3barrier4thPopCommands, line2)) {
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
			Stage3Barrier4thGenerate(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3Barrier4thGenerate(const Vector3& position)
{
	// 敵の生成
	Stage3Barrier4th* stage3barrier4th = new Stage3Barrier4th();



	stage3barrier4th->Initialize(modelbarrier_.get(), position);
	stage3barrier4th->SetGameScene(this);

	stage3barrier4ths_.push_back(static_cast<std::unique_ptr<Stage3Barrier4th>>(stage3barrier4th));
}

void GameScene::LoadStage3FirePopData()
{
	// ファイルを開く
	std::ifstream file2;
	std::string filename = "Resources//Stage3FirePop.csv";
	file2.open(filename);
	assert(file2.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stage3firePopCommands << file2.rdbuf();


	// ファイルを閉じる
	file2.close();
}

void GameScene::UpdateStage3FirePopCommands()
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
	while (getline(stage3firePopCommands, line2)) {
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
			Stage3FireGeneration(Vector3(x, y, z));
		}
	}
}

void GameScene::Stage3FireGeneration(const Vector3& position)
{
	// 敵の生成
	Stage3Fire* stage3fire = new Stage3Fire();



	stage3fire->Initialize(model_, position);
	stage3fire->SetGameScene(this);

	stage3fires_.push_back(static_cast<std::unique_ptr<Stage3Fire>>(stage3fire));
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

	stage2barrier->Initialize(modelbarrier_.get(), position);
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

	speeddown->Initialize(modelspeeddown_.get(), position);
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
	if (isstage1_&&recovery_||isstage2_&&recovery_ || isstage3_ && recovery_) {
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
		if (PositionMeasure <= RadiusMeasure&&isstage1_) {
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
			if (PosA.x <= -19.1f)
			{
				player_->SetPlayerPosition4();
			}
			else if (PosA.x >= 19.1f)
			{
				player_->SetPlayerPosition3();
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
			if (PosA.x <= -19.1f)
			{
				player_->SetPlayerPosition4();
			}
			else if (PosA.x >= 19.1f)
			{
				player_->SetPlayerPosition3();
			}
		}
	}
#pragma endregion

#pragma region プレイヤーと2つめのステージの壁
	for (const std::unique_ptr<Stage2>& stage2 : stages2_) {
		if (stage2 && isstage2_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//1つめのステージの座標
			PosB = stage2->GetPosition();
			RadiusB = stage2->GetScale();
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
			if (PosA.x <= -19.1f)
			{
				player_->SetPlayerPosition4();
			}
			else if (PosA.x >= 19.1f)
			{
				player_->SetPlayerPosition3();
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
	
		if (isstage1_ || isstage3_) {
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

	if (isstage1_ || isstage2_ || isstage3_) {
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
		if (PositionMeasure <= RadiusMeasure&&isstage1_) {
			arrow->OnCollision(player_.get());
		}
	}

#pragma endregion

#pragma region プレイヤーと回る矢印
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
		PosB = stage2rotatingarrow_->GetWorldPosition();
		RadiusB = stage2rotatingarrow_->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && isstage2_) {
			stage2rotatingarrow_->OnCollision(player_.get());
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

#pragma region プレイヤーとゴール白1
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

#pragma region プレイヤーとゴール黒1
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

#pragma region プレイヤーとゴール白2
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalWhites2_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr && isstage2_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール黒2
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalBlacks2_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr && isstage2_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール白3
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalWhites3_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr && isstage3_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール黒3
	// プレイヤーの座標
	PosA = player_->GetWorldPosition();
	RadiusA = player_->GetRadius();
	for (const std::unique_ptr<Goal>& goal : GoalBlacks3_) {
		PosB = goal->GetWorldPosition();
		RadiusB = goal->GetRadius();
		// 座標AとBの距離を求める
		PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
			(PosB.y - PosA.y) * (PosB.y - PosA.y) +
			(PosB.z - PosA.z) * (PosB.z - PosA.z);
		RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
		// 弾と弾の交差判定
		if (PositionMeasure <= RadiusMeasure && ball_ == nullptr && isstage3_) {
			scene = CLEAR;
			player_->Initialize(modelPlayerHead_.get());
		}
	}
#pragma endregion

#pragma region プレイヤーとゴール白チュートリアル
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

#pragma region プレイヤーとゴール黒チュートリアル
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
		if (PositionMeasure <= RadiusMeasure||isstage2_) {
			player_->SpeedDownOnCollision();
		}
	}

#pragma endregion 

#pragma region プレイヤーと大砲の弾

	//for (Cannonbullet* cannonbullet : cannonbullets_) {
	//	if (cannonbullet && isstage2_) {
	//		// プレイヤーの座標
	//		PosA = player_->GetWorldPosition();
	//		RadiusA = player_->GetRadius();
	//		//炎の座標
	//		PosB = cannonbullet->GetPosition();
	//		RadiusB = cannonbullet->GetScale();
	//		if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

	//			PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
	//		{
	//			player_->CannonOnCollision();
	//		}

	//		if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

	//			PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
	//		{
	//			player_->CannonOnCollision2();
	//		}

	//		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

	//			PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
	//		{
	//			player_->CannonOnCollision3();
	//		}

	//		if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

	//			PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
	//		{
	//			player_->CannonOnCollision4();
	//		}

	//		

	//	}
	//}
#pragma endregion

#pragma region プレイヤーと大砲の弾

	for (Cannonbullet* cannonbullet : cannonbullets_) {
		if (isstage2_ ) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//炎の座標
			PosB = cannonbullet->GetPosition();
			RadiusB = cannonbullet->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->CannonOnCollision();
				cannonbullet->OnCollision();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->CannonOnCollision2();
				cannonbullet->OnCollision();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
			{
				player_->CannonOnCollision3();
				cannonbullet->OnCollision();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
			{
				player_->CannonOnCollision4();
				cannonbullet->OnCollision();
			}



		}
	}
#pragma endregion

#pragma region プレイヤーと大砲 当たったら反射

	for (const std::unique_ptr<Cannon>& cannon : cannons_) {
		if (cannon ) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//炎の座標
			PosB = cannon->GetPosition();
			RadiusB = cannon->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision2();
			}

			if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->OnCollision2();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
			{
				player_->OnCollision2();
			}

			if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
			{
				player_->OnCollision2();
			}



		}
	}
#pragma endregion

#pragma region 大砲と壁 

	
		for (const std::unique_ptr<Stage2>& stage2 : stages2_) {
			for (Cannonbullet* bullet : cannonbullets_)
			{
				if (isstage2_) {
					// プレイヤーの座標
					PosA = stage2->GetPosition();
					RadiusA = stage2->GetScale();
					//炎の座標
					PosB = bullet->GetPosition();
					RadiusB = bullet->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						bullet->OnCollision();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						bullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
					{
						bullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
					{
						bullet->OnCollision();
					}

				}

			}
		
	}
#pragma endregion



#pragma region プレイヤーと回転大砲の弾

		for (RotateCannonBullet* rotatecannonbullet : rotatecannonbullets_) {
			if (rotatecannonbullet) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//炎の座標
				PosB = rotatecannonbullet->GetPosition();
				RadiusB = rotatecannonbullet->GetScale();
				if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
				{
					player_->RotateCannonOnCollision();
					rotatecannonbullet->OnCollision();
				}

				if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
				{
					player_->RotateCannonOnCollision2();
					rotatecannonbullet->OnCollision();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
				{
					player_->CannonOnCollision3();
					rotatecannonbullet->OnCollision();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
				{
					player_->CannonOnCollision4();
					rotatecannonbullet->OnCollision();
				}



			}
		}
#pragma endregion

#pragma region プレイヤーと大砲の本体 当たったら反射

		
			if (isstage2_) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//炎の座標
				PosB = rotatecannon_->GetPosition();
				RadiusB = rotatecannon_->GetScale();
				if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
				{
					player_->OnCollision2();
				}

				if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
				{
					player_->OnCollision2();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
				{
					player_->OnCollision2();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
				{
					player_->OnCollision2();
				}



			}
		
#pragma endregion

#pragma region 回転大砲と壁 


		for (const std::unique_ptr<Stage2>& stage2 : stages2_) {
			for (RotateCannonBullet* rotatebullet : rotatecannonbullets_)
			{
				if (isstage2_) {
					// プレイヤーの座標
					PosA = stage2->GetPosition();
					RadiusA = stage2->GetScale();
					//炎の座標
					PosB = rotatebullet->GetPosition();
					RadiusB = rotatebullet->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						rotatebullet->OnCollision();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						rotatebullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
					{
						rotatebullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
					{
						rotatebullet->OnCollision();
					}

				}

			}

		}
#pragma endregion

#pragma region プレイヤーとステージ2の回復
		if (isstage2_ && stage2recovery_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage2recovery_->GetWorldPosition();
			RadiusB = stage2recovery_->GetRadius();



			// 座標AとBの距離を求める
			PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
				(PosB.y - PosA.y) * (PosB.y - PosA.y) +
				(PosB.z - PosA.z) * (PosB.z - PosA.z);
			RadiusMeasure = (float)(Dot(RadiusA, RadiusB));


			// 弾と弾の交差判定
			if (PositionMeasure <= RadiusMeasure) {
				stage2recovery_->OnCollision();
				player_->RecoveryOnCollision();
			}

		}
#pragma endregion

#pragma region プレイヤーとステージ2の炎
		for (const std::unique_ptr<Fire2>& fire2 : fires2_) {
			if (fire2 && isstage2_) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//炎の座標
				PosB = fire2->GetPosition();
				RadiusB = fire2->GetScale();
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

#pragma region プレイヤーとステージ2のバリア
		for (const std::unique_ptr<Stage2Barrier>& stage2barrier : stage2barriers_) {
			if (stage2barrier && isstage2_) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//2つめのバリアの座標
				PosB = stage2barrier->GetPosition();
				RadiusB = stage2barrier->GetScale();
				if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage2barrier->IsDead() == false)
				{
					player_->OnCollision2();
				}

				if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage2barrier->IsDead() == false)
				{
					player_->OnCollision3();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && stage2barrier->IsDead() == false)
				{
					player_->OnCollision4();
				}

				if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

					PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && stage2barrier->IsDead() == false)
				{
					player_->OnCollision5();
				}

			}
		}
#pragma endregion

#pragma region プレイヤーとステージ3の1つめのワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp_->GetPosition();
			RadiusB = stage3warp_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp2nd2_->GetPosition();
			RadiusB2 = stage3warp2nd2_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warpcooltime_ >= 20) {
				player_->Stage3WarpOnCollision();
				stage3warpcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warpcooltime_ <= 20) {
				stage3warpcooltime_++;
			}


		}

#pragma endregion

#pragma region プレイヤーとステージ3での2つ目のワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp2_->GetPosition();
			RadiusB = stage3warp2_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp2nd_->GetPosition();
			RadiusB2 = stage3warp2nd_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp2ndcooltime_ >= 20) {
				player_->Stage3Warp2OnCollision();
				stage3warp2ndcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp2ndcooltime_ <= 20 ) {
				stage3warp2ndcooltime_++;
			}


		}

#pragma endregion

#pragma region プレイヤーとステージ3の3つめのワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp2nd_->GetPosition();
			RadiusB = stage3warp2nd_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp2_->GetPosition();
			RadiusB2 = stage3warp2_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp2ndcooltime_ >= 20) {
				player_->Stage3Warp2ndOnCollision();
				stage3warp2ndcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp2ndcooltime_ <= 20 ) {
				stage3warp2ndcooltime_++;
			}
		}
#pragma endregion

#pragma region プレイヤーとステージ3での4つ目のワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp2nd2_->GetPosition();
			RadiusB = stage3warp2nd2_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp_->GetPosition();
			RadiusB2 = stage3warp_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warpcooltime_ >= 20) {
				player_->Stage3Warp2nd2OnCollision();
				stage3warpcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warpcooltime_ <= 20 ) {
				stage3warpcooltime_++;
			}
		}

#pragma endregion

#pragma region プレイヤーとステージ3の5つめのワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp3rd_->GetPosition();
			RadiusB = stage3warp3rd_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp3rd2_->GetPosition();
			RadiusB2 = stage3warp3rd2_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp3rdcooltime_ >= 20) {
				player_->Stage3Warp3rdOnCollision();
				stage3warp3rdcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp3rdcooltime_ <= 20) {
				stage3warp3rdcooltime_++;
			}
		}
#pragma endregion

#pragma region プレイヤーとステージ3での6つ目のワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp3rd2_->GetPosition();
			RadiusB = stage3warp3rd2_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp3rd_->GetPosition();
			RadiusB2 = stage3warp3rd_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp3rdcooltime_ >= 20) {
				player_->Stage3Warp3rd2OnCollision();
				stage3warp3rdcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp3rdcooltime_ <= 20) {
				stage3warp3rdcooltime_++;
			}
		}

#pragma endregion

#pragma region プレイヤーとステージ3の7つめのワープ
	
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp4th_->GetPosition();
			RadiusB = stage3warp4th_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp4th2_->GetPosition();
			RadiusB2 = stage3warp4th2_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp4thcooltime_ >= 20) {
				player_->Stage3Warp4thOnCollision();
				stage3warp4thcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp4thcooltime_ <= 20) {
				stage3warp4thcooltime_++;
			}
		}
#pragma endregion

#pragma region プレイヤーとステージ3での8つ目のワープ
		
		if (isstage3_) {
		
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp4th2_->GetPosition();
			RadiusB = stage3warp4th2_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp4th_->GetPosition();
			RadiusB2 = stage3warp4th_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp4thcooltime_ >= 20) {
				player_->Stage3Warp4th2OnCollision();
				stage3warp4thcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp4thcooltime_ <= 20 ) {
				stage3warp4thcooltime_++;
			}
		}

#pragma endregion

#pragma region プレイヤーとステージ3の9つめのワープ
		
		if (isstage3_) {
		
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp5th_->GetPosition();
			RadiusB = stage3warp5th_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp5th2_->GetPosition();
			RadiusB2 = stage3warp5th2_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp5thcooltime_ >= 20) {
				player_->Stage3Warp5thOnCollision();
				stage3warp5thcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp5thcooltime_ <= 20) {
				stage3warp5thcooltime_++;
			}
		}
#pragma endregion

#pragma region プレイヤーとステージ3での10つ目のワープ
		
		if (isstage3_) {
			
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//回復の座標
			PosB = stage3warp5th2_->GetPosition();
			RadiusB = stage3warp5th2_->GetScale();
			//2つめのワープの座標
			PosB2 = stage3warp5th_->GetPosition();
			RadiusB2 = stage3warp5th_->GetScale();

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
			if (PositionMeasure <= RadiusMeasure && stage3warp5thcooltime_ >= 20) {
				player_->Stage3Warp5th2OnCollision();
				stage3warp5thcooltime_ = 0;
				movestoptime = 0;
			}
			else if (PositionMeasure <= RadiusMeasure && movestoptime <= 10) {
				player_->MoveStop();
			}
			if (PositionMeasure >= RadiusMeasure && PositionMeasure2 >= RadiusMeasure2 && stage3warp5thcooltime_ <= 20 ) {
				stage3warp5thcooltime_++;
			}
		}

#pragma endregion

#pragma region プレイヤーと3つめのステージの壁
		for (const std::unique_ptr<Stage3Wall>& stage3wall : stage3walls_) {
			if (stage3wall && isstage3_) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//1つめのステージの座標
				PosB = stage3wall->GetPosition();
				RadiusB = stage3wall->GetScale();
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
				if (PosA.x <= -19.1f)
				{
					player_->SetPlayerPosition4();
				}
				else if (PosA.x >= 19.1f)
				{
					player_->SetPlayerPosition3();
				}
			}
		}
#pragma endregion

#pragma region プレイヤーと大きいスイッチ

		if (isstage3_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//小さいスイッチの座標
			PosB = bigswitch_->GetPosition();
			RadiusB = bigswitch_->GetScale();
			//もしプレイヤーのサイズが中以上だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x <= 1.0f)
			{
				player_->OnCollision2();

			}
			//もしプレイヤーのサイズが小だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x >= 1.5f)
			{
				player_->OnCollision2();
				bigswitch_->OnCollision();
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

#pragma region プレイヤーと大きいスイッチ

		if (isstage3_) {
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			//小さいスイッチの座標
			PosB = bigswitch2_->GetPosition();
			RadiusB = bigswitch2_->GetScale();
			//もしプレイヤーのサイズが中以上だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x <= 1.0f)
			{
				player_->OnCollision2();

			}
			//もしプレイヤーのサイズが小だったら
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && player_->GetRadius().x >= 1.5f)
			{
				player_->OnCollision2();
				bigswitch2_->OnCollision();
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
		for (const std::unique_ptr<Stage3Fire>& stage3fire : stage3fires_) {
			if (stage3fire && isstage3_) {
				// プレイヤーの座標
				PosA = player_->GetWorldPosition();
				RadiusA = player_->GetRadius();
				//炎の座標
				PosB = stage3fire->GetPosition();
				RadiusB = stage3fire->GetScale();
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

#pragma region プレイヤーとステージ3スピードダウン
		
			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			PosB = stage3speeddown_->GetPosition();
			RadiusB = stage3speeddown_->GetScale();
			// 座標AとBの距離を求める
			PositionMeasure = (PosB.x - PosA.x) * (PosB.x - PosA.x) +
				(PosB.y - PosA.y) * (PosB.y - PosA.y) +
				(PosB.z - PosA.z) * (PosB.z - PosA.z);
			RadiusMeasure = (float)(Dot(RadiusA, RadiusB));
			// 弾と弾の交差判定
			if (PositionMeasure <= RadiusMeasure) {
				player_->SpeedDownOnCollision();
			}
		

#pragma endregion 

#pragma region プレイヤーとステージ3スピードアップ

			// プレイヤーの座標
			PosA = player_->GetWorldPosition();
			RadiusA = player_->GetRadius();
			PosB = speedup_->GetPosition();
			RadiusB = speedup_->GetScale();
			if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z )
			{
				player_->SpeedUpOnCollision();
			}

			else if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
			{
				player_->SpeedUpOnCollision2();
			}

			else if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) )
			{
				player_->SpeedUpOnCollision3();
			}

			else if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

				PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) )
			{
				player_->SpeedUpOnCollision4();
			}
			else
			{
				player_->NoSpeedOnCollision();
			}
			// 弾と弾の交差判定
			/*if (PositionMeasure <= RadiusMeasure&&isstage3_) {
				player_->SpeedUpOnCollision();
			}
			else
			{
				player_->NoSpeedOnCollision();
			}*/


#pragma endregion 

#pragma region プレイヤーとステージ2のバリア
			for (const std::unique_ptr<Stage3Barrier>& stage3barrier : stage3barriers_) {
				if (stage3barrier && isstage3_) {
					// プレイヤーの座標
					PosA = player_->GetWorldPosition();
					RadiusA = player_->GetRadius();
					//2つめのバリアの座標
					PosB = stage3barrier->GetPosition();
					RadiusB = stage3barrier->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier->IsDead() == false)
					{
						player_->OnCollision2();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier->IsDead() == false)
					{
						player_->OnCollision3();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && stage3barrier->IsDead() == false)
					{
						player_->OnCollision4();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && stage3barrier->IsDead() == false)
					{
						player_->OnCollision5();
					}

				}
			}
#pragma endregion

#pragma region プレイヤーとステージ2のバリア
			for (const std::unique_ptr<Stage3Barrier2nd>& stage3barrier2nd : stage3barrier2nds_) {
				if (stage3barrier2nd && isstage3_) {
					// プレイヤーの座標
					PosA = player_->GetWorldPosition();
					RadiusA = player_->GetRadius();
					//2つめのバリアの座標
					PosB = stage3barrier2nd->GetPosition();
					RadiusB = stage3barrier2nd->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier2nd->IsDead() == false)
					{
						player_->OnCollision2();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier2nd->IsDead() == false)
					{
						player_->OnCollision3();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && stage3barrier2nd->IsDead() == false)
					{
						player_->OnCollision4();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && stage3barrier2nd->IsDead() == false)
					{
						player_->OnCollision5();
					}

				}
			}
#pragma endregion

#pragma region プレイヤーとステージ2のバリア
			for (const std::unique_ptr<Stage3Barrier3rd>& stage3barrier3rd : stage3barrier3rds_) {
				if (stage3barrier3rd && isstage3_) {
					// プレイヤーの座標
					PosA = player_->GetWorldPosition();
					RadiusA = player_->GetRadius();
					//2つめのバリアの座標
					PosB = stage3barrier3rd->GetPosition();
					RadiusB = stage3barrier3rd->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier3rd->IsDead() == false)
					{
						player_->OnCollision2();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier3rd->IsDead() == false)
					{
						player_->OnCollision3();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && stage3barrier3rd->IsDead() == false)
					{
						player_->OnCollision4();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && stage3barrier3rd->IsDead() == false)
					{
						player_->OnCollision5();
					}

				}
			}
#pragma endregion

#pragma region プレイヤーとステージ2のバリア
			for (const std::unique_ptr<Stage3Barrier4th>& stage3barrier4th : stage3barrier4ths_) {
				if (stage3barrier4th && isstage3_) {
					// プレイヤーの座標
					PosA = player_->GetWorldPosition();
					RadiusA = player_->GetRadius();
					//2つめのバリアの座標
					PosB = stage3barrier4th->GetPosition();
					RadiusB = stage3barrier4th->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier4th->IsDead() == false)
					{
						player_->OnCollision2();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z && stage3barrier4th->IsDead() == false)
					{
						player_->OnCollision3();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) && stage3barrier4th->IsDead() == false)
					{
						player_->OnCollision4();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f) && stage3barrier4th->IsDead() == false)
					{
						player_->OnCollision5();
					}

				}
			}
#pragma endregion

#pragma region 大砲と壁 


			for (const std::unique_ptr<Stage3Wall>& stage3wall : stage3walls_) {
				for (Stage3RotateCannonBullet* stage3rotatecannonbullet : stage3rotatecannonbullets_)
				{
					if (isstage3_) {
						// プレイヤーの座標
						PosA = stage3wall->GetPosition();
						RadiusA = stage3wall->GetScale();
						//炎の座標
						PosB = stage3rotatecannonbullet->GetPosition();
						RadiusB = stage3rotatecannonbullet->GetScale();
						if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

							PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z )
						{
							stage3rotatecannonbullet->OnCollision();
						}

						/*if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

							PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z )
						{
							stage3rotatecannonbullet->OnCollision();
						}

						if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

							PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f) )
						{
							stage3rotatecannonbullet->OnCollision();
						}

						if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

							PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
						{
							stage3rotatecannonbullet->OnCollision();
						}*/

					}

				}

			}
#pragma endregion

#pragma region プレイヤーと大砲の弾

			for (Stage3RotateCannonBullet* stage3rotatecannonbullet : stage3rotatecannonbullets_) {
				if (stage3rotatecannonbullet) {
					// プレイヤーの座標
					PosA = player_->GetWorldPosition();
					RadiusA = player_->GetRadius();
					//炎の座標
					PosB = stage3rotatecannonbullet->GetPosition();
					RadiusB = stage3rotatecannonbullet->GetScale();
					if (PosA.x - RadiusA.x <= PosB.x + RadiusB.x && PosA.x >= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						player_->CannonOnCollision();
						stage3rotatecannonbullet->OnCollision();
					}

					if (PosA.x + RadiusA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z <= PosB.z + RadiusB.z && PosA.z >= PosB.z - RadiusA.z)
					{
						player_->CannonOnCollision2();
						stage3rotatecannonbullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z - RadiusA.z <= PosB.z + (RadiusB.z + 0.2f) && PosA.z >= PosB.z + (RadiusA.z + 0.2f))
					{
						player_->CannonOnCollision3();
						stage3rotatecannonbullet->OnCollision();
					}

					if (PosA.x >= PosB.x - RadiusB.x && PosA.x <= PosB.x + RadiusB.x &&

						PosA.z + RadiusA.z >= PosB.z - (RadiusB.z - 0.2f) && PosA.z <= PosB.z - (RadiusA.z - 0.2f))
					{
						player_->CannonOnCollision4();
						stage3rotatecannonbullet->OnCollision();
					}



				}
			}
#pragma endregion



}

void GameScene::GameReset()
{
	for (const std::unique_ptr<Barrier>& barrier : barriers_) {
		barrier->Reset();
	}
	for (const std::unique_ptr<Barrier2>& barrier2 : barriers2_) {
		barrier2->Reset();
	}
	for (const std::unique_ptr<Stage2Barrier>& stage2barrier : stage2barriers_) {
		stage2barrier->Reset();
	}
	for (const std::unique_ptr<Stage3Barrier2nd>& stage3barrier2nd : stage3barrier2nds_) {
		stage3barrier2nd->Reset();
	}
	for (const std::unique_ptr<Stage3Barrier3rd>& stage3barrier3rd : stage3barrier3rds_) {
		stage3barrier3rd->Reset();
	}
	for (const std::unique_ptr<Stage3Barrier4th>& stage3barrier4th : stage3barrier4ths_) {
		stage3barrier4th->Reset();
	}
	smallswitch_->Reset();
	normalswitch_->Reset();
	bigswitch_->Reset();
	bigswitch2_->Reset();
	player_->Reset();
	scene = GAME;
	//玉の生成
	ball_ = std::make_unique<Ball>();
	//3Dモデルの生成
	modelBall_.reset(Model::CreateFromOBJ("Ball", true));
	//玉の初期化
	ball_->Initialize(modelBall_.get());
	ball_->SetGameScene(this);
}

void GameScene::AddCannonBullet(Cannonbullet* cannonbullet)
{
	cannonbullets_.push_back(cannonbullet);
}

void GameScene::AddRotateCannonBullet(RotateCannonBullet* rotatecannonbullet)
{
	rotatecannonbullets_.push_back(rotatecannonbullet);
}

void GameScene::AddStage3RotateCannonBullet(Stage3RotateCannonBullet* stage3rotatecannonbullet)
{
	stage3rotatecannonbullets_.push_back(stage3rotatecannonbullet);
}
