#include "FollowCamera.h"
#include "Mymath.h"
//#include <ImGuiManager.h>


void FollowCamera::Initialize()
{
	viewProjection_.Initialize();
	viewProjection_.rotation_ = { 0.4f,0.0f,0.0f };
}

void FollowCamera::Update()
{

	// 追従対象がいれば
	if (target_) {

		/*ImGui::Begin("camera");
		ImGui::DragFloat3("translation", &offset.x);
		ImGui::DragFloat3("rotation", &rotation.x);
		ImGui::End();*/

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
		viewProjection_.rotation_ = Add(target_->rotation_, rotation);

	}

	viewProjection_.UpdateMatrix();
}