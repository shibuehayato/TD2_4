#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Initialize()
{
	TitleTexture_ = TextureManager::Load("scene/title.png");
	TitleSprite_ = std::make_unique<Sprite>();
	TitleSprite_.reset(Sprite::Create(TitleTexture_, { 0, 0 }));

	input_ = Input::GetInstance();
	input_->Initialize();
}

void TitleScene::Update()
{
	// ゲームパッドが有効の場合if文が通る
	/*if (Input::GetInstance()->GetJoystickState(0, *joyState_)) {
		if (Input::GetInstance()->GetJoystickStatePrevious(0, *prejoyState_)) {
			if (joyState_->Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(prejoyState_->Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				sceneNo = kOperation;
			}
		}
	}*/

	if (input_->PushKey(DIK_SPACE)) {
		sceneNo = kOperation;
	}

}

void TitleScene::Draw()
{
	TitleSprite_->Draw();
}