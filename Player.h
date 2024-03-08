#include "model.h"
#include "WorldTransform.h"

class Player {
public:

	void Initialize(Model* head);
	void Update();
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransformHead_;
	// モデル
	Model* HeadModel_ = nullptr;
	// 速さ
	float speed = 0;
	// 移動量を保管する
	Vector3 KeepMove = { 0,0,0 };
};