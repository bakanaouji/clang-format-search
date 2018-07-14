//==========================================================
/// @file
/// @brief    dashboard.h�̎���
/// @author   �������V
///
/// @attention  �_�b�V���{�[�h�i�_�O���t�A�X�C�b�`�Ȃǁj��\���N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B

//==========================================================

#include "dashboard.h"

//=============================================================================
// Seven Segment�R���X�g���N�^
//=============================================================================
SevenSegment::SevenSegment()
{
    mDigits = 1;	// ����
    mNumber = 0;	// �\�����鐔�l
}

//=============================================================================
// �Z�u���Z�O�����g�\���̏�����
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   digits = ����
//		   color = �����̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool SevenSegment::initialize(Graphics *graphics, TextureManager *textureM,
                   const int left, const int top, const float scale, UINT digs, const COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SEGMENT_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
        if(digs < 1)
            digs = 1;
        mDigits = digs;

        mDecimal.initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT,
                           dashboardNS::TEXTURE_COLS, textureM);
        mDecimal.setCurrentFrame(dashboardNS::DECIMAL_FRAME);
        mDecimal.setColorFilter(color);
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �Z�u���Z�O�����g�\���ɕ\�����鐔�l��ݒ�
//=============================================================================
void SevenSegment::set(const double value)
{
    mNumber = value;
}

//=============================================================================
// �Z�u���Z�O�����g�̐����u0�v�`�u9�v�Ɓu-�v��\��
//         A
//       -----
//     F|     |B
//      |  G  |
//       -----
//     E|     |C
//      |  D  |
//       -----  
//=============================================================================
void SevenSegment::drawDigit(const char n, const COLOR_ARGB color)
{
    float lowerY = mSpriteData.y + mSpriteData.height * mSpriteData.scale * 0.75f;
    float saveY = mSpriteData.y;

	// �Z�O�����gA
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(0);
        Image::draw(color);
    }
	// �Z�O�����gB
    if(n=='0' || n=='1' || n=='2' || n=='3' || n=='4' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
	// �Z�O�����gG
    if(n=='-' || n=='2' || n=='3' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// �Z�O�����gF�̏ꍇ
    if(n=='0' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(270);
        Image::draw(color);
    }

    mSpriteData.y = lowerY;  // �����̉������p��Y��ݒ�
	
	// �Z�O�����gE�̏ꍇ
    if(n=='0' || n=='2' || n=='6' || n=='8')
    {
        setDegrees(270);
        Image::draw(color);
    }
	// �Z�O�����gD�̏ꍇ
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// �Z�O�����gC�̏ꍇ
    if(n=='0' || n=='1' || n=='3' || n=='4' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
    mSpriteData.y = saveY;
}

//=============================================================================
// �����_��`��
//=============================================================================
void SevenSegment::drawDecimal(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;
    float saveY = mSpriteData.y;

    setDegrees(0);
    mSpriteData.x -= mSpriteData.width * mSpriteData.scale * 0.25f;
    mSpriteData.y += mSpriteData.height * mSpriteData.scale * 0.80f;

    mDecimal.draw(mSpriteData, color);

    mSpriteData.x = saveX;
    mSpriteData.y = saveY;
}

//=============================================================================
// �Z�u���Z�O�����g�\����`��
// number�ϐ��ɂ́A�\�����镂�������_���l���i�[����Ă���
//=============================================================================
void SevenSegment::draw(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;
    float saveY = mSpriteData.y;
    char ch;

    if(mDigits == 0)
        return;

	// ���l�𕶎���ɕϊ�
    std::stringstream strstm;
    strstm.precision(mDigits);
    strstm.flags(std::ios_base::fixed);
    strstm << mNumber;           // ���l�𕶎���ɕϊ�
    std::string str = strstm.str();

    UINT digitN = str.length(); // ������̌������擾
	// ������̌������A�Z�u���Z�O�����g�\���̌������������ꍇ
    if (digitN > mDigits)
        digitN = mDigits;

	// �ł����Ɉʒu���錅��x�ʒu
    mSpriteData.x += mSpriteData.width * mSpriteData.scale * 1.2f * (mDigits-digitN);

    UINT n=0;
    ch = str.at(n++);           // number�̍ŏ��̌����擾
    while(digitN > 0)           // �\�����錅���c���Ă���Ԃ͌p��
    {
        if(ch == '.')           // �����_�̏ꍇ
            drawDecimal(color);
        else
        {
            drawDigit(ch, color);   // ������\��
			// ���̌��̉�ʏ�̈ʒu
            mSpriteData.x += mSpriteData.width * mSpriteData.scale * 1.2f;
        }
        if(n < str.length())
            ch = str.at(n++);   // ���̌����擾
        else
            ch = '0';           // �����񂪁u.�v�ŏI����Ă���ꍇ�A����0�Ŗ��߂�
        if(ch != '.')           // �����_�łȂ��ꍇ
            digitN--;           // ����������
    }
    mSpriteData.x = saveX;
}

//=============================================================================
// Bar��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   color = �o�[�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, const int left,
                     const int top, const float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    // OK��߂�
    return true;
}

//=============================================================================
// �o�[�̃T�C�Y��ݒ�
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    mSpriteData.rect.right = mSpriteData.rect.left + (LONG)(mSpriteData.width*p/100);
}

//=============================================================================
// DialGauge��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   type = �~�Ղ̎��
//		   zeroAngle = �~�Ղ̃[���̈ʒu
//		   dialColor = �~�Ղ̐F
//		   pointColor = �����̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool DialGauge::initialize(Graphics *graphics, TextureManager *textureM, const int left,
                    const int top, const float scale, const dashboardNS::DialType type,
                    const float zeroAngle, const COLOR_ARGB dialColor, const COLOR_ARGB pointerColor)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        mDialType = type;
        switch(type)
        {
        case dashboardNS::DIAL360:
            setCurrentFrame(dashboardNS::DIAL360_FRAME); break;
        case dashboardNS::DIAL270:
            setCurrentFrame(dashboardNS::DIAL270_FRAME); break;
        case dashboardNS::DIAL180:
            setCurrentFrame(dashboardNS::DIAL180_FRAME); break;
        }
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = dialColor;
        setDegrees(zeroAngle);

        mPointer.initialize(graphics, dashboardNS::WIDTH,
                           dashboardNS::HEGIHT, dashboardNS::TEXTURE_COLS,
                           textureM);
        mPointer.setCurrentFrame(dashboardNS::POINTER_FRAME);
        mPointer.setColorFilter(pointerColor);
    }
    catch(...)
    {
        return false;
    }
    // OK��߂�
    return true;
}

//=============================================================================
// DialGauge�̎w�j��ݒ�
//=============================================================================
void DialGauge::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    switch(mDialType)
    {
    case dashboardNS::DIAL360:
        mPointer.setDegrees(p * 360/100.0f + getDegrees()); break;
    case dashboardNS::DIAL270:
        mPointer.setDegrees(p * 270/100.0f + getDegrees() - 135); break;
    case dashboardNS::DIAL180:
        mPointer.setDegrees(p * 180/100.0f + getDegrees() - 90); break;
    }
}

//=============================================================================
// DialGauge�Ǝw�j��`��
//=============================================================================
void DialGauge::draw(const COLOR_ARGB color)
{
    Image::draw(color);
    float dialAngle = mSpriteData.angle;
    mSpriteData.angle = mPointer.getRadians();
    mPointer.draw(mSpriteData, graphicsNS::FILTER);
    mSpriteData.angle = dialAngle;
}

//=============================================================================
// Light�R���X�g���N�^
//=============================================================================
Light::Light()
{
    mFlashDelay = 0;
    mFlashTimer = 0;
    mOnColor = graphicsNS::RED;
    mOffColor = SETCOLOR_ARGB(255,32,0,0);   // �_�[�N���b�h
}

//=============================================================================
// Light��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//         scale = �{���i�Y�[���j
//		   flashRate = �_��/����/�_�ł̑���
//		   �i<0�̏ꍇ�͓_���A=0�̏ꍇ�͏����A>0�̏ꍇ��1�b������̓_�ł̎��ԁj
//		   colorOn = �_�����̃��C�g�̐F
//		   colorOff = �������̃��C�g�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool Light::initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                const float scale, const float flashRate, const COLOR_ARGB colorOn, const COLOR_ARGB colorOff)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::LIGHT_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = colorOff;
        mOffColor = colorOff;
        mOnColor = colorOn;
        if(flashRate > 0)
            mFlashDelay = flashRate/2.0f;    // �f���[�e�B�[���50%
        else
            mFlashDelay = flashRate;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// flashRate��ݒ�F<0�̏ꍇ�͓_���A =0�̏ꍇ�͏����A>0�̏ꍇ�͓_�ł̎���
//=============================================================================
void Light::set(const float flashRate)
{
    if(flashRate > 0)
        mFlashDelay = flashRate/2.0f;    // �f���[�e�B�[���50%
    else
        mFlashDelay = flashRate;
}

//=============================================================================
// ���C�g���X�V
//=============================================================================
void Light::update(const float frameTime)
{
    if(mFlashDelay > 0)  // >0�̏ꍇ�A���C�g��_�ł�����
    {
        mFlashTimer += frameTime;
        if(mFlashTimer >= mFlashDelay)
        {
            mFlashTimer -= mFlashDelay;
            if(mColorFilter == mOnColor)
                mColorFilter = mOffColor;
            else
                mColorFilter = mOnColor;
        }
    }
}


//=============================================================================
//  ToggleSwitch�R���X�g���N�^
//=============================================================================
ToggleSwitch::ToggleSwitch()
{
    mSwitchOn = false;
    mMouseClick = true;
}

//=============================================================================
// �؂�ւ��X�C�b�`�iToggleSwitch�j��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   *in = Input�I�u�W�F�N�g�ւ̃|�C���^
//         hwnd = �E�B���h�E�ւ̃n���h��
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool ToggleSwitch::initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND h,
                   const int left, const int top, const float scale)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mHwnd = h;                       // �E�B���h�E�ւ̃n���h��
        mInput = in;                     // ���̓I�u�W�F�N�g
        mSwitchRect.left = left;
        mSwitchRect.top  = top;
        mSwitchRect.right = (long)(left + (dashboardNS::SWITCH_WIDTH * mSpriteData.scale));
        mSwitchRect.bottom = (long)(top + (mSpriteData.height * mSpriteData.scale));
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �X�C�b�`��ł̃}�E�X�N���b�N���`�F�b�N
//=============================================================================
void ToggleSwitch::update(const float frameTime)
{
    if (!mInitialized || !mVisible)
        return;

    // �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
    RECT clientRect;
    GetClientRect(mHwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (mInput->getMouseLButton())       // �}�E�X�̍��{�^���̏ꍇ
    {
		// �}�E�X�N���b�N���X�C�b�`�̓����Ŕ��������ꍇ
        if (mInput->getMouseX()*screenRatioX >= mSwitchRect.left &&
            mInput->getMouseX()*screenRatioX <= mSwitchRect.right &&
            mInput->getMouseY()*screenRatioY >= mSwitchRect.top &&
            mInput->getMouseY()*screenRatioY <= mSwitchRect.bottom)
        {
            if(mMouseClick)
            {
                mMouseClick = false;
                mSwitchOn = !mSwitchOn;       // �X�C�b�`��؂�ւ���
                if(mSwitchOn)
                    setCurrentFrame(dashboardNS::SWITCH_ON_FRAME);
                else
                    setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
            }
        }
    } else
        mMouseClick = true;
}

//=============================================================================
// PushButton�R���X�g���N�^
//=============================================================================
PushButton::PushButton()
{
    mSwitchOn = false;
    mMouseClick = true;
    mMomentary = true;
}

//=============================================================================
// �v�b�V���{�^���iPushButton�j��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   *in = Input�I�u�W�F�N�g�ւ̃|�C���^
//         hwnd = �E�B���h�E�ւ̃n���h��
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
//		   type = true�̏ꍇ�̓��[�����^���[�Afalse�̏ꍇ�̓I���^�l�[�g
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool PushButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND h,
                    const int left, const int top, const float scale, const bool type)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mHwnd = h;                       // �E�B���h�E�ւ̃n���h��
        mInput = in;                     // ���̓I�u�W�F�N�g
        mSwitchRect.left = left;
        mSwitchRect.top  = top;
        mSwitchRect.right = (long)(left + mSpriteData.width * mSpriteData.scale);
        mSwitchRect.bottom = (long)(top + mSpriteData.height * mSpriteData.scale);
        mMomentary = type;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// �v�b�V���{�^����ł̃}�E�X�N���b�N���`�F�b�N
//=============================================================================
void PushButton::update(const float frameTime)
{
    if (!mInitialized || !mVisible)
        return;

	// �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
    RECT clientRect;
    GetClientRect(mHwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (mInput->getMouseLButton())           // �}�E�X�̍��{�^���̏ꍇ
    {
		// �X�C�b�`�̓����Ń}�E�X���N���b�N���ꂽ�ꍇ
        if (mInput->getMouseX()*screenRatioX >= mSwitchRect.left &&
            mInput->getMouseX()*screenRatioX <= mSwitchRect.right &&
            mInput->getMouseY()*screenRatioY >= mSwitchRect.top &&
            mInput->getMouseY()*screenRatioY <= mSwitchRect.bottom)
        {
            if(mMouseClick)
            {
                mMouseClick = false;
                if(mMomentary)               // ���[�����^���[�X�C�b�`�̏ꍇ
                    mSwitchOn = true;
                else
                    mSwitchOn = !mSwitchOn;   // �X�C�b�`��؂�ւ���
                if(mSwitchOn)
                    setCurrentFrame(dashboardNS::BUTTON_DOWN_FRAME);
                else
                    setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
            }
        }
    } 
    else
    {
        mMouseClick = true;
        if(mMomentary)
        {
            mSwitchOn = false;
            setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        }
    }
}

//=============================================================================
//  BarGraph�R���X�g���N�^
//=============================================================================
BarGraph::BarGraph()
{
    mMaxBars = 1;
    mBarsOn = 0;
}

//=============================================================================
// BarGraph��������
// ���s�O�F*graphics = Graphics�I�u�W�F�N�g�ւ̃|�C���^
//		   *textureM = TextureManager�I�u�W�F�N�g�ւ̃|�C���^
//		   left�Atop = ��ʈʒu
//		   scale = �{���i�Y�[���j
//		   bars = ���[�^�[���̃o�[�̖{��
//		   color = �o�[�̐F
// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
//=============================================================================
bool BarGraph::initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const UINT bars, const COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::WIDTH, dashboardNS::HEGIHT, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_GRAPH_FRAME);
        mSpriteData.x = (float)left;
        mSpriteData.y = (float)top;
        mSpriteData.scale = scale;
        mColorFilter = color;
        if(mMaxBars > 0)
            mMaxBars = bars;
    }
    catch(...)
    {
        return false;
    }
	// OK��߂�
    return true;
}

//=============================================================================
// barsOn�̒l���A�\������o�[�̖{���ɐݒ�
// �p�����[�^p�͊����i0�`100�j
//=============================================================================
void BarGraph::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    mBarsOn = (int)(p * 0.01f * mMaxBars + 0.5);
}

//=============================================================================
// BarGraph��`��
// barsOn�́A�\������o�[�̖{�����i�[
//=============================================================================
void BarGraph::draw(const COLOR_ARGB color)
{
    float saveX = mSpriteData.x;

    for (int i=mBarsOn; i > 0; i--)
    {
		// ���̃o�[�̉�ʏ�̈ʒu
        mSpriteData.x += dashboardNS::BAR_GRAPH_WIDTH * mSpriteData.scale;
        Image::draw(color);
    }
    mSpriteData.x = saveX;
}

