#include "LeftArrow.h"
#include<cassert>
#include"Player.h"
#include <Mymath.h>

void LeftArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 26.0f,0.0f,50.0f };
	worldTransform_.rotation_ = { 0.0f,1.6f,0.0f };
	velocity_ = { 1.0f,1.0f,1.0f };

	ArrowRot = { 1.0f,1.0f,1.0f };


	//試し
	worldTransform_.translation_ = { 15.0f,0.0f,-20.0f };
}

void LeftArrow::Update()
{
	worldTransform_.UpdateMatrix();

	ArrowRot = { 1.0f,0.0f,0.0f };

}

void LeftArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void LeftArrow::OnCollision(Player* player)
{
	//速度調整
	Vector3 speedScale = { 2.0f,0,2.0f };

	Vector3 arrowSpeed = Transform(ArrowRot, speedScale);

	//プレイヤーに速度あげる　多分入ってる
	player->SetKeepMove(arrowSpeed);

	//playerの当たり判定ここで
	player->ArrowOnCollision();
}

Vector3 LeftArrow::GetVelocity()
{
	Vector3 velocity;

	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}
