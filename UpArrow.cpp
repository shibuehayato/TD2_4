#include "UpArrow.h"
#include <cassert>
void UpArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 26.0f,0.0f,0.0f };

	velocity_ = { 1.0f,1.0f,1.0f };
}

void UpArrow::Update()
{
	worldTransform_.UpdateMatrix();
}

void UpArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 UpArrow::GetVelocity()
{
	Vector3 velocity;

	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}
