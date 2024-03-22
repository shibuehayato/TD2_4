#pragma once
#include <Input.h>

// �V�[���N���X����邽�тɂ����Ɏ�ނ�ǉ�
enum SceneType {
	kTitle,       // �^�C�g���V�[��
	kOperation,	  // ��������V�[��
	kStageSelect, // �X�e�[�W�I���V�[��
	kGame,		  // �Q�[���V�[��
	kClear,		  // �N���A�V�[��
	kGameOver,	  // �Q�[���I�[�o�[�V�[��
	kSceneMax	  // �V�[���̍ő吔
};

class IScene
{
protected:
	// �V�[���ԍ����Ǘ�����ϐ�
	static int sceneNo;

	// �Q�[���p�b�h�̏�Ԃ𓾂�ϐ�
	XINPUT_STATE joyState_;
	XINPUT_STATE prejoyState_;

public:
	// �p����Ŏ��������֐�
	// ���ۃN���X�Ȃ̂ŏ������z�֐��Ƃ���
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ���z�f�X�g���N�^��p�ӂ��Ȃ��ƌx�������
	virtual ~IScene();

	// �V�[���ԍ��̃Q�b�^�[
	int GetSceneNo();

	void SetKeys(XINPUT_STATE joyState, XINPUT_STATE prejoyState);
};