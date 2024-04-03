#include "LeftArrow.h"
#include<cassert>
void LeftArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 6.0f,0.0f,35.0f };
	worldTransform_.rotation_ = { 0.0f,-1.6f,0.0f };
	velocity_ = { 1.0f,1.0f,1.0f };
}

void LeftArrow::Update()
{
	worldTransform_.UpdateMatrix();

	ImGui::Begin("LeftArrow");
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::End();

}

void LeftArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 LeftArrow::GetVelocity()
{
	Vector3 velocity;

	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}
