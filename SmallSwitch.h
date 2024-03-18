#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ImGuiManager.h"
class SmallSwitch
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);
	//当たった時の処理
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_;

	uint32_t textureHandle_ = 0u;

	bool isOncollision_ = false;

};

