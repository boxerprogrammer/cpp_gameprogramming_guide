#pragma once
class Input;
class SceneController;
/// <summary>
/// �Q�[���V�[���̊��N���X(�������z�N���X)
/// </summary>
class Scene
{
protected:
	SceneController& controller_;///������̎Q��
public:
	/// <summary>
	/// �V�[��������(�̍ۂɃV�[���R���g���[�������)
	/// </summary>
	/// <param name="controller"></param>
	Scene(SceneController& controller);
	virtual ~Scene() {}
	/// <summary>
	/// ���t���[����Ԃ��X�V����
	/// </summary>
	/// <param name="input">���̓N���X</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	/// ���t���[���`�悷��
	/// </summary>
	virtual void Draw() = 0;
};

