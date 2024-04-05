#include"RotatingArrow.h"
#include<cassert>
#include <Mymath.h>
#include <Player.h>

void RotatingArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 4.0f,0.0f,12.0f };

	velocity_ = { 1.0f, 1.0f, 1.0f };
	ArrowRot = { 1.0f,1.0f,1.0f };
	ArrowRotspeed = { 0.01f,0.0f,0.01f };
	input_ = Input::GetInstance();
}

void RotatingArrow::Update()
{

	worldTransform_.UpdateMatrix();

	//速度ベクトルを自機に向きに合わせて回転させる
	//velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);

	/*if (input_->TriggerKey(DIK_S)) {
		worldTransform_.rotation_.y += 0.5f;
	}*/

	//回転ギリギリ
	worldTransform_.rotation_.y += 0.05f;

	//0.5ずつ
	//0.5
	if (worldTransform_.rotation_.y >= 0.1f && worldTransform_.rotation_.y <= 0.9f) {
		ArrowRot = { 1.0f,0.0f,2.0f };
	}
	//1
	else if (worldTransform_.rotation_.y >= 0.6f && worldTransform_.rotation_.y <= 1.4f) {
		ArrowRot = { 1.4f,0.0f,1.6f };
	}
	//1.5
	else if (worldTransform_.rotation_.y >= 1.1f && worldTransform_.rotation_.y <= 1.9f) {
		ArrowRot = { 1.5f,0.0f,0.1f };
	}
	//2
	else if (worldTransform_.rotation_.y >= 1.6f && worldTransform_.rotation_.y <= 2.4f) {
		ArrowRot = { 1.9f,0.0f,-1.6f };
	}
	//2.5
	else if (worldTransform_.rotation_.y >= 2.1f && worldTransform_.rotation_.y <= 2.9f) {
		ArrowRot = { 1.1f,0.0f,-1.3f };
	}
	//3
	else if (worldTransform_.rotation_.y >= 2.6f && worldTransform_.rotation_.y <= 3.4f) {
		ArrowRot = { 0.7f,0.0f,-2.0f };
	}
	//3.5
	else if (worldTransform_.rotation_.y >= 3.1f && worldTransform_.rotation_.y <= 3.9f) {
		ArrowRot = { -1.3f,0.0f,-1.5f };
	}
	//4
	else if (worldTransform_.rotation_.y >= 3.6f && worldTransform_.rotation_.y <= 4.4f) {
		ArrowRot = { -1.4f,0.0f,-1.4f };
	}
	//4.5
	else if (worldTransform_.rotation_.y >= 4.1f && worldTransform_.rotation_.y <= 4.9f) {
		ArrowRot = { -2.0f,0.0f,-1.0f };
	}
	//5
	else if (worldTransform_.rotation_.y >= 4.6f && worldTransform_.rotation_.y <= 5.4f) {
		ArrowRot = { -1.6f,0.0f,0.9f };
	}
	//5.5
	else if (worldTransform_.rotation_.y >= 5.1f && worldTransform_.rotation_.y <= 5.9f) {
		ArrowRot = { -2.0f,0.0f,2.2f };
	}
	//6
	else if (worldTransform_.rotation_.y >= 5.6f && worldTransform_.rotation_.y <= 6.4f) {
		ArrowRot = { -1.0f,0.0f,2.0f };
	}
	//0
	else
	{
		ArrowRot = { 0.0f,0.0f,2.0f };
	}

	if (worldTransform_.rotation_.y >= 6.2f) {
		worldTransform_.rotation_.y = 0.0f;
	}

	ImGui::Begin("arrow");
	ImGui::DragFloat("arrowrotation.x", &ArrowRot.x, 0.1f);
	ImGui::DragFloat("arrowrotation.z", &ArrowRot.z, 0.1f);
	ImGui::DragFloat("worldtransform.y", &worldTransform_.rotation_.y, 0.5f);
	ImGui::End();
}

void RotatingArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}


void RotatingArrow::OnCollision(Player* player)
{
	//速度調整
	Vector3 speedScale = { 1.0f,0,1.0f };

	Vector3 arrowSpeed = Transform(ArrowRot, speedScale);

	//プレイヤーに速度あげる　多分入ってる
	player->SetKeepMove(arrowSpeed);

	//playerの当たり判定ここで
	player->ArrowOnCollision();
}