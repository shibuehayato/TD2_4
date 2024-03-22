#include "IScene.h"

int IScene::sceneNo = kTitle;

IScene::~IScene(){}

int IScene::GetSceneNo()
{
	return sceneNo;
}

void IScene::SetKeys(XINPUT_STATE joyState, XINPUT_STATE prejoyState)
{
	joyState_ = joyState;
	prejoyState_ = prejoyState;
}