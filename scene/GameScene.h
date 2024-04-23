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
#include"Tutorial.h"
#include<memory>
#include"DebugCamera.h"
#include<list>
#include <sstream>
#include"Stage1.h"
#include"Fire.h"
#include"SmallSwitch.h"
#include"NormalSwitch.h"
#include"Wind.h"
#include"Pitfall.h"
#include"Ball.h"
#include "Skydome.h"
#include"Barrier.h"
#include"Warp.h"
#include"Warp2.h"
#include"Barrier2.h"
#include"RightArrow.h"
#include"LeftArrow.h"
#include"UpArrow.h"
#include"DownArrow.h"
#include"RotatingArrow.h"
#include"Mymath.h"
#include"Recovery.h"
#include"Stage2Recovery.h"
#include"Stage2.h"
#include"Fire2.h"
#include"Stage2Barrier.h"
#include"Stage2RotatingArrow.h"
#include"SpeedDown.h"
#include <Goal.h>
#include "FollowCamera.h"

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

	void Stage2LoadWallPopData();
	void Stage2UpdateWallPopCommands();
	void Stage2WallGeneration(const Vector3& position);

	void LoadFlamePopData();
	void UpdateFlamePopCommands();
	void FlameGeneration(const Vector3& position);

	void LoadStage2FlamePopData();
	void UpdateStage2FlamePopCommands();
	void Stage2FlameGeneration(const Vector3& position);

	void LoadWindPopData();
	void UpdateWindPopCommands();
	void WindGeneration(const Vector3& position);

	void LoadBarrierPopData();
	void UpdateBarrierPopCommands();
	void BarrierGeneration(const Vector3& position);

	void LoadBarrier2PopData();
	void UpdateBarrier2PopCommands();
	void Barrier2Generation(const Vector3& position);

	void LoadPitfallPopData();
	void UpdatePitfallPopCommands();
	void PitfallGeneration(const Vector3& position);

	void LoadArrowPopData();
	void UpdateArrowPopCommands();
	void ArrowGeneration(const Vector3& position);

	//ゴールステージ１
	void LoadGoalWhitePopData();
	void UpdateGoalWhitePopCommands();
	void GoalWhiteGeneration(const Vector3& position);

	void LoadGoalBlackPopData();
	void UpdateGoalBlackPopCommands();
	void GoalBlackGeneration(const Vector3& position);

	//ゴールチュートリアル
	void LoadTutorialGoalWhitePopData();
	void UpdateTutorialGoalWhitePopCommands();
	void TutorialGoalWhiteGeneration(const Vector3& position);

	void LoadStage2BarrierPopData();

	void UpdateStage2BarrierPopCommands();

	void Stage2BarrierGeneration(const Vector3& position);

	void LoadSpeedDownPopData();

	void UpdateSpeedDownPopCommands();

	void SpeedDownGeneration(const Vector3& position);

	void LoadTutorialGoalBlackPopData();
	void UpdateTutorialGoalBlackPopCommands();
	void TutorialGoalBlackGeneration(const Vector3& position);
	//--------------------------------------------//
	//バリアが解除した時の処理の関数
	void BarrierRemoved();

	void CheckAllCollisions();

	//それぞれのギミックをステージごとに位置を変えるために必要なフラグの関数
	bool IsStage1() { return isstage1_; }
	bool IsStage2() { return isstage2_; }
	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isDebugCameraAcctive_ = false;


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
	std::unique_ptr<Tutorial> stage_;
	std::list<std::unique_ptr<Tutorial>> tutorials_;
	std::unique_ptr<Model> modelwall_;
	std::list<std::unique_ptr<Stage1>> stages1_;
	std::list<std::unique_ptr<Stage2>> stages2_;

	//ギミックの宣言
	//炎の壁の宣言
	std::list<std::unique_ptr<Fire>> fires_;
	std::list<std::unique_ptr<Fire2>> fires2_;
	//小スイッチの宣言
	std::unique_ptr<SmallSwitch> smallswitch_;
	std::unique_ptr<Model> modelsmallswitch_;
	std::unique_ptr<Model> modelsmallbutton_;
	//中スイッチの宣言
	std::unique_ptr<NormalSwitch> normalswitch_;
	std::unique_ptr<Model> modelnormalswitch_;
	std::unique_ptr<Model> modelnormalbutton_;
	//風のギミックの宣言
	std::list<std::unique_ptr<Wind>> winds_;
	//落とし穴の宣言
	std::list<std::unique_ptr<Pitfall>> pitfalls_;
	//バリアの宣言
	std::list<std::unique_ptr<Barrier>> barriers_;
	std::unique_ptr<Model> modelbarrier_;
    //ワープの宣言
	std::unique_ptr<Warp> warp_;
	std::unique_ptr<Model> modelwarp_;
	int32_t warpcooltime_ = 0;
	int32_t movestoptime = 0;
	//2つ目のワープの宣言
	std::unique_ptr<Warp2> warp2_;
	//2つめのバリアの宣言
	std::list <std::unique_ptr<Barrier2>> barriers2_;
	std::unique_ptr<Barrier2> barrier2_;
	//ステージ2のバリアの宣言
	std::list < std::unique_ptr<Stage2Barrier>> stage2barriers_;
	//玉
	std::unique_ptr<Model> modelBall_;
	std::unique_ptr <Ball> ball_;
	bool isballdead_ = false;
	//右矢印の宣言
	std::unique_ptr<RightArrow> rightarrow_;
	//左矢印の宣言
	std::unique_ptr<LeftArrow> leftarrow_;
	//上矢印の宣言
	std::unique_ptr<UpArrow> uparrow_;
	//下矢印の宣言
	std::unique_ptr<DownArrow> downarrow_;
	//回転矢印の宣言
	//std::unique_ptr<RotatingArrow> rotatingarrow_;
	std::unique_ptr<Model> modelArrow_;
	std::list<std::unique_ptr<RotatingArrow>> Arrows_;
	//ステージ2の回転矢印の宣言
	std::unique_ptr<Stage2RotatingArrow> stage2rotatingarrow_;

	//ゴールの白と黒
	std::unique_ptr<Model> modelGoalWhite_;
	std::list<std::unique_ptr<Goal>> GoalWhites_;
	std::list<std::unique_ptr<Goal>> TutorialGoalWhites_;
	std::unique_ptr<Model> modelGoalBlack_;
	std::list<std::unique_ptr<Goal>> GoalBlacks_;
	std::list<std::unique_ptr<Goal>> TutorialGoalBlacks_;

	//回復
	std::unique_ptr<Model> modelRecovery_;
	std::unique_ptr <Recovery> recovery_;
	std::unique_ptr <Stage2Recovery> stage2recovery_;
	bool isrecoverydead_ = false;
	bool isrecoverydeadflag = false;
	float recoveryTime_ = 0;
	//スピードダウン
	std::list<std::unique_ptr<SpeedDown>> speeddowns_;
	std::unique_ptr<Model> modelspeeddown_;

	// 壁発生コマンド
	std::stringstream wallPopCommands;
	// 壁発生コマンド
	std::stringstream stage1wallPopCommands;
	//ステージ2の壁発生コマンド
	std::stringstream stage2wallPopCommands;
	//炎発生コマンド
	std::stringstream flamePopCommands;
	std::stringstream stage2flamePopCommands;
	//風のギミックの発生コマンド
	std::stringstream windPopCommands;
	//バリアの発生コマンド
	std::stringstream barrierPopCommands;
	//2つめのバリアの発生コマンド
	std::stringstream barrier2PopCommands;
	//落とし穴の発生コマンド
	std::stringstream pitfallPopCommands;
	//ステージ２の回復発生コマンド
	std::stringstream stage2recoveryPopCommands;
	//矢印のギミックの発生コマンド
	std::stringstream arrowPopCommands;
	//ステージ2のバリア発生コマンド
	std::stringstream stage2barrierPopCommands;
	//スピードダウンの発生コマンド
	std::stringstream speedDownPopCommands;
	//ゴールのギミックの発生コマンド
	std::stringstream GoalWhitePopCommands;
	std::stringstream GoalBlackPopCommands;
	std::stringstream TutorialGoalWhitePopCommands;
	std::stringstream TutorialGoalBlackPopCommands;

	//ステージを分けるためのフラグ
	bool istutorial_ = false;
	bool isstage1_ = false;
	bool isstage2_ = false;
	

	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 3Dモデルの生成
	std::unique_ptr<Model> modelSkydome_;

	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	XINPUT_STATE prejoyState;

	// シーン切り替え
	enum Scene {
		TITLE,
		OPERATION,
		GAME,
		CLEAR,
		GAMEOVER
	};
	Scene scene = TITLE;

	//テクスチャハンドル
	uint32_t TitleTexture_ = 0;
	uint32_t OperationTexture_ = 0;
	uint32_t ClearTexture_ = 0;
	uint32_t GameOverTexture_ = 0;

	//シーン画面のスプライト
	std::unique_ptr<Sprite> TitleSprite_ = nullptr;
	std::unique_ptr<Sprite> OperationSprite_ = nullptr;
	std::unique_ptr<Sprite> ClearSprite_ = nullptr;
	std::unique_ptr<Sprite> GameOverSprite_ = nullptr;

	//大きさ
	enum Size {
		Big_,
		Medium_,
		Small_
	};
	Size size_ = Small_;

	//テクスチャハンドル
	uint32_t BigTexture_ = 0;
	uint32_t MediumTexture_ = 0;
	uint32_t SmallTexture_ = 0;

	//大きさのスプライト
	std::unique_ptr<Sprite> BigSprite_ = nullptr;
	std::unique_ptr<Sprite> MediumSprite_ = nullptr;
	std::unique_ptr<Sprite> SmallSprite_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// マップ全体を映すカメラ
	bool IsFullMapCamera = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};