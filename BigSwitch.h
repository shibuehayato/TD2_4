#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"


class BigSwitch
{
public:
	void Initialize(Model* model, Model* modelbutton);

	void Update();

	void Draw(ViewProjection& viewProjection);
	//当たった時の処理
	void OnCollision();
	//ステージ2の時の位置調整の関数
	void Position();

	bool GetIsOnCollision() { return isOncollision_; }

	void Reset();

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();
	//指定したサイズになった時の関数
	bool IsScale() { return isScale_; }

private:
	WorldTransform worldTransformswitch_;
	WorldTransform worldTransformbutton_;

	Model* model_ = nullptr;
	Model* modelbutton_ = nullptr;



	uint32_t textureHandle_ = 0u;

	bool isOncollision_ = false;
	//指定したになった時のフラグ
	bool isScale_ = false;



	
};

