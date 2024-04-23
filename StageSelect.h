#pragma once
#include"Vector2.h"
#include"Sprite.h"
#include"Input.h"
class StageSelect
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	Vector2 position;

	Sprite* sprite[2] = { nullptr,nullptr };

	Input* input_ = nullptr;

	bool isTrriger_ = false;

	XINPUT_STATE joyState;
};

