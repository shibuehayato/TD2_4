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
	// �e�N�X�`���n���h��
	uint32_t OperationTexture_ = 0;
	// �^�C�g����ʂ̃X�v���C�g
	std::unique_ptr<Sprite> OperationSprite_ = nullptr;
};