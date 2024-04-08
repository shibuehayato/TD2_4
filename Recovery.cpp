#include "Recovery.h"
#include <cassert>
#include <imgui.h>
#include"GameScene.h"
void Recovery::Initialize(Model* model)
{
	assert(model);

	Model_ = model;


	WorldTransform_.Initialize();
	
	

		WorldTransform_.translation_ = { -14,0,-7 };
		WorldTransform_.scale_ = { 1,1,1 };
		WorldTransform_.rotation_ = { 0,3.9f,0 };
	
	isDead_ = false;

	drawcooltime_ = 0;

	WorldTransform_.UpdateMatrix();
}

void Recovery::Update()
{
	// 行列を定数バッファに転送
	WorldTransform_.UpdateMatrix();
	if (drawcooltime_ <= 10)
	{
		drawcooltime_++;
	}
	if (gamescene_->IsStage1()&&isDead_==false)
	{
		WorldTransform_.translation_ = { -14.0f,0.0f,-7.0f };
		WorldTransform_.rotation_ = { 0.0f,3.9f,0.0f };
		
	}

	if (gamescene_->IsStage2()&&isDead_==false)
	{
		WorldTransform_.translation_ = { 12.0f,0.0f,25.0f };

		WorldTransform_.rotation_ = { 0.0f,3.9f,0.0f };
		
	}
	
}

void Recovery::Draw(const ViewProjection& ViewProjection)
{
	if (drawcooltime_ >= 10)
	{
		// 3Dモデルを描画
		Model_->Draw(WorldTransform_, ViewProjection);
	}

}

void Recovery::OnCollision()
{
	isDead_ = true;
}

