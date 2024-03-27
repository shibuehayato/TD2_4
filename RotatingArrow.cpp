#include "RotatingArrow.h"
#include<cassert>
#include <Mymath.h>
#include <Player.h>

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

void RotatingArrow::OnCollision()
{
	Player* player_ = nullptr;
	//速度ベクトルを自機に向きに合わせて回転させる
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);

	player_->ArrowOnCollision(velocity_);
}
