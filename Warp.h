#pragma once
#include"WorldTransform.h"
#include"Model.h"
class Warp
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};

