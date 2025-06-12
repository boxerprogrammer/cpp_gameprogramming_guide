#pragma once
#include<memory>
enum class CollisionType {
	none,
	circle,
	box,
	capsule,
};

enum class RigidType {
	none,//��������Ȃ�
	movable,//������
	unmovable//�����Ȃ�
};

class Actor;//������̌^

//�����蔻��̊��N���X
class Collider
{
protected:
	Actor* owner_;//������̎Q��
	bool isRigit_ = false;//�����o��
	bool isDead_ = false;//Collider�𖳌��ɂ���
	RigidType rigidType_ = RigidType::none;
public:
	explicit Collider(Actor* owner);
	RigidType GetRigidType()const;
	bool IsDead()const { return isDead_; }
	void Kill() { isDead_ = true; }
	Actor* GetOwner();
	/// <summary>
	/// �����̓����蔻��^�C�v(Box,Circle,Capsule)
	/// </summary>
	/// <returns>�����蔻��^�C�v</returns>
	virtual CollisionType GetType()const=0;
	/// <summary>
	/// �R���W�����̍��W��񓙂��X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �����������ɔ�������C�x���g�Ƃ��Ē�`
	/// </summary>
	/// <param name="target">�������������Collider</param>
	virtual void OnHit(std::shared_ptr<Collider> target)=0;
	
};

