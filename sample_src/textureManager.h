//==========================================================
/// @file
/// @brief    TextureManager�N���X
/// @author   �������V
///
/// @attention  �e�N�X�`���������N���X�ł��B
///				�摜�t�@�C���̓ǂݍ��݂Ȃǂ��s���܂��B

//==========================================================
#ifndef _TEXTUREMANAGER_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TEXTUREMANAGER_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <vector>
#include <string>
#include <fstream>
#include "graphics.h"
#include "constants.h"

//==========================================================
// �e�N�X�`���������N���X�ł��B
// �摜�t�@�C���̓ǂݍ��݂Ȃǂ��s���܂��B
class TextureManager
{
private:
	// TextureManager �v���p�e�B
	std::vector<UINT>       mWidth;			// �e�N�X�`���̕��i�s�N�Z���P�ʁj
	std::vector<UINT>       mHeight;		// �e�N�X�`���̍����i�s�N�Z���P�ʁj
	std::vector<LP_TEXTURE> mTexture;		// �e�N�X�`���ւ̃|�C���^
	std::vector<std::string> mFileNames;	// �e�N�X�`���̃t�@�C����
	Graphics *mGraphics;					// graphics�ւ̃|�C���^
	bool mInitialized;						// �������ɐ��������ꍇ�Atrue
	HRESULT mHr;							// �W���̖߂�l
public:
	// �R���X�g���N�^
	TextureManager();

	// �f�X�g���N�^
	virtual ~TextureManager();

	// �e�N�X�`���ւ̃|�C���^��߂�
	LP_TEXTURE getTexture(UINT n = 0) const {
		if (n >= mTexture.size())
			return NULL;
		return mTexture[n];
	}

	// �e�N�X�`���̕���߂�
	UINT getWidth(UINT n = 0) const
	{
		if (n >= mTexture.size())
			return 0;
		return mWidth[n];
	}

	// �e�N�X�`���̍�����߂�
	UINT getHeight(UINT n = 0) const
	{
		if (n >= mTexture.size())
			return 0;
		return mHeight[n];
	}

	// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
    virtual bool initialize(Graphics *g, std::string file);

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
	virtual void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
	virtual void onResetDevice();

	// texture�����S�ɉ��
	void safeReleaseTexture(LP_TEXTURE& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = NULL;
		}
	}
};
#endif // !_TEXTUREMANAGER_H
