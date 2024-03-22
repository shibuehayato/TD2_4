#pragma once
#include "IScene.h"
#include "Sprite.h"
#include <memory>

class StageSelect : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	// テクスチャハンドル
	uint32_t StageSelectTexture_ = 0;
	// タイトル画面のスプライト
	std::unique_ptr<Sprite> StageSelectSprite_ = nullptr;
};