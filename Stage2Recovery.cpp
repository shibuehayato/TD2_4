#include "Stage2Recovery.h"
#include <cassert>
#include <imgui.h>
#include"GameScene.h"
void Stage2Recovery::Initialize(Model* model)
{
	assert(model);

	Model_ = model;


	WorldTransform_.Initialize();



	WorldTransform_.translation_ = {-15.5f,0.0f,40.0f};
	WorldTransform_.scale_ = { 1,1,1 };
	WorldTransform_.rotation_ = { 0,3.9f,0 };
	isDead_ = false;


	WorldTransform_.UpdateMatrix();
	
}

void Stage2Recovery::Update()
{
	// 行列を定数バッファに転送
	WorldTransform_.UpdateMatrix();

	

	
}

void Stage2Recovery::Draw(const ViewProjection& ViewProjection)
{
	// 3Dモデルを描画
	Model_->Draw(WorldTransform_, ViewProjection);


}

void Stage2Recovery::OnCollision()
{
	isDead_ = true;
}

// ワールド座標を取得
Vector3 Stage2Recovery::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = WorldTransform_.matWorld_.m[3][0];
	worldPos.y = WorldTransform_.matWorld_.m[3][1];
	worldPos.z = WorldTransform_.matWorld_.m[3][2];
	return worldPos;
}