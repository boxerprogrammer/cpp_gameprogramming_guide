#pragma once

struct Size {
	int width;//��
	int height;//����
};

/// <summary>
/// �A�v���P�[�V�����̋N���ƏI����
/// �R���g���[������V���O���g���N���X
/// </summary>
class Application
{
private:
	Application();//�R���X�g���N�^��private��
	//�R�s�[������̋֎~
	Application(const Application& app) = delete;
	void operator=(const Application& app) = delete;

	Size windowSize_ = {640,480};

public:
	~Application();//�f�X�g���N�^
	static Application& GetInstance();

	const Size& GetWindowSize()const;

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>true:���������� / false:���������s</returns>
	bool Init(int w=640, int h=480);

	/// <summary>
	/// �Q�[�����N������(���C�����[�v�����s)
	/// </summary>
	void Run();

	/// <summary>
	/// �A�v���P�[�V�����̏I������
	/// </summary>
	void Terminate();
};

