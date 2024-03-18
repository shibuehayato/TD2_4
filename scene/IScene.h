#pragma once

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
	char* keys_;
	char* preKeys_;
public:
	// �p����Ŏ��������֐�
	// ���ۃN���X�Ȃ̂ŏ������z�֐��Ƃ���
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ���z�f�X�g���N�^��p�ӂ��Ȃ��ƌx�������
	virtual ~IScene();

	// �V�[���ԍ��̃Q�b�^�[
};