//==========================================================
/// @file
/// @brief    textureManager.h�̎���
/// @author   �������V
///
/// @attention  �e�N�X�`���������N���X�ł��B
///				�摜�t�@�C���̓ǂݍ��݂Ȃǂ��s���܂��B
#include "textureManager.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
TextureManager::TextureManager()
{
	mGraphics = NULL;
	mInitialized = false;            // �������ɐ��������ꍇ�́Atrue
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
TextureManager::~TextureManager()
{
	for (UINT i = 0; i < mTexture.size(); i++)
		safeReleaseTexture(mTexture[i]);
}

//=============================================================================
// �e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool TextureManager::initialize(Graphics *g, std::string file)
{
	bool success = true;
	try {
		mGraphics = g;								// graphics�I�u�W�F�N�g
		for (UINT i = 0; i < file.size(); i++)		// �������ɕϊ�
			file.at(i) = tolower(file.at(i));
		if (file.rfind(".txt") == file.size() - 4)	// .txt�g���q�̏ꍇ
		{
			// ���ꂼ��̃e�N�X�`�������܂ރt�@�C�����J��
			std::ifstream infile(file.c_str());
			if (!infile)                     // �J���̂Ɏ��s�����ꍇ
				return false;
			std::string name;
			while (getline(infile, name))
			{
				mFileNames.push_back(name);  // �t�@�C������ǉ�
				mWidth.push_back(0);         // �e�N�X�`���̕��ɗ]�����쐬
				mHeight.push_back(0);        // �e�N�X�`���̍����ɗ]�����쐬
				mTexture.push_back(NULL);    // �e�N�X�`���ɗ]�����쐬
			}
			infile.close();
		}
		else    // .txt�t�@�C���łȂ��A�P�̃e�N�X�`�������t�@�C���Ɋ܂܂�Ă���
		{
			mFileNames.push_back(file);  //�t�@�C������ǉ�
			mWidth.push_back(0);         // �e�N�X�`���̕��ɗ]�����쐬
			mHeight.push_back(0);        // �e�N�X�`���̍����ɗ]�����쐬
			mTexture.push_back(NULL);    // �e�N�X�`���ɗ]�����쐬
		}

		// �e�N�X�`���t�@�C���̓ǂݍ���
		for (UINT i = 0; i < mFileNames.size(); i++)
		{
			mHr = mGraphics->loadTexture(mFileNames[i].c_str(),
				graphicsNS::TRANSCOLOR, mWidth[i], mHeight[i], mTexture[i]);
			if (FAILED(mHr))
				success = false;    // �Œ�ł��P�̃e�N�X�`���̓ǂݍ��݂Ɏ��s
		}
	}
	catch (...) { return false; }
	mInitialized = true;                    // ����ɏ��������ꂽ�ꍇ�Atrue��ݒ�
	return success;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void TextureManager::onLostDevice()
{
	try
	{
		if (!mInitialized)
			return;
		for (UINT i = 0; i<mTexture.size(); i++)
			safeReleaseTexture(mTexture[i]);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::WARNING,
			"Warning, TextureManager onLostDevice attempted to access an invalid texture."));
	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!mInitialized)
		return;
	// load texture files
	for (UINT i = 0; i<mFileNames.size(); i++)
	{
		mHr = mGraphics->loadTexture(mFileNames[i].c_str(),
			graphicsNS::TRANSCOLOR, mWidth[i], mHeight[i], mTexture[i]);
		if (FAILED(mHr))
			safeReleaseTexture(mTexture[i]);
	}
}