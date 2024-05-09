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
#include"WindParticle.h"
#include <cstdlib>
#include <ctime>  
#include"Cyclone.h"
#include"Cannon.h"
#include"RotateCannon.h"
#include"StageSelect.h"
#include"Stage3Wall.h"
#include"Stage3Barrier.h"
#include"Stage3Barrier2nd.h"
#include"Stage3Barrier3rd.h"
#include"Stage3Barrier4th.h"
#include"Stage3Warp.h"
#include"Stage3Warp2.h"
#include"Stage3Warp2nd.h"
#include"Stage3Warp2nd2.h"
#include"Stage3Warp3rd.h"
#include"Stage3Warp3rd2.h"
#include"Stage3Warp4th.h"
#include"Stage3Warp4th2.h"
#include"Stage3Warp5th.h"
#include"Stage3Warp5th2.h"
#include"Stage3Fire.h"
#include"BigSwitch.h"
#include"BigSwitch2.h"
#include"Stage3SpeedDown.h"
#include"SpeedUp.h"
#include"Stage3RotateConnon.h"

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

	//風のパーティクル
	void UpdateWindParticlePopCommands();
	void WindParticleInitilize();
	void WindParticleStartPosition(float& startX, float& startY, float& startZ);

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

	void LoadCannonPopData();
	void UpdateCannonPopCommands();
	void CannonGenerate(const Vector3& position, const Vector3& Headposition, const Vector3& rotation);

	//ステージ3の壁生成の関数
	void LoadStage3WallPopData();
	void UpdateStage3WallPopCommands();
	void Stage3WallGenerate(const Vector3& position);

	void LoadStage3BarrierPopData();
	void UpdateStage3BarrierPopCommands();
	void Stage3BarrierGenerate(const Vector3& position);

	void LoadStage3Barrier2ndPopData();
	void UpdateStage3Barrier2ndPopCommands();
	void Stage3Barrier2ndGenerate(const Vector3& position);

	void LoadStage3Barrier3rdPopData();
	void UpdateStage3Barrier3rdPopCommands();
	void Stage3Barrier3rdGenerate(const Vector3& position);

	void LoadStage3Barrier4thPopData();
	void UpdateStage3Barrier4thPopCommands();
	void Stage3Barrier4thGenerate(const Vector3& position);
	//-------------------------------//
	//ステージ3の炎
	void LoadStage3FirePopData();
	void UpdateStage3FirePopCommands();
	void Stage3FireGeneration(const Vector3& position);

	//--------------------------------------------//
	//バリアが解除した時の処理の関数
	void BarrierRemoved();

	void CheckAllCollisions();

	//ゲームをクリアしたりゲームオーバーになった時ゲームを終了せずにゲームができるようにリセットする関数
	void GameReset();

	//大砲の弾を登録するための関数
	void AddCannonBullet(Cannonbullet* cannonbullet);
	//回転大砲の弾を登録するための関数
	void AddRotateCannonBullet(RotateCannonBullet* rotatecannonbullet);
	//ステージ3の回転大砲の弾を登録するための関数
	void AddStage3RotateCannonBullet(Stage3RotateCannonBullet* stage3rotatecannonbullet);
	
	//それぞれのギミックをステージごとに位置を変えるために必要なフラグの関数
	bool IsTutorial() { return istutorial_; }
	bool IsStage1() { return isstage1_; }
	bool IsStage2() { return isstage2_; }
	bool IsStage3() { return isstage3_; }

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
	//風パーティクルのギミックの宣言
	std::list<WindParticle*> windParticles_;
	std::unique_ptr<Model> modelWind_;
	//風ファンのギミックの宣言
	std::unique_ptr<Cyclone> cyclone_;
	std::unique_ptr<Model> modelCyclone_;
	//落とし穴の宣言
	std::list<std::unique_ptr<Pitfall>> pitfalls_;
	std::unique_ptr<Model> modelpitfall_;
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
	//大砲
	std::list<std::unique_ptr<Cannon>> cannons_;
	std::unique_ptr<Model> modelcannon_;
	std::unique_ptr<Model>modelcannonhead_;
	std::list <Cannonbullet*> cannonbullets_;
	//回転大砲
	std::unique_ptr<RotateCannon> rotatecannon_;
	std::list <RotateCannonBullet*> rotatecannonbullets_;
	//ステージセレクト
	std::unique_ptr<StageSelect> stageselect_;

	//ステージ3の壁
	std::list<std::unique_ptr<Stage3Wall>> stage3walls_;

	//ステージ3のバリア
	std::list<std::unique_ptr<Stage3Barrier>> stage3barriers_;
	std::list<std::unique_ptr<Stage3Barrier2nd>> stage3barrier2nds_;
	std::list<std::unique_ptr<Stage3Barrier3rd>> stage3barrier3rds_;
	std::list<std::unique_ptr<Stage3Barrier4th>> stage3barrier4ths_;
	//------------------------------//
	//ステージ3のワープ
	std::unique_ptr<Stage3Warp> stage3warp_;
	int32_t stage3warpcooltime_ = 0;
	std::unique_ptr<Stage3Warp2> stage3warp2_;
	int32_t stage3warp2cooltime_ = 0;
	std::unique_ptr<Stage3Warp2nd> stage3warp2nd_;
	int32_t stage3warp2ndcooltime_ = 0;
	std::unique_ptr<Stage3Warp2nd2> stage3warp2nd2_;
	int32_t stage3warp2nd2cooltime_ = 0;
	std::unique_ptr<Stage3Warp3rd> stage3warp3rd_;
	int32_t stage3warp3rdcooltime_ = 0;
	std::unique_ptr<Stage3Warp3rd2> stage3warp3rd2_;
	int32_t stage3warp3rd2cooltime_ = 0;
	std::unique_ptr<Stage3Warp4th> stage3warp4th_;
	int32_t stage3warp4thcooltime_ = 0;
	std::unique_ptr<Stage3Warp4th2> stage3warp4th2_;
	int32_t stage3warp4th2cooltime_ = 0;
	std::unique_ptr<Stage3Warp5th> stage3warp5th_;
	int32_t stage3warp5thcooltime_ = 0;
	std::unique_ptr<Stage3Warp5th2> stage3warp5th2_;
	int32_t stage3warp5th2cooltime_ = 0;
	//----------------------------------//
	//ステージ3の炎
	std::list<std::unique_ptr<Stage3Fire>> stage3fires_;
	//大のスイッチ
	std::unique_ptr<BigSwitch> bigswitch_;
	std::unique_ptr<BigSwitch2> bigswitch2_;
	std::unique_ptr<Model> modelbigswitch_;
	std::unique_ptr<Model> modelbigbutton_;
	std::unique_ptr<Model> modelbigbutton2_;
	//-----------------------------//
	//ステージ3のスピードダウン
	std::unique_ptr<Stage3SpeedDown> stage3speeddown_;
	//スピードアップ
	std::unique_ptr<SpeedUp> speedup_;
	//ステージ3の回転大砲
	std::unique_ptr<Stage3RotateConnon> stage3rotatecannon_;
	std::list <Stage3RotateCannonBullet*> stage3rotatecannonbullets_;

	//csvに読み込まれた数値を代入するための変数
	Vector3 position_;
	Vector3 rotation_;

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
	//大砲の発生コマンド
	std::stringstream CannonPopCommands;
	//ステージ3の発生コマンド
	std::stringstream stage3wallPopCommands;
	//ステージ3のバリア発生コマンド
	std::stringstream stage3barrierPopCommands;
	std::stringstream stage3barrier2ndPopCommands;
	std::stringstream stage3barrier3rdPopCommands;
	std::stringstream stage3barrier4thPopCommands;
	//----------------------------//
	//ステージ3の炎
	std::stringstream stage3firePopCommands;

	//ステージを分けるためのフラグ
	bool istutorial_ = false;
	bool isstage1_ = false;
	bool isstage2_ = false;
	bool isstage3_ = false;

	

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

	//風のパーティクル
	float windtime_ = 3;

	//風のパーティクル範囲
	const Vector3 rangestart = { 3.0f,3.0f,1.5f }; //範囲はじめ
	const Vector3 rangeend = { -14.0f,3.5f,26.0f }; //範囲終わり

	// マップ全体を映すカメラ
	bool IsFullMapCamera = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};