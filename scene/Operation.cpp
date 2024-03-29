#include "Operation.h"
#include "TextureManager.h"

void Operation::Initialize()
{
	OperationTexture_ = TextureManager::Load("scene/operation.png");
	OperationSprite_ = std::make_unique<Sprite>();
	OperationSprite_.reset(Sprite::Create(OperationTexture_, { 0,0 }));
}

void Operation::Update()
{
	// ゲームパッドが有効の場合if文が通る
	/*if (Input::GetInstance()->GetJoystickState(0, *joyState_)) {
		if (Input::GetInstance()->GetJoystickStatePrevious(0, *prejoyState_)) {
			if (joyState_->Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(prejoyState_->Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				sceneNo = kStageSelect;
			}
		}
	}*/

	if (keys_[DIK_SPACE] && !preKeys_[DIK_SPACE]) {
		sceneNo = kStageSelect;
	}

}

void Operation::Draw()
{
	OperationSprite_->Draw();
}