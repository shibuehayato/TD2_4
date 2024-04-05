#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"

class Ball
{
public:
	void Initialize(Model* ball);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition() {

		// ワールド座標を入れる変数
		Vector3 worldPos{};
		WorldTransform_.matWorld_.m;
		// ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = WorldTransform_.matWorld_.m[3][0];
		worldPos.y = WorldTransform_.matWorld_.m[3][1];
		worldPos.z = WorldTransform_.matWorld_.m[3][2];
		return worldPos;
	}
	// 大きさ取得
	Vector3 GetRadius() { return WorldTransform_.scale_; };
	
private:
	WorldTransform WorldTransform_;

	Model* Model_=nullptr;

	bool isDead_ = false;
};

