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

	velocity_ = { 1.0f, 1.0f, 1.0 };
}

void RotatingArrow::Update()
{

	worldTransform_.UpdateMatrix();

	//速度ベクトルを自機に向きに合わせて回転させる
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);
	
	worldTransform_.rotation_.y += 0.01f;

}

void RotatingArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

//void RotatingArrow::OnCollision()
//{
//	Player* player_ = new Player();
//	
//
//	player_->SetKeepMove(worldTransform_.rotation_);
//
//}

void RotatingArrow::OnCollision(Player* player)
{
	//矢印の向きに基づいて速度ベクトルを計算
	Vector3 arrowDirection = worldTransform_.rotation_;

	//速度調整
	Vector3 speedScale = {1.0f,0,1.0f};

	Vector3 arrowSpeed = Transform(arrowDirection , speedScale);

	//プレイヤーに速度あげる　多分入ってる
	player->SetKeepMove(arrowSpeed);

	//playerの当たり判定ここで
	player->ArrowOnCollision();
}
