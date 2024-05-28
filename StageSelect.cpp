#include "StageSelect.h"
#include <TextureManager.h>

void StageSelect::Initialize()
{
	uint32_t textureStage = TextureManager::Load("choiceArrow.png");

	sprite[0] = Sprite::Create(textureStage, position, color_, {1.0f,1.0f});

	position = { 1140.0f,210.0f };
	sprite[0]->SetPosition(position);
	
	input_->GetInstance();
}

void StageSelect::Update()
{
	if (cleckcooltime_ <= 10)
	{
		cleckcooltime_++;
	}
	if (cooltime <= 10)
	{
		cooltime++;
	}
	
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		speed += 0.001f;
		
		
			selectspeed += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed;
			
		
		if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 410 && selectspeed <= -0.001f && cooltime >= 10 ) {

			position.y += 120.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 410 && selectspeed >= 0.001f && cooltime >= 10) {
			position.y -= 200.0f;
			position.x += 140.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 210 && selectspeed <= -0.001f && cooltime >= 10) {
			position.y += 200.0f;
			position.x -= 140.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 530 && selectspeed >= 0.001f && cooltime >= 10) {
			position.y -= 120.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 530 && selectspeed <= -0.001f && cooltime >= 10) {

			position.y += 100.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else if ((float)joyState.Gamepad.sThumbLY != 0 && position.y == 630 && selectspeed >= 0.001f && cooltime >= 10) {

			position.y -= 100.0f;
			sprite[0]->SetPosition(position);
			cooltime = 0;
		}
		else
		{
			selectspeed = 0.0f;
			
		}
		if (Input::GetInstance()->GetJoystickState(0, prejoyState)) {
			if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && position.y == 210 && cleckcooltime_>=10&&
					istutorial_==false&&isstage1_==false&&isstage2_==false&&isstage3_==false) {
						{
							istutorial_ = true;
							isstage1_ = false;
							isstage2_ = false;
							isstage3_ = false;
						}
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) && position.y == 410 && cleckcooltime_ >= 10 &&
					istutorial_ == false && isstage1_ == false && isstage2_ == false && isstage3_ == false)
				{
					istutorial_ = false;
					isstage1_ = true;
					isstage2_ = false;
					isstage3_ = false;
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) && position.y == 530 && cleckcooltime_ >= 10 &&
					istutorial_ == false && isstage1_ == false && isstage2_ == false && isstage3_ == false)
				{
					istutorial_ = false;
					isstage1_ = false;
					isstage2_ = true;
					isstage3_ = false;
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && position.y == 630 && cleckcooltime_ >= 10 &&
					istutorial_ == false && isstage1_ == false && isstage2_ == false && isstage3_ == false)
				{
					istutorial_ = false;
					isstage1_ = false;
					isstage2_ = false;
					isstage3_ = true;
				}
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(prejoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A))
				{
					iscleck_ = true;
				}
				else
				{
					iscleck_ = false;
				}
			}
		}
		if (iscolor_)
		{
			color_.w-=0.02f;
			sprite[0]->SetColor(color_);
		}
		else if (iscolor_ == false)
		{
			color_.w+=0.02f;
			sprite[0]->SetColor(color_);
		}
		if (color_.w >= 1.0f)
		{
			iscolor_ = true;
		}
		else if (color_.w <= 0.0f)
		{
			iscolor_ = false;
		}
		

	}
	/*ImGui::Begin("StageSelect");
	ImGui::DragFloat2("position", &position.x, 10.0f);
	ImGui::DragInt("cooltime", &cooltime, 1);
	ImGui::DragInt("cleckcooltime", &cleckcooltime_, 1);
	ImGui::Checkbox("Stage1", &isstage1_);
	ImGui::Checkbox("Stage2", &isstage2_);
	ImGui::Checkbox("Stage3", &isstage3_);
	ImGui::Checkbox("Tutorial", &istutorial_);
	ImGui::End();*/
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

void StageSelect::Reset()
{
	istutorial_ = false;
	isstage1_ = false;
	isstage2_ = false;
	isstage3_ = false;
	cleckcooltime_ = 0;
	iscleck_ = false;
	position = { 1140.0f,210.0f };
	sprite[0]->SetPosition(position);
	color_.w = 1.0f;
	sprite[0]->SetColor(color_);
}
