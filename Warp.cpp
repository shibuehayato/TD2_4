#include "Warp.h"
#include<cassert>
void Warp::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 14.0f,1.5f,50.0f };
	worldTransform_.rotation_ = { 0.0f,3.2f,0.0f };
}

void Warp::Update()
{
	worldTransform_.UpdateMatrix();

	//ImGui::Begin("Warp");
	//ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	//ImGui::End();
}

void Warp::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Warp::OnCollision()
{
	isOncollision_ = true;
}

void Warp::OnCollision2()
{
	isOncollision_ = false;
}

Vector3 Warp::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Warp::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
