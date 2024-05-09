#pragma once
#include"WorldTransform.h"
#include"Model.h"
class Stage3Warp5th
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();
	void OnCollision2();

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();

	bool IsOncollision() { return isOncollision_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	bool isOncollision_ = false;
};

