#include "StageSelect.h"
#include "TextureManager.h"

void StageSelect::Initialize()
{
	StageSelectTexture_ = TextureManager::Load("scene/stageSelect.png");
	StageSelectSprite_ = std::make_unique<Sprite>();
	StageSelectSprite_.reset(Sprite::Create(StageSelectTexture_, { 0,0 }));
}

void StageSelect::Update()
{
	// ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState_)) {
		if (Input::GetInstance()->GetJoystickStatePrevious(0, prejoyState_)) {
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(prejoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				sceneNo = kGame;
			}
		}
	}
}

void StageSelect::Draw()
{
	StageSelectSprite_->Draw();
}