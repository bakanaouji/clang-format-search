//==========================================================
/// @file
/// @brief    console.h�̎���
/// @author   �������V
///
/// @attention  �f�o�b�O�p�̃R���\�[����\���N���X�ł��B
///				�u`�v�ŕ\������A����̃R�}���h��ł��ƂŃf�o�b�O���\�ł��B

//==========================================================

#include "console.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Console::Console()
{
    mInitialized = false;                // ����ɏ��������ꂽ�ꍇ�Atrue��ݒ�
    mGraphics = NULL;
    mVisible = false;                    // ��\���ɂ���
    mFontColor = consoleNS::FONT_COLOR;
    mBackColor = consoleNS::BACK_COLOR;

    mX = consoleNS::X;                   // �ŏ��̃R���\�[���ʒu
    mY = consoleNS::Y;

    mTextRect.bottom = consoleNS::Y + consoleNS::HEIGHT - consoleNS::MARGIN;
    mTextRect.left = consoleNS::X + consoleNS::MARGIN;
    mTextRect.right = consoleNS::X + consoleNS::WIDTH - consoleNS::MARGIN;
    mTextRect.top = consoleNS::Y + consoleNS::MARGIN;

    mVertexBuffer = NULL;

    mRows = 0;
    mScrollAmount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Console::~Console()
{
    onLostDevice();
}

//=============================================================================
// �R���\�[����������
//=============================================================================
bool Console::initialize(Graphics *g, Input *in)
{
    try {
        mGraphics = g;                    // �O���t�B�b�N�V�X�e��
        mInput = in;

        // ����
        mVtx[0].x = mX;
        mVtx[0].y = mY;
        mVtx[0].z = 0.0f;
        mVtx[0].rhw = 1.0f;
        mVtx[0].color = mBackColor;

        // �E��
        mVtx[1].x = mX + consoleNS::WIDTH;
        mVtx[1].y = mY;
        mVtx[1].z = 0.0f;
        mVtx[1].rhw = 1.0f;
        mVtx[1].color = mBackColor;

        // �E��
        mVtx[2].x = mX + consoleNS::WIDTH;
        mVtx[2].y = mY + consoleNS::HEIGHT;
        mVtx[2].z = 0.0f;
        mVtx[2].rhw = 1.0f;
        mVtx[2].color = mBackColor;

        // ����
        mVtx[3].x = mX;
        mVtx[3].y = mY + consoleNS::HEIGHT;
        mVtx[3].z = 0.0f;
        mVtx[3].rhw = 1.0f;
        mVtx[3].color = mBackColor;

        mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

        // DirectX�t�H���g��������
        if(mDxFont.initialize(mGraphics, consoleNS::FONT_HEIGHT, false,
                             false, consoleNS::FONT) == false)
            return false;      // ���s�̏ꍇ
        mDxFont.setFontColor(mFontColor);

    } catch(...) {
        return false;
    }

    mInitialized = true;
    return true;
}

//=============================================================================
// �R���\�[����`��
// ���s�O�FBeginScene��EndScene�̓����ɂ���
//=============================================================================
const void Console::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mVertexBuffer);       // �w�i��`��
    if(mText.size() == 0)
        return;

    mGraphics->spriteBegin();                // �X�v���C�g�̕`����J�n

    // �R���\�[���Ƀe�L�X�g��\��
    mTextRect.left = 0;
    mTextRect.top = 0;

	// textRect�̉��[��1�s�̍����ɐݒ�
    mDxFont.print("|",mTextRect,DT_CALCRECT); // �u|�v���A�S�����g�������Ƃ��Ďg�p
    int rowHeight = mTextRect.bottom + 2;    // 1�s�̍����i+2�͍s�ԃX�y�[�X�j
    if(rowHeight <= 0)                      // �����true�ɂ͂Ȃ�Ȃ��Ƒz�肳���
        rowHeight = 20;                     // �@�\����l�������I�ɐݒ�

	// �R���\�[����ɕ\���\�ȍs��
    mRows = (consoleNS::HEIGHT - 2*consoleNS::MARGIN) / rowHeight;
    mRows -= 2;                              // �����ɁA���̓v�����v�g�p�̃X�y�[�X���󂯂�
    if (mRows <= 0)                          // �����true�ɂ͂Ȃ�Ȃ��Ƒz�肳���
        mRows = 5;                           // �@�\����l�������I�ɐݒ�

	// 1�s�̃e�L�X�g�\��rect��ݒ�
    mTextRect.left = (long)(mX + consoleNS::MARGIN);
    mTextRect.right = (long)(mTextRect.right + consoleNS::WIDTH - consoleNS::MARGIN);
    // -2*rowHeight�́A���̓v�����v�g�p�̃X�y�[�X
    mTextRect.bottom = (long)(mY + consoleNS::HEIGHT - 2*consoleNS::MARGIN - 2*rowHeight);
    // ���ׂĂ̍s�i�ő��text.size())���������֏���
    for(int r=mScrollAmount; r<mRows+mScrollAmount && r<(int)(mText.size()); r++)
    {
		// ���̍s�̃e�L�X�g�\��rect�̏�[��ݒ�
        mTextRect.top = mTextRect.bottom - rowHeight; 
        // �e�L�X�g��1�s��\��
        mDxFont.print(mText[r],mTextRect,DT_LEFT);     
		// �e�L�X�g�\��rect�̉��[�����̍s�p�ɒ���
        mTextRect.bottom -= rowHeight;               
    }

	// �R�}���h�p�v�����v�g�ƁA���݂̃R�}���h�������\��
	// �v�����v�g�p�̃e�L�X�g�\��rect��ݒ�
    mTextRect.bottom = (long)(mY + consoleNS::HEIGHT - consoleNS::MARGIN);
    mTextRect.top = mTextRect.bottom - rowHeight;
    std::string prompt = ">";                   // �v�����v�g��������쐬
    prompt += mInput->getTextIn();
    mDxFont.print(prompt,mTextRect,DT_LEFT);      // �v�����v�g�ƃR�}���h��\��

    mGraphics->spriteEnd();                      // �X�v���C�g�̕`����I��
}

//=============================================================================
// �R���\�[���̕\���Ɣ�\��
//=============================================================================
void Console::showHide() 
{
    if (!mInitialized)
        return;
    mVisible = !mVisible;
    mInput->clear(inputNS::KEYS_PRESSED|inputNS::TEXT_IN);    // �Â����͂�����
}            


//=============================================================================
// �R���\�[���Ƀe�L�X�g��ǉ�
// str�̐擪�s�̃e�L�X�g�̂ݕ\�������
//=============================================================================
void Console::print(const std::string &str)     // �R���\�[���Ƀe�L�X�g��ǉ�
{
    if (!mInitialized)
        return;
    mText.push_front(str);                       // str���A�f�b�Ntext�ɒǉ�
    if(mText.size() > consoleNS::MAX_LINES)
        mText.pop_back();                        // �ł��Â��s���폜
}

//=============================================================================
// �R���\�[���R�}���h��߂�
// �V���O���L�[�̃R���\�[���R�}���h����������
// ���̑��̃R�}���h�͂��ׂăQ�[���ɖ߂�
//=============================================================================
std::string Console::getCommand()
{
	// �R���\�[��������������Ă��Ȃ��ꍇ�A�܂��͕\������Ă��Ȃ��ꍇ
    if (!mInitialized || !mVisible)               
        return "";

	// �R���\�[���L�[���`�F�b�N
    if (mInput->wasKeyPressed(CONSOLE_KEY))
        hide();                                 // �R���\�[�����\��

	// Esc�L�[���`�F�b�N
    if (mInput->wasKeyPressed(ESC_KEY))
        return "";

	// �X�N���[�����`�F�b�N
    if (mInput->wasKeyPressed(VK_UP))            // ������L�[
        mScrollAmount++;
    else if (mInput->wasKeyPressed(VK_DOWN))     // �������L�[
        mScrollAmount--;
    else if (mInput->wasKeyPressed(VK_PRIOR))    // Page Up�L�[
        mScrollAmount += mRows;
    else if (mInput->wasKeyPressed(VK_NEXT))     // Page Down�L�[
        mScrollAmount -= mRows;
    if (mScrollAmount < 0)
        mScrollAmount = 0;
    if (mScrollAmount > consoleNS::MAX_LINES-1)
        mScrollAmount = consoleNS::MAX_LINES-1;
    if (mScrollAmount > (int)(mText.size())-1)
        mScrollAmount = (int)(mText.size())-1;

    mCommandStr = mInput->getTextIn();            // ���[�U�[�����͂����e�L�X�g���擾
    // �L�[���Q�[���ɓn���Ȃ�
    mInput->clear(inputNS::KEYS_DOWN|inputNS::KEYS_PRESSED|inputNS::MOUSE);

    if (mCommandStr.length() == 0)               // �R�}���h�����͂���Ă��Ȃ��ꍇ
        return "";
	// Enter�L�[��������Ă��Ȃ��ꍇ
    if (mCommandStr.at(mCommandStr.length()-1) != '\r')
        return "";                              // �߂�i�R�}���h�łȂ��j

	// �R�}���h�̖�������u\r�v���폜
    mCommandStr.erase(mCommandStr.length()-1);
    mInput->clearTextIn();						// ���͍s���N���A
    mInputStr = mCommandStr;
    return mCommandStr;                          // �R�}���h��߂�
}


//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void Console::onLostDevice()
{
    if (!mInitialized)
        return;
    mDxFont.onLostDevice();
    SAFE_RELEASE(mVertexBuffer);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void Console::onResetDevice()
{
    if (!mInitialized)
        return;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);
    mDxFont.onResetDevice();
}

