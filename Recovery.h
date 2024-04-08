#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"

class GameScene;

class Recovery
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }

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
	WorldTransform WorldTransform_ ;
	WorldTransform worldTransform2_;

	Model* Model_ =  nullptr;

	bool isDead_ = false;

	int32_t drawcooltime_;

	GameScene* gamescene_ = nullptr;
};

