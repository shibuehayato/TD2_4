#include "Player.h"
#include <cassert>
#include <Input.h>
#include <Mymath.h>
#include <ImGuiManager.h>

void Player::Initialize(Model* head)
{
	assert(head);
	HeadModel_ = head;

	worldTransformHead_.translation_ = { 0,0,-15.0f };
	worldTransformHead_.Initialize();
}

void Player::Update() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		if ((float)joyState.Gamepad.sThumbLX != 0 || (float)joyState.Gamepad.sThumbLY != 0) {
			speed += 0.001f;

			KeepMove.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;
			KeepMove.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;

		}

		if ((float)joyState.Gamepad.sThumbLX == 0 && (float)joyState.Gamepad.sThumbLY == 0) {
			speed = 0;
			
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
			if (KeepMove.x == 0 || KeepMove.z==0) {
				KeepMove.x = 0;
				KeepMove.z = 0;
			}

			// 移動量
			Vector3 move = { 0,0,0 };
			move.x -= KeepMove.x;
			move.z -= KeepMove.z;
			// 座標移動
			worldTransformHead_.translation_ = Add(worldTransformHead_.translation_, move);
		}
	}

	// 行列を定数バッファに転送
	worldTransformHead_.UpdateMatrix();

	ImGui::Begin("speed");
	ImGui::DragFloat("speed", &speed);
	ImGui::DragFloat3("KeepMove", &KeepMove.x);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) { 
	// 3Dモデル描画
	HeadModel_->Draw(worldTransformHead_, viewProjection);
}