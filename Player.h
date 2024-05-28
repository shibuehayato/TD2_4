#include "model.h"
#include "WorldTransform.h"
#include <Input.h>
#include <Audio.h>


class Cannonbullet;
class Player {
public:

	void Initialize(Model* head);
	void Update();
	void Draw(ViewProjection viewProjection);

	void OnCollision();
	//反射するための関数
	void OnCollision2();
	void OnCollision3();
	void OnCollision4();
	void OnCollision5();
	//-----------------//
	void OnCollision6();
	//炎に当たった時の関数
	void OnCollision7();
	//スピードダウンに当たった時の関数
	void SpeedDownOnCollision();
	void SpeedUpOnCollision();
	void SpeedUpOnCollision2();
	void SpeedUpOnCollision3();
	void SpeedUpOnCollision4();
	void NoSpeedOnCollision();
	//1つめのワープに当たった時の関数
	void WarpOnCollision();
	void WarpOnCollision2();
	void MoveStop();
	//ステージ3のワープに当たった時の関数
	void Stage3WarpOnCollision();
	void Stage3Warp2OnCollision();
	void Stage3Warp2ndOnCollision();
	void Stage3Warp2nd2OnCollision();
	void Stage3Warp3rdOnCollision();
	void Stage3Warp3rd2OnCollision();
	void Stage3Warp4thOnCollision();
	void Stage3Warp4th2OnCollision();
	void Stage3Warp5thOnCollision();
	void Stage3Warp5th2OnCollision();
	//当た後
	void RecoveryOnCollision();
	void WindOnCollision();
	//落とし穴の当たり判定
	void PitfallOnCollision(); 
	//矢印の当たり判定
	void ArrowOnCollision();
	//大砲に当たった時の関数
	void CannonOnCollision();  //右
	void CannonOnCollision2(); //左
	void CannonOnCollision3(); //上
	void CannonOnCollision4(); //下
	//回転大砲に当たった時の関数
	void RotateCannonOnCollision();
	void RotateCannonOnCollision2();
	//-----------------------//

	//プレイヤーの位置調整の関数
	void SetPlayerPosition();
	void SetPlayerPosition2();
	void SetPlayerPosition3();
	void SetPlayerPosition4();
	

	void Reset();

	float GetTransformZ() { return worldTransformHead_.translation_.z; };

	// ワールド座標を取得
	Vector3 GetWorldPosition() {

		// ワールド座標を入れる変数
		Vector3 worldPos{};
		worldTransformHead_.matWorld_.m;
		// ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldTransformHead_.matWorld_.m[3][0];
		worldPos.y = worldTransformHead_.matWorld_.m[3][1];
		worldPos.z = worldTransformHead_.matWorld_.m[3][2];
		return worldPos;
	}
	// 大きさ取得
	Vector3 GetRadius() { return worldTransformHead_.scale_; };

	//矢印の向きのセッター
	void SetKeepMove(const Vector3& ArrowRot) { ArrowRot_ = ArrowRot; };

	//プレイヤーの大きさの取得xだけ
	float GetPlayerScaleX() { return worldTransformHead_.scale_.x; };

	bool IsMove() { return isMove; }

	const WorldTransform& GetWorldTransform() { return worldTransformHead_; }

private:
	Audio* audio_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransformHead_;
	// モデル
	Model* HeadModel_ = nullptr;
	// 速さ
	float speed = 0;
	//反発係数
	float e = 0.4f;

	Vector3 incident = { 0.2f,0.0f,0.2f };
	Vector3 normal = { 0.0f,0.5f,0.0f };
	// 移動できるかフラグ
	bool isMove = false;
	// 移動量を保管する
	Vector3 KeepMove_ = { 0,0,0 };

	//当たった時一定の無敵時間を作るための変数
	bool isOncollision_;

	int32_t Oncollisiontimer_;

	//---------------------------//

	//スピードダウンに必要な変数
	float speeddown_ = 0.0f;
	//スピードアップに必要な変数
	float speedup_ = 0.0f;
	float speedup2_ = 0.0f;

	//当たった時点滅する用の変数
	int32_t blikingtimer_;
	bool isBliking_;
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	Vector3 ArrowRot_ = { 0.0f,0.0f,0.0f };

	Cannonbullet* cannonbullet_ = nullptr;

	uint32_t DisengageSE_ = 0;
};