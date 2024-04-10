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
	ArrowRot_ = { 0.0f,0.0f,0.0f };
	speeddown_ = 0.01f;
	speedup_ = 0.01f;

	KeepMove_ = { 0,0,0 };
}

void Player::Update() {

	

	// ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if (isMove == false) {
			if ((float)joyState.Gamepad.sThumbLX != 0 || (float)joyState.Gamepad.sThumbLY != 0) {
				// スティックを傾けているとき、スピードが加算される	
				speed += 0.001f;

				// スピードが上がりすぎないようにする
				if (speed < 0.04f) {
					KeepMove_.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;
					KeepMove_.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;
				}
			}
		}

		if ((float)joyState.Gamepad.sThumbLX == 0 && (float)joyState.Gamepad.sThumbLY == 0) {
			speed = 0;
		
			if (speed == 0) {
				isMove = true;
			}

			if ((KeepMove_.x >= -0.020f && KeepMove_.x <= 0.020f) && (KeepMove_.z >= -0.020f && KeepMove_.z <= 0.020f)) {
				KeepMove_.x = 0;
				KeepMove_.z = 0;
				speeddown_ = 0.01f;
				e = 0.4f;
				isMove = false;
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
	move.x -= KeepMove_.x;
	move.z -= KeepMove_.z;
	if (isMove == true) {
		
		// 速度を落とす
		if (KeepMove_.x > 0) {
			KeepMove_.x -= speeddown_;
		}
		if (KeepMove_.x < 0) {
			KeepMove_.x += speeddown_;
		}
		if (KeepMove_.z > 0) {
			KeepMove_.z -= speeddown_;
		}
		if (KeepMove_.z < 0) {
			KeepMove_.z += speeddown_;
		}

	

		// 座標移動
		worldTransformHead_.translation_ = Add(worldTransformHead_.translation_, move);
	}



	// 行列を定数バッファに転送
	worldTransformHead_.UpdateMatrix();

	ImGui::Begin("speed");
	ImGui::DragInt("speed", &Oncollisiontimer_);
	ImGui::DragFloat3("tr", &worldTransformHead_.translation_.x);
	

	ImGui::DragFloat("e", &e);
	ImGui::DragFloat3("KeepMove", &KeepMove_.x,0.1f);
	ImGui::DragFloat("Position", &speeddown_, 0.1f);

	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) { 
	// 3Dモデル描画
	if (blikingtimer_<=5)
	{
		HeadModel_->Draw(worldTransformHead_, viewProjection);
	}
}

void Player::RecoveryOnCollision()
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
	if (isMove)
	{
		KeepMove_.x = -e ;
		
	}
	
}

void Player::OnCollision3()
{
	
	if (isMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove_.x = e;

	}
	
}

void Player::OnCollision4()
{
	
	if (isMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove_.z = -e;

	}

}

void Player::OnCollision5()
{
	
	if (isMove)
	{
		if (e >= 0.01f)
		{
			e -= 0.01f;
		}
		KeepMove_.z = e;

	}
	
}
//---------------------------//

void Player::OnCollision6()
{
	KeepMove_.z = e;
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

void Player::SpeedDownOnCollision()
{
	speeddown_ = 0.02f;
}

void Player::NoSpeedOnCollision()
{
	speeddown_ = 0.0f;
	speedup_ = 0.0f;
}

void Player::WarpOnCollision()
{
	worldTransformHead_.translation_= { -14.0f,0.0f,65.0f };
	//KeepMove = { 0,0,0 };
}

void Player::WarpOnCollision2()
{
	worldTransformHead_.translation_ = { 14.0f,0.0f,50.0f };
	//KeepMove = { 0,0,0 };
}

void Player::MoveStop()
{
	KeepMove_.x = 0;
	KeepMove_.z = 0;
	speed = 0;
	isMove = false;
}





void Player::WindOnCollision()
{
	//風の強さ　あとで調整
		KeepMove_.x += 0.02f;
}

void Player::PitfallOnCollision()
{
	worldTransformHead_.translation_ = { 0,0,-15.0f };
}

void Player::ArrowOnCollision()
{
	//当たった後のスピードの調整
	float Adjustment = 0.3f;

	KeepMove_ = V3FDot(ArrowRot_, Adjustment);
}