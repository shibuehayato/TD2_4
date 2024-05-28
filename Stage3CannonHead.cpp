#include "Stage3CannonHead.h"

void Stage3CannonHead::Initialize(Model* model)
{
	assert(model);
	
		model_ = model;

		worldTransform_.Initialize();
	
	worldTransform_.translation_ = { -15,	1, 46 };
	worldTransform_.rotation_ = { 1.5f, -5.23f,	0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
}

void Stage3CannonHead::Update()
{
	worldTransform_.UpdateMatrix();
	worldTransform_.rotation_.y += rotation_;
	if (isrotation_ == false)
	{
		rotation_ = 0.05f;
	}
	else if (isrotation_ == true)
	{
		rotation_ = -0.05f;
	}

	if (worldTransform_.rotation_.y >= -3.8f)
	{
		isrotation_ = true;
	}
	else if (worldTransform_.rotation_.y <= -5.8f)
	{
		isrotation_ = false;
	}
}

void Stage3CannonHead::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
