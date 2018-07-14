//==========================================================
/// @file
/// @brief    inputDialog.h�̎���
/// @author   �������V
///
/// @attention ���[�U�[�̓��͗p�̃_�C�A���O�{�b�N�X��\���N���X�ł��B

//==========================================================
#include "inputDialog.h"
//==========================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
InputDialog::InputDialog()
{
    mTextBackColor = inputDialogNS::TEXT_BACK_COLOR;
    mTextFontColor = inputDialogNS::TEXT_COLOR;
    mInTextVerts = NULL;
    mInText = "";
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
InputDialog::~InputDialog()
{
    onLostDevice();            // ���ׂẴO���t�B�b�N�A�C�e���ɂ���onLostDevice()���Ăяo��
}

//=============================================================================
// �_�C�A���O�̔w�i�ƃ{�^����`�悷�邽�߂ɒ��_�o�b�t�@������
//=============================================================================
void InputDialog::prepareVerts()
{
    MessageDialog::prepareVerts();  // ��{�N���X��prepareVerts���Ăяo��
    SAFE_RELEASE(mInTextVerts);

    // inText�̍���
    mVtx[0].x = mX + messageDialogNS::BORDER*2;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT*2.5f;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mTextBackColor;
    // inText�̉E��
    mVtx[1].x = mX + mWidth - messageDialogNS::BORDER*2;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mTextBackColor;
    // inText�E��
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mTextBackColor;
    // inText����
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mTextBackColor;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mInTextVerts);

    // inTextRect��ݒ�
    mInTextRect.left   = (long)mVtx[0].x;
    mInTextRect.right  = (long)mVtx[1].x;
    mInTextRect.top    = (long)mVtx[0].y;
    mInTextRect.bottom = (long)mVtx[2].y;
}

//=============================================================================
// InputDialog��`��
//=============================================================================
const void InputDialog::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mBorderVerts);       // ���E�̕`��
    mGraphics->drawQuad(mDialogVerts);       // �w�i�̕`��
    mGraphics->drawQuad(mButtonVerts);       // �{�^���̕`��
    mGraphics->drawQuad(mButton2Verts);      // �{�^��2�̕`��
    mGraphics->drawQuad(mInTextVerts);       // �e�L�X�g���͗̈�̕`��

    mGraphics->spriteBegin();				 // �X�v���C�g�`��J�n

    if(mText.size() == 0)
        return;
	// ���b�Z�[�W�_�C�A���O��̃e�L�X�g��\��
    mDxFont.setFontColor(mFontColor);
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK);

	// �{�^����̃e�L�X�g��`��
    mDxFont.setFontColor(mButtonFontColor);
    mDxFont.print(messageDialogNS::BUTTON1_TEXT[mButtonType],mButtonRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    mDxFont.print(messageDialogNS::BUTTON2_TEXT[mButtonType],mButton2Rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	// ���͂��ꂽ�e�L�X�g��`��
    mDxFont.setFontColor(mTextFontColor);
    mTempRect = mInTextRect;      // �ۑ�
	// DT_CALCRECT�I�v�V�������w�肳���ƃe�L�X�g�͕\������Ȃ��BRECT.right���ړ�
    mDxFont.print(mInText,mTempRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
    if(mTempRect.right > mInTextRect.right)   // �e�L�X�g����������ꍇ�A�E���𒲐�
        mDxFont.print(mInText,mInTextRect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
    else    // ����ȊO�̏ꍇ�A�����𒲐�
        mDxFont.print(mInText,mInTextRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER);

    mGraphics->spriteEnd();                  // �X�v���C�g�`��I��
}

//=============================================================================
// DIALOG_CLOSE_KEY�ƃ{�^���̃}�E�X�N���b�N���`�F�b�N
//=============================================================================
void InputDialog::update()
{
    MessageDialog::update();        // �X�[�p�[�N���X��update���Ăяo��
    if (!mInitialized || !mVisible)
    {
        if(mButtonClicked == 2)     // �L�����Z���{�^���������ꂽ��
            mInText = "";           // ���̓e�L�X�g���N���A
        return;
    }
    mInText = mInput->getTextIn();  // ���̓e�L�X�g���m��
}

//=============================================================================
// �e�L�X�gstr��InputDialog�ɕ\��
//=============================================================================
void InputDialog::print(const std::string &str)         
{
    if (!mInitialized || mVisible)  // ������������Ă��Ȃ��A�܂��͎g�p���̏ꍇ
        return;
    mText = str + "\n\n\n\n\n";		// ���̓e�L�X�g��{�^���̗]�����c��

	// �_�C�A���O�̃e�L�X�g�̈�p��textRect���Z�b�g
    mTextRect.left   = (long)(mX + messageDialogNS::MARGIN);
    mTextRect.right  = (long)(mX + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    mTextRect.top    = (long)(mY + messageDialogNS::MARGIN);
    mTextRect.bottom = (long)(mY + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// �e�L�X�g�ɕK�v�Ƃ���鍂����textRect.bottom���Z�b�g
	// DT_CALCRECT�I�v�V�������w�肳���ƃe�L�X�g�͕\������Ȃ��B
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    mHeight = mTextRect.bottom - (int)mY + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // ���_�o�b�t�@������
    mInText = "";                   // �Â����͂��N���A
    mInput->clearTextIn();
    mButtonClicked = 0;             // buttonClicked���N���A
    mVisible = true;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
//=============================================================================
void InputDialog::onLostDevice()
{
    if (!mInitialized)
        return;
    MessageDialog::onLostDevice();
    SAFE_RELEASE(mInTextVerts);
}

