#include "Ground.h"
#include<cassert>
#include"GameScene.h"
void Ground::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f,-1.0f,0.0f };
	
}

void Ground::Update()
{
	worldTransform_.UpdateMatrix();

	if (gameScene_->IsTutorial())
	{
	 worldTransform_.translation_ = { 0.0f,-1.0f,0.0f };
		worldTransform_.scale_ = { 27.4f,1.0f,26.2f };
	}
	else if(gameScene_->IsStage1()||gameScene_->IsStage2())
	{
		worldTransform_.translation_ = { -4.2f,-1.0f,10.7f };
		worldTransform_.scale_ = { 27.9f,1.0f,26.4f };
	}
	else if (gameScene_->IsStage3())
	{
		worldTransform_.translation_ = { -3.3f,-1.0f,0.0f };
		worldTransform_.scale_ = { 28.5f,1.0f,26.9f };
	}

	/*ImGui::Begin("Ground");
	ImGui::DragFloat3("Scale", &worldTransform_.scale_.x, 0.1f);
	ImGui::DragFloat3("Position", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();*/
}

void Ground::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
