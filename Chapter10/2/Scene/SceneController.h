#pragma once

#include<memory>
#include<list>
class Scene;
class Input;
/// <summary>
/// �V�[���̐؂�ւ��֐���񋟂�
/// ���t���[���莝���̃V�[���̍X�V���s��
/// </summary>
class SceneController
{
private:
	std::list<std::shared_ptr<Scene>> scenes_;//���͂�����list�����܂�
public:
	/// <summary>
	/// ���s���ׂ��V�[����؂�ւ��܂�
	/// </summary>
	/// <param name="scene">��������s�������V�[��</param>
	void ChangeScene(std::shared_ptr<Scene> scene);
	/// <summary>
	/// �莝���̃V�[�����X�V����
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// �莝���̃V�[����`�悷��
	/// </summary>
	void Draw();

	/// <summary>
	/// �V�[����V�����u�ςށv
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// �Ō�ɒǉ������V�[�����폜����
	/// </summary>
	void PopScene();

	/// <summary>
	/// �����I�ɓ���̃V�[���ɔ�ԁB�ς�łĂ����Z�b�g�����
	/// </summary>
	/// <param name="scene">�W�����v��V�[��</param>
	void JumpScene(std::shared_ptr<Scene> scene);
};

