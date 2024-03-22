#pragma once
#include "IScene.h"
#include "Sprite.h"
#include <memory>

class Operation : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	// テクスチャハンドル
	uint32_t OperationTexture_ = 0;
	// タイトル画面のスプライト
	std::unique_ptr<Sprite> OperationSprite_ = nullptr;
};