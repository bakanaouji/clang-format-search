//==========================================================
/// @file
/// @brief    text.hの実装
/// @author   阿部拳之
///
/// @attention  スプライトテキストを描画するクラスです。


#include "text.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Text::Text() : Image()
{
	mFile = NULL;                        // フォントのテクスチャファイル名
	mGraphics = NULL;                    // グラフィックシステムへのポインタ
	mColor = graphicsNS::WHITE;          // デフォルトは白文字
	mBackColor = graphicsNS::TRANSCOLOR; // デフォルトは透明色
	mAlign = textNS::LEFT;
	mWidth = textNS::GRID_WIDTH - 3;     // 透明な境界のために-2、区切り線のために-1
	mHeight = textNS::GRID_HEIGHT - 3;
	mFontHeight = textNS::FONT_HEIGHT;
	mSpriteData.width = 0;
	mSpriteData.height = 0;
	mSpriteData.rect.bottom = 1;         // 画像の一部を選択するための四角形
	mSpriteData.rect.right = 1;
	for (int row = 0; row < textNS::ROWS; row++)         // フォント画像の各行の文字について
	{
		for (int col = 0; col < textNS::COLUMNS; col++)  // フォント画像の各列の文字について
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
// デストラクタ
//=============================================================================
Text::~Text()
{
	Image::~Image();
}

//=============================================================================
// Textを初期化
// フォント画像内の各文字の左端と右端を特定
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// fontData配列は、各文字と左端と右端を格納
//=============================================================================
bool Text::initialize(Graphics *g, const char *file)
{
	try {
		mGraphics = g;           // grpahicsオブジェクトへのポインタ

		//-------------------------------------------------------------
		// フォントテクスチャを読み込んで、各文字の正確な位置を
		// 特定するために、ピクセル単位で検査
		//-------------------------------------------------------------
		// フォントテクスチャを、ロック可能なシステムメモリに読み込む
		UINT w, h;
		HRESULT result = mGraphics->loadTextureSystemMem(file, graphicsNS::TRANSCOLOR, w, h, mTextureData);
		if (FAILED(result))
		{
			safeRelease(mTextureData);
			return false;
		}

		// textureData.widthとtextureData.heightはフォントテクスチャ
		// 全体のサイズを格納
		// 各文字は1ピクセル幅の枠を持つ
		// ROWS * COLS個の文字が存在する
		// フォントテクスチャをロック
		// （ピクセルデータにアクセスするために必要）
		D3DLOCKED_RECT rect;
		result = mTextureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
		if (FAILED(result))                          // ロックが失敗した場合
		{
			safeRelease(mTextureData);
			return false;
		}

		// フォント内の文字の各行を処理
		for (DWORD row = 0; row<textNS::ROWS; row++)
		{
			// フォント内の文字の各列を処理
			for (DWORD col = 0; col<textNS::COLUMNS; col++)
			{
				mFontData[row][col].left = MAXINT;    // fontDataを初期化
				mFontData[row][col].right = 0;

				// 文字を1ピクセルずつ処理
				// for y = 上端ピクセル; y <= 下端ピクセル; y++
				for (DWORD y = row*textNS::GRID_HEIGHT + 1; y<(row + 1)*textNS::GRID_HEIGHT - 1; y++)
				{
					// テクスチャ内のこのスキャンラインの先頭へのポインタを取得
					DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
					// この行のピクセルを処理
					for (DWORD x = col*textNS::GRID_WIDTH + 1; x<(col + 1)*textNS::GRID_WIDTH - 1; x++)
					{
						// このピクセルを取得
						DWORD dwPixel = pBits[x];

						// アルファが透明でない場合
						if ((dwPixel & 0xff000000) != 0x00)
						{
							// このピクセルのほうが左にある場合
							if (x < mFontData[row][col].left)
								// 文字の左端として保存
								mFontData[row][col].left = x;    
							// このピクセルのほうが右にある場合
							if (x > mFontData[row][col].right)
								// 文字の右端として保存
								mFontData[row][col].right = x;
						}
					}
				}
			}
		}

		// テクスチャの処理が完了したら、ロックを解除
		mTextureData->UnlockRect(0);

		// 単にフォントのスペースを取得するために必要だったので、
		// このフォントテクスチャを解放
		safeRelease(mTextureData);

		//-------------------------------------------------------------
		// フォント画像を使用するためにテクスチャマネージャーに読み込む
		//-------------------------------------------------------------
		if (!mFontTexture.initialize(mGraphics, file))
			return false;                   // フォントテクスチャの読み込みがエラーの場合
		// フォント画像を準備
		if (!Image::initialize(mGraphics, textNS::FONT_WIDTH, textNS::FONT_HEIGHT, 0, &mFontTexture))
			return false;                   // 失敗の場合
	}
	catch (...)
	{
		return false;
	}
	return true;                    // 成功の場合
}

//=============================================================================
// setXY
// 表示する位置（XY座標）をセット
//=============================================================================
void Text::setXY(int x2, int y2)
{
	if (x2 < 0 || y2 < 0)
		return;
	mSpriteData.x = (float)x2;
	mSpriteData.y = (float)y2;
}

//=============================================================================
// 文字列をX、Yに出力
// 実行前：spriteBegin()
// 実行後：spriteEnd()
//=============================================================================
void Text::print(const std::string &str)
{
	print(str, (int)mSpriteData.x, (int)mSpriteData.y);
}

//=============================================================================
// 文字列をX、Yに出力
// アラインメントは以下の通り：
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y, textNS::Alignment al)
{
	mAlign = al;             // アラインメントを保存
	print(str, x, y);
	mAlign = textNS::LEFT;
}

//=============================================================================
// 文字列をX、Yに出力
// 実行前：spriteBegin()
// 実行後：spriteEnd()
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
		// 表示可能な文字の場合
		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) 
		{
			chN = ch - textNS::MIN_CHAR;                // MIN_CHARの位置がインデックス0
			mSpriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
			mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::FONT_HEIGHT;
			if (mProportional)
			{
				mSpriteData.rect.left = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].left;
				// DirectXは右端 + 1を必要とする
				mSpriteData.rect.right = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
				mWidth = mSpriteData.rect.right - mSpriteData.rect.left;
				// 全幅を使う文字の場合、スペースは空けない
				if (mWidth >= textNS::FONT_WIDTH) 
				{
					mWidth = textNS::FONT_WIDTH;         // 幅を制限
					mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
					mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
				}
				else    // 全幅を使う文字でない場合、文字間にスペースを空ける
					mWidth += mProportionalSpacing;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				drawChar(ch);
			}
			else    // 固定ピッチ
			{
				mWidth = textNS::FONT_WIDTH;
				mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
				drawChar(ch);
			}
			mSpriteData.x += scaledWidth;
		}
		else    // 表示不可な文字
		{
			switch (ch)
			{
			case ' ':                            // スペース
				if (mProportional)
				{
					mWidth = textNS::FONT_WIDTH / 2;
					scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				}
				drawChar(' ');
				mSpriteData.x += scaledWidth;
				break;
				// ニューラインは下に1行進み、左端を、
				// 画面の左端でなく、Xの開始位置に設定
			case '\n':                            // ニューライン
				mSpriteData.x = (float)x;
				mSpriteData.y += static_cast<int>(mHeight*mSpriteData.scale);
				saveY = mSpriteData.y;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				mSpriteData.y = saveY;
				break;
			case '\r':                            // Xの開始位置に戻る
				mSpriteData.x = (float)x;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				break;
			case '\t':                            // タブ
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
						mWidth = tabW;        // 文字の端数分を処理してタブ位置に合わせる
						drawChar(' ');
						mSpriteData.x += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			case '\b':                            // バックスペース
				mSpriteData.x -= scaledWidth;
				if (mSpriteData.x < 0)
					mSpriteData.x = 0;
				break;
			case '\v':                            // 垂直タブ
				mSpriteData.y += static_cast<int>(mHeight*mSpriteData.scale);
				break;
			case 0x01:                            // フォントシグネチャ文字
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
// 現在の文字列と配置の指定に合わせてspriteData.x、spriteData.yを設定
// デフォルトの配置はLEFT
//=============================================================================
void Text::doAlign(const std::string &str)
{
	if (mSpriteData.texture == NULL)  // テクスチャがない場合
		return;

	UINT w, h;
	switch (mAlign) {
		// Xが中央となるように中央揃え、Yが上端となるように上揃え
	case textNS::CENTER:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		break;
		// X、Yが右端となるように右揃え
	case textNS::RIGHT: 
		getWidthHeight(str, w, h);
		mSpriteData.x -= w;
		break;
		// Xが中央となるように中央揃え、Yが上下の真ん中となるように配置
	case textNS::CENTER_MIDDLE:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		mSpriteData.y -= h / 2;
		break;
		// Xが中央となるように中央揃え、Yが下端となるように下揃え
	case textNS::CENTER_BOTTOM:
		getWidthHeight(str, w, h);
		mSpriteData.x -= w / 2;
		mSpriteData.y -= h;
		break;
		// Xが左端となるように左揃え、Yが下端となるように下揃え
	case textNS::LEFT_BOTTOM:
		getWidthHeight(str, w, h);
		mSpriteData.y -= h;
		break;
		// Xが右端となるように左揃え、Yが下端となるように下揃え
	case textNS::RIGHT_BOTTOM: 
		getWidthHeight(str, w, h);
		mSpriteData.x -= w;
		mSpriteData.y -= h;
		break;
	}
}

//=============================================================================
// getWidthHeight
// 現在のフォントサイズでの文字列の幅と高さ（ピクセル単位）を取得
// 文字列は表示しない
//=============================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
	if (mSpriteData.texture == NULL)         // テクスチャがない場合
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
		// 表示可能な文字の場合
		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
		{
			chN = ch - textNS::MIN_CHAR;    // MIN_CHARの位置がインデックス0
			if (mProportional)
			{
				mSpriteData.rect.left = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].left;
				// DirectXスプライト幅のため+1
				mSpriteData.rect.right = mFontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
				mWidth = mSpriteData.rect.right - mSpriteData.rect.left + mProportionalSpacing;
				scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
			}
			else    // 固定ピッチ
			{
				mWidth = textNS::FONT_WIDTH;
				mSpriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				mSpriteData.rect.right = mSpriteData.rect.left + textNS::FONT_WIDTH;
			}
			stringWidth += scaledWidth;
		}
		else    // または、表示不可能な文字の場合
		{
			switch (ch)
			{
			case ' ':   // スペース
				if (mProportional)
				{
					mWidth = (textNS::FONT_WIDTH) / 2;
					scaledWidth = static_cast<int>(mWidth*mSpriteData.scale);
				}
				stringWidth += scaledWidth;
				break;
			case '\n':  // ニューライン
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				h += static_cast<int>(mHeight*mSpriteData.scale);
				break;
			case '\r':  // リターン
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				break;
			case '\t':  // タブ
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
						// 文字の端数分を処理して
						// タブ位置に合わせる
						mWidth = tabW;
						stringWidth += tabW;
					}
					tabW -= scaledWidth;
				}
			}
			break;
			case '\b':      // バックスペース
				stringWidth -= scaledWidth;
				if (stringWidth < 0)
					stringWidth = 0;
				break;
			case 0x01:      // 特別なケース
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
// spriteDateによって記述される文字スプライトを色と塗りつぶしを使って表示
// 下線と文字を表示する
//=============================================================================
void Text::drawChar(UCHAR ch)
{
	SpriteData sd2 = mSpriteData;    // スプライトデータをコピー

	// backColor色を表示
	if (mBackColor != graphicsNS::TRANSCOLOR) // backColorが透明でない場合
	{
		mSpriteData.rect.top = (textNS::SOLID - textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
		mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::GRID_HEIGHT - 2;
		mSpriteData.rect.left = (textNS::SOLID - textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
		mSpriteData.rect.right = mSpriteData.rect.left + mWidth;
		draw(mBackColor);				// backColorを描画
		mSpriteData.rect = sd2.rect;    // 文字の矩形を復元
	}

	// 下線を表示
	if (mUnderline)
	{
		mSpriteData.rect.top = (textNS::UNDERLINE - textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
		mSpriteData.rect.bottom = mSpriteData.rect.top + textNS::GRID_HEIGHT - 2;
		mSpriteData.rect.left = (textNS::UNDERLINE - textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
		mSpriteData.rect.right = mSpriteData.rect.left + mWidth;
		draw(mColor);
		mSpriteData.rect = sd2.rect;   // 文字の矩形を復元
	}

	// 文字を表示
	if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // 表示可能な文字の場合
	{
		draw(mSpriteData, mColor);
		if (mBold)   // 太字は、オフセットXを使って文字を2回表示することによって実現
		{
			mSpriteData.x += textNS::BOLD_SIZE*mSpriteData.scale;
			draw(mSpriteData, mColor);
			mSpriteData.x = sd2.x;
		}
	}
}


//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
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
// グラフィックスデバイスがリセットされたときに呼び出される
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

