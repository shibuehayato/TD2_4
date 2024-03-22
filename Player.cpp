#include "Player.h"
#include <cassert>
#include <Input.h>
#include <Mymath.h>
#include <ImGuiManager.h>

void Player::Initialize(Model* head)
{
	assert(head);
	HeadModel_ = head;

	worldTransformHead_.translation_ = { -0.5f,0,-15.0f };
	worldTransformHead_.Initialize();
}

void Player::Update() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

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
				IsMove = false;
			}
		}
	}

	// 移動量
	move = { 0,0,0 };
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
	ImGui::DragFloat("speed", &speed);
	ImGui::DragFloat3("KeepMove", &KeepMove.x);
	ImGui::Checkbox("IsMove", &IsMove);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) { 
	// 3Dモデル描画
	HeadModel_->Draw(worldTransformHead_, viewProjection);
}

void Player::WallOnCollision()
{
	
	KeepMove.x *= -1;
	KeepMove.z *= -1;
	/*move.x *= -1;
	move.z *= -1;
	speed *= -1;*/

	//当たっては、いる
	//worldTransformHead_.translation_ = { 0,0,0 };
}
