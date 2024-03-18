#include "IScene.h"

int IScene::sceneNo = kTitle;

IScene::~IScene(){}

int IScene::GetSceneNo()
{
	return sceneNo;
}

void IScene::SetKeys(char* keys, char* preKeys) 
{
	keys_ = keys;
	preKeys_ = preKeys;
}