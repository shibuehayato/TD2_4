#include "model.h"
#include "WorldTransform.h"
#include <Input.h>
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

	

private:
	// ワールド変換データ
	WorldTransform worldTransformHead_;
	// モデル
	Model* HeadModel_ = nullptr;
	// 速さ
	float speed = 0;
	// 移動量を保管する
	Vector3 KeepMove = { 0,0,0 };
	//反発係数
	float e = 0.4f;

	Vector3 incident = { 0.2f,0.0f,0.2f };
	Vector3 normal = { 0.0f,0.5f,0.0f };
	// 移動できるかフラグ
	bool IsMove = false;
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
};