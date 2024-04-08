#include "Goal.h"
#include"GameScene.h"

void Goal::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
