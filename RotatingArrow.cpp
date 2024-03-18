#include "RotatingArrow.h"
#include<cassert>

void RotatingArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 4.0f,0.0f,12.0f };

}

void RotatingArrow::Update()
{

	worldTransform_.UpdateMatrix();

	
	worldTransform_.rotation_.y += 0.1f;



}

void RotatingArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
