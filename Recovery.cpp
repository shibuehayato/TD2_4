#include "Recovery.h"
#include <cassert>
#include <imgui.h>

void Recovery::Initialize(Model* model)
{
	assert(model);

	Model_ = model;

	WorldTransform_.Initialize();

	WorldTransform_.translation_ = { -14,0,-7 };
	WorldTransform_.scale_ = { 1,1,1 };
	WorldTransform_.rotation_ = { 0,3.9f,0 };
	isDead_ = false;

	WorldTransform_.UpdateMatrix();
}

void Recovery::Update()
{
	// 行列を定数バッファに転送
	WorldTransform_.UpdateMatrix();
}

void Recovery::Draw(const ViewProjection& ViewProjection)
{
	// 3Dモデルを描画
	Model_->Draw(WorldTransform_, ViewProjection);
}

void Recovery::OnCollision()
{
	isDead_ = true;
}

