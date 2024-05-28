#pragma once
#include"Vector2.h"
#include"Sprite.h"
#include"Input.h"
#include"ImGuiManager.h"
class Howtoplay
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Reset();

	//それぞれのギミックをステージごとに位置を変えるために必要なフラグの関数
	bool IsTutorial() { return istutorial_; }
	bool IsStage1() { return isstage1_; }
	bool IsStage2() { return isstage2_; }
	bool IsStage3() { return isstage3_; }
	bool IsCleck() { return iscleck_; }

private:
	Vector2 position[2];

	Sprite* sprite[2] = { nullptr,nullptr };

	Input* input_ = nullptr;

	bool isTrriger_ = false;

	float selectspeed = 0.0f;

	int32_t cooltime = 0;
	int32_t cleckcooltime_ = 0;

	float speed = 0.0f;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	bool iscolor_ = false;

	//ステージを分けるためのフラグ
	bool istutorial_ = false;
	bool isstage1_ = false;
	bool isstage2_ = false;
	bool isstage3_ = false;

	//ボタンを押したか確認するフラグ
	bool iscleck_ = false;

	XINPUT_STATE joyState;
	XINPUT_STATE prejoyState;
};

