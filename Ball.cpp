#include "Ball.h"
#include <cassert>
#include <imgui.h>

void Ball::Initialize(Model* model)
{
	assert(model);

	Model_ = model;

	WorldTransform_.Initialize();

	WorldTransform_.translation_ = { 7.5f,0,4 };
	WorldTransform_.scale_ = { 1,1,1 };

	isDead_ = false;
}

void Ball::Update()
{

	ImGui::Begin("window");
	ImGui::DragFloat3("balltransrate", &WorldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("ballangle", &WorldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ballsize", &WorldTransform_.scale_.x, 0.01f);
	ImGui::End();

	// 行列を定数バッファに転送
	WorldTransform_.UpdateMatrix();
}

void Ball::Draw(const ViewProjection& ViewProjection)
{
	// 3Dモデルを描画
	Model_->Draw(WorldTransform_, ViewProjection);
}

void Ball::OnCollision()
{
	isDead_ = true;
}

