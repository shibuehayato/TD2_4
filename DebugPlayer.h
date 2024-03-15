﻿#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
#include <Input.h>
#include<Mymath.h>

class DebugPlayer
{
public:

	void Initialize(Model* Wall);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void WallOnCollision();


	// ワールド座標を取得
	Vector3 GetWorldPosition() {

		// ワールド座標を入れる変数
		Vector3 worldPos{};
		worldTransform_.matWorld_.m;
		// ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];
		return worldPos;
	}

	// 大きさ取得
	Vector3 GetRadius() { return worldTransform_.scale_; };
	

private:
	Input* input_=nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* Model_ = nullptr;

	//スピード
	Vector3 speed_;
};
