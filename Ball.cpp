#include "Ball.h"
#include <cassert>
#include <imgui.h>

void Ball::Initialize(Model* ball)
{
	assert(ball);

	ballModel_ = ball;

	ballWorldTransform_.Initialize();

	ballWorldTransform_.translation_ = { 0,0,0 };
	ballWorldTransform_.scale_ = { 10,10,10 };
}

void Ball::Update()
{

	ImGui::Begin("window");
	ImGui::DragFloat3("balltransrate", &ballWorldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("ballangle", &ballWorldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("ballsize", &ballWorldTransform_.scale_.x, 0.01f);
	ImGui::End();

	// 行列を定数バッファに転送
	ballWorldTransform_.UpdateMatrix();
}

void Ball::Draw(const ViewProjection& ViewProjection)
{
	// 3Dモデルを描画
	ballModel_->Draw(ballWorldTransform_, ViewProjection);
}

