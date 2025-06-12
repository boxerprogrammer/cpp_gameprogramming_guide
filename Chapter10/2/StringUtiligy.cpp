#include "StringUtiligy.h"
#include<Windows.h>
//
std::wstring StringUtiligy::GetWStringFromString(const std::string& str)
{
    //���̊֐��𐳂����g���ɂ�2��Ăяo���K�v������
    std::wstring ret;
    //���ڂ̌Ăяo���͕Ԃ���镶����̕�����
    auto length = MultiByteToWideChar(CP_ACP,//���݂̃R�[�h�y�[�W
                        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,//�G���b�������Ԃ�
                        str.c_str(),//���̕�����ւ̃|�C���^
                        str.length(),//���̕�����̒���
                        nullptr,
                        0);//�Ō�̈�����nullptr,0�ɂ��邱�Ƃ�
                            //wstring�ɕK�v�ȕ����񐔂�Ԃ��Ă���
    if (length <= 0) {
        return L"";
    }
    ret.resize(length);//�K�v�ȃ��������m��

    //���ڂ̌Ăяo���͎��ۂ̕�����𓾂�
    MultiByteToWideChar(CP_ACP,
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
        str.c_str(),
        str.length(),
        ret.data(),//����ꂽ���C�h�����������A�h���X
        ret.size());//����ꂽ���C�h����������郁�����T�C�Y
    return ret;
}
