#include "Player.h"
#include <cassert>

#include <Mymath.h>
#include <ImGuiManager.h>

void Player::Initialize(Model* head)
{
	assert(head);
	HeadModel_ = head;

	worldTransformHead_.translation_ = { 0,0,-15.0f };
	worldTransformHead_.Initialize();

	e = 0.4f;
	
}

void Player::Update() {

	

	// ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if (IsMove == false) {
			if ((float)joyState.Gamepad.sThumbLX != 0 || (float)joyState.Gamepad.sThumbLY != 0) {
				// スティックを傾けているとき、スピードが加算される	
				speed += 0.001f;

				// スピードが上がりすぎないようにする
				if (speed < 0.04f) {
					KeepMove.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;
					KeepMove.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;
				}
			}
		}

		if ((float)joyState.Gamepad.sThumbLX == 0 && (float)joyState.Gamepad.sThumbLY == 0) {
			speed = 0;
		
			if (speed == 0) {
				IsMove = true;
			}

			if ((KeepMove.x >= -0.010f && KeepMove.x <= 0.010f) && (KeepMove.z >= -0.010f && KeepMove.z <= 0.010f)) {
				KeepMove.x = 0;
				KeepMove.z = 0;
				e = 0.4f;
				IsMove = false;
			}
		}
		
	}

	if (isOncollision_)
	{
		Oncollisiontimer_++;
		blikingtimer_++;
		
	}
	if (blikingtimer_ >= 10)
	{
		
		blikingtimer_ = 0;
	}

	if (Oncollisiontimer_ >= 200)
	{
		isOncollision_ = false;
		Oncollisiontimer_ = 0;
	
		blikingtimer_ = 0;
	}


	// 移動量
	Vector3 move = { 0,0,0 };
	move.x -= KeepMove.x;
	move.z -= KeepMove.z;
	if (IsMove == true) {
		
		// 速度を落とす
		if (KeepMove.x > 0) {
			KeepMove.x -= 0.01f;
		}
		if (KeepMove.x < 0) {
			KeepMove.x += 0.01f;
		}
		if (KeepMove.z > 0) {
			KeepMove.z -= 0.01f;
		}
		if (KeepMove.z < 0) {
			KeepMove.z += 0.01f;
		}

	

		// 座標移動
		worldTransformHead_.translation_ = Add(worldTransformHead_.translation_, move);
	}



	// 行列を定数バッファに転送
	worldTransformHead_.UpdateMatrix();

	ImGui::Begin("speed");
	ImGui::DragInt("speed", &Oncollisiontimer_);
	ImGui::DragFloat("e", &e);
	ImGui::DragFloat3("KeepMove", &KeepMove.x);


	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) { 
	// 3Dモデル描画
	if (blikingtimer_<=5)
	{
		HeadModel_->Draw(worldTransformHead_, viewProjection);
	}
}

void Player::OnCollision()
{
	if (worldTransformHead_.scale_.x <= 1.5f) {
		worldTransformHead_.scale_.x += 0.5f;
		worldTransformHead_.scale_.y += 0.5f;
		worldTransformHead_.scale_.z += 0.5f;
	}
}
//反射するための関数
void Player::OnCollision2()
{
	if (e >= 0.01f)
	{
		e -= 0.01f;
	}
	if (IsMove)
	{
		KeepMove.x = -e ;
		
	}
	
}

void Player::OnCollision3()
{
	
	if (IsMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove.x = e;

	}
	
}

void Player::OnCollision4()
{
	
	if (IsMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove.z = -e;

	}

}

void Player::OnCollision5()
{
	
	if (IsMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove.z = e;

	}
	
}
//---------------------------//

void Player::OnCollision6()
{
	KeepMove.z = e;
}

void Player::OnCollision7()
{
	
	if (worldTransformHead_.scale_.x >= 0.5f&&isOncollision_==false) {
		isOncollision_ = true;
	
		
		worldTransformHead_.scale_.x -= 0.5f;
		worldTransformHead_.scale_.y -= 0.5f;
		worldTransformHead_.scale_.z -= 0.5f;
	}
}




