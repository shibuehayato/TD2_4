#include "StageSelect.h"
#include <TextureManager.h>

void StageSelect::Initialize()
{
	uint32_t textureStage = TextureManager::Load("uvChecker.png");

	sprite[0] = Sprite::Create(textureStage, position, {1.0f,1.0f,1.0f,1.0f}, {1.0f,1.0f});

	position = { 640.0f,360.0f };
	sprite[0]->SetPosition(position);

	input_->GetInstance();
}

void StageSelect::Update()
{
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if ((float)joyState.Gamepad.sThumbLX != 0 ) {

			position.x += 100.0f;
			sprite[0]->SetPosition(position);
		}
		if ((float)joyState.Gamepad.sThumbLX == XINPUT_GAMEPAD_DPAD_RIGHT) {
			position.x -= 100.0f;
			sprite[0]->SetPosition(position);
		}
	}
	/*if (isTrriger_ == true)
	{
		position.x = 800.0f;
		sprite[0]->SetPosition(position);
		isTrriger_ = false;
	}*/
}

void StageSelect::Draw()
{
	sprite[0]->Draw();
}
