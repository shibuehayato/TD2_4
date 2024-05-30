#include "Howtoplay.h"
#include <TextureManager.h>

void Howtoplay::Initialize()
{
	
	uint32_t textureStage2 = TextureManager::Load("scene/operation.png");

	sprite[0] = Sprite::Create(textureStage2, position[0], color_, {1.0f,1.0f});
	//sprite[1] = Sprite::Create(textureStage2, position[1], color_, { 1.0f,1.0f });

	position[0] = {1280.0f,720.0f};
	//position[1] = { 2580.0f,720.0f };
	sprite[0]->SetPosition(position[0]);
	//sprite[1]->SetPosition(position[1]);

	input_->GetInstance();
}

void Howtoplay::Update()
{
	//if (cleckcooltime_ <= 10)
	//{
	//	cleckcooltime_++;
	//}
	//if (cooltime <= 10)
	//{
	//	cooltime++;
	//}

	//if (Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	speed += 0.001f;


	//	selectspeed += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed;


	//	if ((float)joyState.Gamepad.sThumbLX != 0 && position[0].x == 1280 && selectspeed <= -0.001f && cooltime >= 10) {

	//		position[0].x -= 1300.0f;
	//		sprite[0]->SetPosition(position[0]);
	//		position[1].x -= 1300.0f;
	//		sprite[1]->SetPosition(position[1]);
	//		cooltime = 0;
	//	}
	//	
	//
	//	else if ((float)joyState.Gamepad.sThumbLX != 0 && position[0].x == -20 && selectspeed >= 0.001f && cooltime >= 10) {
	//		position[0].x += 1300.0f;
	//		sprite[0]->SetPosition(position[0]);
	//		position[1].x += 1300.0f;
	//		sprite[1]->SetPosition(position[1]);
	//		cooltime = 0;
	//	}
	//	else
	//	{
	//		selectspeed = 0.0f;

	//	}
	//}

}

void Howtoplay::Draw()
{
	sprite[0]->Draw();
	//sprite[1]->Draw();
}

void Howtoplay::Reset()
{
}
