#pragma once
#include "IScene.h"
#include "Sprite.h"
#include <memory>

class TitleScene : public IScene
{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

private:
    // テクスチャハンドル
    uint32_t TitleTexture_ = 0;
    // タイトル画面のスプライト
    std::unique_ptr<Sprite> TitleSprite_ = nullptr;
};