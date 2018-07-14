//==========================================================
/// @file
/// @brief    GameError�N���X
/// @author   �������V
///
/// @attention  �Q�[���G���W���ɂ���ăX���[�����Error�N���X

//==========================================================
#ifndef _GAMEERROR_H            // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _GAMEERROR_H            // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <exception>

// GameError�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace gameErrorNS
{
	// �G���[�R�[�h
	// ���̐��́A�Q�[�����V���b�g�_�E������K�v�̂���v���I�ȃG���[��\���܂��B
	// ���̐��́A�Q�[�����V���b�g�_�E������K�v�̂Ȃ��x����\���܂��B
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}

//==========================================================
// GameError�N���X�B�Q�[���G���W���ɂ���ăG���[�����m���ꂽ�Ƃ��ɃX���[����܂��B
// std::exception���p��
class GameError : public std::exception
{
private:
    int     mErrorCode;		// �G���[�R�[�h
    std::string mMessage;	// �G���[���b�Z�[�W
public:
	// �f�t�H���g�R���X�g���N�^
    GameError() throw() :mErrorCode(gameErrorNS::FATAL_ERROR), mMessage("Undefined Error in game.") {}
    // �R�s�[�R���X�g���N�^
    GameError(const GameError& e) throw(): std::exception(e), mErrorCode(e.mErrorCode), mMessage(e.mMessage) {}
    // ��������̃R���X�g���N�^
    GameError(int code, const std::string &s) throw() :mErrorCode(code), mMessage(s) {}
    // ������Z�q
    GameError& operator= (const GameError& rhs) throw() 
    {
        std::exception::operator=(rhs);
        this->mErrorCode = rhs.mErrorCode;
        this->mMessage = rhs.mMessage;
    }
	// �f�X�g���N�^
    virtual ~GameError() throw() {};

	// ��{�N���X�ɑ΂���I�[�o�[���C�h
    virtual const char* what() const throw() { return this->getMessage(); }

    const char* getMessage() const throw() { return mMessage.c_str(); }
    int getErrorCode() const throw() { return mErrorCode; }
};

#endif
