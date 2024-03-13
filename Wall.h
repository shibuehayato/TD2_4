#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
#include<Mymath.h>


class Wall
{
public:

	void Initialize(Model*Wall);

	void Update();

	void Draw(ViewProjection& viewProjection);


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

		//	// ローカル座標でのオフセット
		//const Vector3 offset = { 10.0f, 1.5f, 0.0f };
		//// ワールド座標に変換
		//Vector3 worldPos = TransformNormal(offset, worldTransform_.matWorld_);
		//return worldPos;
	}
	// 大きさ取得
	Vector3 GetRadius() { return worldTransform_.scale_; };
	/*Vector3 GetRadius() {
		Vector3 size{};
		size.x = worldTransform_.scale_.x;
		size.y = worldTransform_.scale_.y;
		size.z = worldTransform_.scale_.z;
		return size;
	};*/
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* Model_ = nullptr;
};

