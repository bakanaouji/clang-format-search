//==========================================================
/// @file
/// @brief    text.h�̎���
/// @author   �������V
///
/// @attention  �X�v���C�g�e�L�X�g��`�悷��N���X�ł��B


#include "text.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Text::Text() : Image()
{
	mFile = NULL;                        // �t�H���g�̃e�N�X�`���t�@�C����
	mGraphics = NULL;                    // �O���t�B�b�N�V�X�e���ւ̃|�C���^
	mColor = graphicsNS::WHITE;          // �f�t�H���g�͔�����
	mBackColor = graphicsNS::TRANSCOLOR; // �f�t�H���g�͓����F
	mAlign = textNS::LEFT;
	mWidth = textNS::GRID_WIDTH - 3;     // �����ȋ��E�̂��߂�-2�A��؂���̂��߂�-1
	mHeight = textNS::GRID_HEIGHT - 3;
	mFontHeight = textNS::FONT_HEIGHT;
	mSpriteData.width = 0;
	mSpriteData.height = 0;
	mSpriteData.rect.bottom = 1;         // �摜�̈ꕔ��I�����邽�߂̎l�p�`
	mSpriteData.rect.right = 1;
	for (int row = 0; row < textNS::ROWS; row++)         // �t�H���g�摜�̊e�s�̕����ɂ���
	{
		for (int col = 0; col < textNS::COLUMNS; col++)  // �t�H���g�摜�̊e��̕����ɂ���
		{
			mFontData[row][col].left = MAXINT;
			mFontData[row][col].right = 0;
		}
	}
	mProportional = false;
	mProportionalSpacing = textNS::PROPORTIONAL_SPACING;
	mUnderline = false;
	mBold = false;
	mTabSize = textNS::TAB_SIZE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Text::~Text()
{
	Image::~Image();
}

//=============================================================================
// Text��������
// �t�H���g�摜���̊e�����̍��[�ƉE�[�����
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// fontData�z��́A�e�����ƍ��[�ƉE�[���i�[
//=============================================================================
bool Text::initialize(Graphics *g, const char *file)
{
	try {
		mGraphics = g;           // grpahics�I�u�W�F�N�g�ւ̃|�C���^

		//-------------------------------------------------------------
		// �t�H���g�e�N�X�`����ǂݍ���ŁA�e�����̐��m�Ȉʒu��
		// ���肷�邽�߂ɁA�s�N�Z���P�ʂŌ���
		//-------------------------------------------------------------
		// �t�H���g�e�N�X�`�����A���b�N�\�ȃV�X�e���������ɓǂݍ���
		UINT w, h;
		HRESULT result = mGraphics->loadTextureSystemMem(file, graphicsNS::TRANSCOLOR, w, h, mTextureData);
		if (FAILED(result))
		{
			safeRelease(mTextureData);
			return false;
		}

		// textureData.width��textureData.height�̓t�H���g�e�N�X�`��
		// �S�̂̃T�C�Y���i�[
		// �e������1�s�N�Z�����̘g������
		// ROWS * COLS�̕��������݂���
		// �t�H���g�e�N�X�`�������b�N
		// �i�s�N�Z���f�[�^�ɃA�N�Z�X���邽�߂ɕK�v�j
		D3DLOCKED_RECT rect;
		result = mTextureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
		if (FAILED(result))                          // ���b�N�����s�����ꍇ
		{
			safeRelease(mTextureData);
			return false;
		}

		// �t�H���g���̕����̊e�s������
		for (DWORD row = 0; row<textNS::ROWS; row++)
		{
			// �t�H���g���̕����̊e�������
			for (DWORD col = 0; col<textNS::COLUMNS; col++)
			{
				mFontData[row][col].left = MAXINT;    // fontData��������
				mFontData[row][col].right = 0;

				// ������1�s�N�Z��������
				// for y = ��[�s�N�Z��; y <= ���[�s�N�Z��; y++
				for (DWORD y = row*textNS::GRID_HEIGHT + 1; y<(row + 1)*textNS::GRID_HEIGHT - 1; y++)
				{
					// �e�N�X�`�����̂��̃X�L�������C���̐擪�ւ̃|�C���^���擾
					DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
					// ���̍s�̃s�N�Z��������
					for (DWORD x = col*textNS::GRID_WIDTH + 1; x<(col + 1)*textNS::GRID_WIDTH - 1; x++)
					{
						// ���̃s�N�Z�����擾
						DWORD dwPixel = pBits[x];

						// �A���t�@�������łȂ��ꍇ
						if ((dwPixel & 0xff000000) != 0x00)
						{
							// ���̃s�N�Z���̂ق������ɂ���ꍇ
							if (x < mFontData[row][col].left)
								// �����̍��[�Ƃ��ĕۑ�
								mFontData[row][col].left = x;    
							// ���̃s�N�Z���̂ق����E�ɂ���ꍇ
							if (x > mFontData[row][col].right)
								// �����̉E�[�Ƃ��ĕۑ�
								mFontData[row][col].right = x;
						}
					}
				}
			}
		}

		// �e�N�X�`���̏���������������A���b�N������
		mTextureData->UnlockRect(0);

		// �P�Ƀt�H���g�̃X�y�[�X���擾���邽�߂ɕK�v�������̂ŁA
		// ���̃t�H���g�e�N�X�`�������
		safeRelease(mTextureData);

		//-------------------------------------------------------------
		// �t�H���g�摜���g�p���邽�߂Ƀe�N�X�`���}�l�[�W���[�ɓǂݍ���
		//-------------------------------------------------------------
		if (!mFontTexture.initialize(mGraphics, file))
			return false;                   // �t�H���g�e�N�X�`���̓ǂݍ��݂��G���[�̏ꍇ
		// �t�H���g�摜������
		if (!Image::initialize(mGraphics, textNS::FONT_WIDTH, textNS::FONT_HEIGHT, 0, &mFontTexture))
			return false;                   // ���s�̏ꍇ
	}
	catch (...)
	{
		return false;
	}
	return true;                    // �����̏ꍇ
}

//=============================================================================
// setXY
// �\������ʒu�iXY���W�j���Z�b�g
//=============================================================================
void Text::setXY(int x2, int y2)
{
	if (x2 < 0 || y2 < 0)
		return;
	mSpriteData.x = (float)x2;
	mSpriteData.y = (float)y2;
}

//=============================================================================
// �������X�AY�ɏo��
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
void Text::print(const std::string &str)
{
	print(str, (int)mSpriteData.x, (int)mSpriteData.y);
}

//=============================================================================
// �������X�AY�ɏo��
// �A���C�������g�͈ȉ��̒ʂ�F
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y, textNS::Alignment al)
{
	mAlign = al;             // �A���C�������g��ۑ�
	print(str, x, y);
	mAlign = textNS::LEFT;
}

//=============================================================================
// �������X�AY�ɏo��
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y)
{
	UCHAR ch = 0, chN = 0;
	std::string str2;
	mWidth = textNS::FONT_WIDTH;
	int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*mSpriteData.scale);
	float saveY = 0;
	int tabX = 0, tabW = 0;

	mSpriteData.x = (float)x;
	mSpriteData.y = (float)y;
	doAlign(str);

	for (UINT i = 0; i<str.length(); i++)
	{
		ch = str.at(i);
		// �\���\�ȕ����̏ꍇ
		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) 
		{
			chN = ch - textNS::MIN_CHAR;                // MIN_CHAR�̈ʒu���C���f�b�N�X0
			mSpriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
			mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::FONT_HEIGHT;
			if (mProportional)
			{
				mSpriteData.rect.left = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].left;
				// DirectX�͉E�[ + 1��K�v�Ƃ���
				mSpriteData.rect.right = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
				mWidth = mSpriteData.rect.right - mSpriteData.rect.left;
				// �S�����g�������̏ꍇ�A�X�y�[�X�͋󂯂Ȃ�
				if (mWidth >= textNS::FONT_WIDTH) 
				{
					mWidth = textNS::FONT_WIDTH;         // ���𐧌�
					mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
					mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
				}
				else    // �S�����g�������łȂ��ꍇ�A�����ԂɃX�y�[�X���󂯂�
					mWidth += mProportionalSpacing;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				drawChar(ch);
			}
			else    // �Œ�s�b�`
			{
				mWidth = textNS::FONT_WIDTH;
				mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
				drawChar(ch);
			}
			mSpriteData.x += scaledWidth;
		}
		else    // �\���s�ȕ���
		{
			switch (ch)
			{
			case ' ':                            // �X�y�[�X
				if (mProportional)
				{
					mWidth = textNS::FONT_WIDTH / 2;
					scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				}
				drawChar(' ');
				mSpriteData.x += scaledWidth;
				break;
				// �j���[���C���͉���1�s�i�݁A���[���A
				// ��ʂ̍��[�łȂ��AX�̊J�n�ʒu�ɐݒ�
			case '\n':                            // �j���[���C��
				mSpriteData.x = (float)x;
				mSpriteData.y += static_cast<int>(mHeight*mSpriteData.scale);
				saveY = mSpriteData.y;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				mSpriteData.y = saveY;
				break;
			case '\r':                            // X�̊J�n�ʒu�ɖ߂�
				mSpriteData.x = (float)x;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				break;
			case '\t':                            // �^�u
				mWidth = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				tabX = static_cast<int>(mSpriteData.x) / (scaledWidth * mTabSize);
				tabX = (tabX + 1) * scaledWidth * mTabSize;
				tabW = tabX - static_cast<int>(mSpriteData.x);
				while (tabW > 0)
				{
					if (tabW >= scaledWidth)
					{
						drawChar(' ');
						mSpriteData.x += scaledWidth;
					}
					else
					{
						mWidth = tabW;        // �����̒[�������������ă^�u�ʒu�ɍ��킹��
						drawChar(' ');
						mSpriteData.x += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			case '\b':                            // �o�b�N�X�y�[�X
				mSpriteData.x -= scaledWidth;
				if (mSpriteData.x < 0)
					mSpriteData.x = 0;
				break;
			case '\v':                            // �����^�u
				mSpriteData.y += static_cast<int>(mHeight*mSpriteData.scale);
				break;
			case 0x01:                            // �t�H���g�V�O�l�`������
				mSpriteData.rect.top = 1;
				mSpriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
				mSpriteData.rect.left = 1;
				mSpriteData.rect.right = 1 + textNS::FONT_WIDTH;
				draw(mSpriteData);
				mSpriteData.x += scaledWidth;
				break;
			}
		}
	}
	return;
}

//=============================================================================
// ���݂̕�����Ɣz�u�̎w��ɍ��킹��spriteData.x�AspriteData.y��ݒ�
// �f�t�H���g�̔z�u��LEFT
//=============================================================================
void Text::doAlign(const std::string &str)
{
	if (mSpriteData.texture == NULL)  // �e�N�X�`�����Ȃ��ꍇ
		return;

	UINT w, h;
	switch (mAlign) {
		// X�������ƂȂ�悤�ɒ��������AY����[�ƂȂ�悤�ɏ㑵��
	case textNS::CENTER:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		break;
		// X�AY���E�[�ƂȂ�悤�ɉE����
	case textNS::RIGHT: 
		getWidthHeight(str, w, h);
		mSpriteData.x -= w;
		break;
		// X�������ƂȂ�悤�ɒ��������AY���㉺�̐^�񒆂ƂȂ�悤�ɔz�u
	case textNS::CENTER_MIDDLE:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		mSpriteData.y -= h / 2;
		break;
		// X�������ƂȂ�悤�ɒ��������AY�����[�ƂȂ�悤�ɉ�����
	case textNS::CENTER_BOTTOM:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		mSpriteData.y -= h;
		break;
		// X�����[�ƂȂ�悤�ɍ������AY�����[�ƂȂ�悤�ɉ�����
	case textNS::LEFT_BOTTOM:
		getWidthHeight(str, w, h);
		mSpriteData.y -= h;
		break;
		// X���E�[�ƂȂ�悤�ɍ������AY�����[�ƂȂ�悤�ɉ�����
	case textNS::RIGHT_BOTTOM: 
		getWidthHeight(str, w, h);
		mSpriteData.x -= w;
		mSpriteData.y -= h;
		break;
	}
}

//=============================================================================
// getWidthHeight
// ���݂̃t�H���g�T�C�Y�ł̕�����̕��ƍ����i�s�N�Z���P�ʁj���擾
// ������͕\�����Ȃ�
//=============================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
	if (mSpriteData.texture == NULL)         // �e�N�X�`�����Ȃ��ꍇ
		return;

	UCHAR ch = 0, chN = 0;
	mWidth = textNS::FONT_WIDTH;
	int scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
	int strW = 0;
	h = 0;
	int stringWidth = 0;

	for (UINT i = 0; i<str.length(); i++)
	{
		ch = str.at(i);
		// �\���\�ȕ����̏ꍇ
		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
		{
			chN = ch - textNS::MIN_CHAR;    // MIN_CHAR�̈ʒu���C���f�b�N�X0
			if (mProportional)
			{
				mSpriteData.rect.left = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].left;
				// DirectX�X�v���C�g���̂���+1
				mSpriteData.rect.right = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
				mWidth = mSpriteData.rect.right - mSpriteData.rect.left + mProportionalSpacing;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
			}
			else    // �Œ�s�b�`
			{
				mWidth = textNS::FONT_WIDTH;
				mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
			}
			stringWidth += scaledWidth;
		}
		else    // �܂��́A�\���s�\�ȕ����̏ꍇ
		{
			switch (ch)
			{
			case ' ':   // �X�y�[�X
				if (mProportional)
				{
					mWidth = (textNS::FONT_WIDTH) / 2;
					scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				}
				stringWidth += scaledWidth;
				break;
			case '\n':  // �j���[���C��
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				h += static_cast<int>(mHeight*mSpriteData.scale);
				break;
			case '\r':  // ���^�[��
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				break;
			case '\t':  // �^�u
			{
				mWidth = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				int tabX = static_cast<int>(mSpriteData.x) / (scaledWidth * mTabSize);
				tabX = (tabX + 1) * scaledWidth * mTabSize;
				int tabW = tabX - static_cast<int>(mSpriteData.x);
				while (tabW > 0)
				{
					if (tabW >= scaledWidth)
						stringWidth += scaledWidth;
					else
					{
						// �����̒[��������������
						// �^�u�ʒu�ɍ��킹��
						mWidth = tabW;
						stringWidth += tabW;
					}
					tabW -= scaledWidth;
				}
			}
			break;
			case '\b':      // �o�b�N�X�y�[�X
				stringWidth -= scaledWidth;
				if (stringWidth < 0)
					stringWidth = 0;
				break;
			case 0x01:      // ���ʂȃP�[�X
				stringWidth += scaledWidth;
				break;
			}
		}
	}
	if (strW == 0)
		strW = stringWidth;
	w = strW;
	return;
}

//=============================================================================
// drawChar
// spriteDate�ɂ���ċL�q����镶���X�v���C�g��F�Ɠh��Ԃ����g���ĕ\��
// �����ƕ�����\������
//=============================================================================
void Text::drawChar(UCHAR ch)
{
	SpriteData sd2 = mSpriteData;    // �X�v���C�g�f�[�^���R�s�[

	// backColor�F��\��
	if (mBackColor != graphicsNS::TRANSCOLOR) // backColor�������łȂ��ꍇ
	{
		mSpriteData.rect.top = (textNS::SOLID - textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
		mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::GRID_HEIGHT - 2;
		mSpriteData.rect.left = (textNS::SOLID - textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
		mSpriteData.rect.right = mSpriteData.rect.left + mWidth;
		draw(mBackColor);				// backColor��`��
		mSpriteData.rect = sd2.rect;    // �����̋�`�𕜌�
	}

	// ������\��
	if (mUnderline)
	{
		mSpriteData.rect.top = (textNS::UNDERLINE - textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
		mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::GRID_HEIGHT - 2;
		mSpriteData.rect.left = (textNS::UNDERLINE - textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
		mSpriteData.rect.right = mSpriteData.rect.left + mWidth;
		draw(mColor);
		mSpriteData.rect = sd2.rect;   // �����̋�`�𕜌�
	}

	// ������\��
	if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // �\���\�ȕ����̏ꍇ
	{
		draw(mSpriteData, mColor);
		if (mBold)   // �����́A�I�t�Z�b�gX���g���ĕ�����2��\�����邱�Ƃɂ���Ď���
		{
			mSpriteData.x += textNS::BOLD_SIZE*mSpriteData.scale;
			draw(mSpriteData, mColor);
			mSpriteData.x = sd2.x;
		}
	}
}


//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void Text::onLostDevice()
{
	try {
		if (!mInitialized)
			return;
		mFontTexture.onLostDevice();
	}
	catch (...) {}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void Text::onResetDevice()
{
	try {
		if (!mInitialized)
			return;
		mFontTexture.onResetDevice();
	}
	catch (...) {}
}

