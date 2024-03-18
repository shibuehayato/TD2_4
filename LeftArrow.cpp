#include "LeftArrow.h"
#include<cassert>
void LeftArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 6.0f,0.0f,35.0f };

	velocity_ = { 1.0f,1.0f,1.0f };
}

void LeftArrow::Update()
{
	worldTransform_.UpdateMatrix();

	

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
