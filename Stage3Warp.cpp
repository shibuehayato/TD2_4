#include "Stage3Warp.h"
#include<cassert>
#include"ImGuiManager.h"
void Stage3Warp::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 16.0f,1.5f,23.0f };
	worldTransform_.rotation_ = { 0.0f,3.2f,0.0f };
}

void Stage3Warp::Update()
{
	worldTransform_.UpdateMatrix();
	/*ImGui::Begin("Warp");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 1.0f);
	ImGui::End();*/
}

void Stage3Warp::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Stage3Warp::OnCollision()
{
	isOncollision_ = true;
}

void Stage3Warp::OnCollision2()
{
	isOncollision_ = false;
}

Vector3 Stage3Warp::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Warp::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
