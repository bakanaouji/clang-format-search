//==========================================================
/// @file
/// @brief    massageDialog.hの実装
/// @author   阿部拳之
///
/// @attention  ユーザーへのメッセージダイアログを表すクラスです。

#include "messageDialog.h"

//=============================================================================
// コンストラクタ
//=============================================================================
MessageDialog::MessageDialog()
{
    mInitialized = false;                // 正常に初期化された場合、trueを設定
    mGraphics = NULL;
    mVisible = false;                    // 非表示にする
    mFontColor = messageDialogNS::FONT_COLOR;
    mBorderColor = messageDialogNS::BORDER_COLOR;
    mBackColor = messageDialogNS::BACK_COLOR;
    mButtonColor = messageDialogNS::BUTTON_COLOR;
    mButtonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
    mX = messageDialogNS::X;             // 開始位置
    mY = messageDialogNS::Y;
    mHeight = messageDialogNS::HEIGHT;
    mWidth = messageDialogNS::WIDTH;
    mTextRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
    mTextRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
    mTextRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
    mTextRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
    mDialogVerts = NULL;
    mBorderVerts = NULL;
    mButtonVerts = NULL;
    mButton2Verts = NULL;
    mButtonType = 0;     // OK/Cancel
}

//=============================================================================
// デストラクタ
//=============================================================================
MessageDialog::~MessageDialog()
{
    onLostDevice();            // すべてのグラフィックアイテムについてonLostDevice()を呼び出す
}

//=============================================================================
// MessageDialogを初期化
//=============================================================================
bool MessageDialog::initialize(Graphics *g, Input *in, HWND h)
{
    try {
        mGraphics = g;                   // Graphicsオブジェクト
        mInput = in;                     // Inputオブジェクト
        mHwnd = h;

		// DirectXフォントを初期化
        if(mDxFont.initialize(mGraphics, messageDialogNS::FONT_HEIGHT, false, 
                             false, messageDialogNS::FONT) == false)
            return false;               // 失敗の場合
        mDxFont.setFontColor(mFontColor);
    } catch(...) {
        return false;
    }

    mInitialized = true;
    return true;
}

//=============================================================================
// ダイアログの背景とボタンを描画するために頂点バッファを準備
//=============================================================================
void MessageDialog::prepareVerts()
{
    SAFE_RELEASE(mDialogVerts);
    SAFE_RELEASE(mBorderVerts);
    SAFE_RELEASE(mButtonVerts);
    SAFE_RELEASE(mButton2Verts);

	// 枠の左上
    mVtx[0].x = mX;
    mVtx[0].y = mY;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mBorderColor;

    // 枠の右上
    mVtx[1].x = mX + mWidth;
    mVtx[1].y = mY;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mBorderColor;

	// 枠の右下
    mVtx[2].x = mX + mWidth;
    mVtx[2].y = mY + mHeight;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mBorderColor;

	// 枠の左下
    mVtx[3].x = mX;
    mVtx[3].y = mY + mHeight;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mBorderColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mBorderVerts);

    // backgroundの左上
    mVtx[0].x = mX + messageDialogNS::BORDER;
    mVtx[0].y = mY + messageDialogNS::BORDER;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mBackColor;

    // backgroundの右上
    mVtx[1].x = mX + mWidth - messageDialogNS::BORDER;
    mVtx[1].y = mY + messageDialogNS::BORDER;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mBackColor;

    // backgroundの右下
    mVtx[2].x = mX + mWidth - messageDialogNS::BORDER;
    mVtx[2].y = mY + mHeight - messageDialogNS::BORDER;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mBackColor;

    // backgroundの左下
    mVtx[3].x = mX + messageDialogNS::BORDER;
    mVtx[3].y = mY + mHeight - messageDialogNS::BORDER;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mBackColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mDialogVerts);

    // buttonの左上
    mVtx[0].x = mX + mWidth/2.0f - messageDialogNS::BUTTON_WIDTH/2.0f;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mButtonColor;

    // buttonの右上
    mVtx[1].x = mX + mWidth/2.0f + messageDialogNS::BUTTON_WIDTH/2.0f;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mButtonColor;

    // buttonの右下
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mButtonColor;

    // buttonの左下
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mButtonColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mButtonVerts);

    // buttonのRectを設定
    mButtonRect.left   = (long)mVtx[0].x;
    mButtonRect.right  = (long)mVtx[1].x;
    mButtonRect.top    = (long)mVtx[0].y;
    mButtonRect.bottom = (long)mVtx[2].y;

    // button2の左上
    mVtx[0].x = mX + mWidth - messageDialogNS::BUTTON_WIDTH*1.2f;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mButtonColor;
    // button2の右上
    mVtx[1].x = mVtx[0].x + messageDialogNS::BUTTON_WIDTH;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mButtonColor;
    // button2の右下
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mButtonColor;
    // button2の左下
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mButtonColor;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mButton2Verts);

	// button2のRectを設定
    mButton2Rect.left   = (long)mVtx[0].x;
    mButton2Rect.right  = (long)mVtx[1].x;
    mButton2Rect.top    = (long)mVtx[0].y;
    mButton2Rect.bottom = (long)mVtx[2].y;
}

//=============================================================================
// MessageDialogを描画
//=============================================================================
const void MessageDialog::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mBorderVerts);        // 枠を描画
    mGraphics->drawQuad(mDialogVerts);        // 背景を描画
    mGraphics->drawQuad(mButtonVerts);        // ボタンを描画
    mGraphics->drawQuad(mButton2Verts);       // ボタン2を描画

    mGraphics->spriteBegin();				  // スプライトの描画を開始

    if(mText.size() == 0)
        return;
	// テキストをMassegeDialogに表示
    mDxFont.setFontColor(mFontColor);
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK);

	// テキストをボタンに表示
    mDxFont.setFontColor(mButtonFontColor);
    mDxFont.print(messageDialogNS::BUTTON1_TEXT[mButtonType],mButtonRect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    mDxFont.print(messageDialogNS::BUTTON2_TEXT[mButtonType],mButton2Rect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    mGraphics->spriteEnd();                  // スプライトの描画を開始
}

//=============================================================================
// DIALOG_CLOSE_KEYとOKボタンのクリックをチェック
//=============================================================================
void MessageDialog::update()
{
    if (!mInitialized || !mVisible)
        return;
    if (mInput->wasKeyPressed(messageDialogNS::DIALOG_CLOSE_KEY))
    {
        mVisible = false;
        mButtonClicked = 1;              // ボタン1がクリックされた
        return;
    }

    if (mGraphics->getFullscreen() == false) // ウィンドウ表示の場合
    {
		// ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
        RECT clientRect;
        GetClientRect(mHwnd, &clientRect);
        mScreenRatioX = (float)GAME_WIDTH / clientRect.right;
        mScreenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }

    if (mInput->getMouseLButton())       // マウスの左ボタンの場合
    {
		// マウスクリックがボタン1（OK）の内側で発生した場合
        if (mInput->getMouseX()*mScreenRatioX >= mButtonRect.left &&
            mInput->getMouseX()*mScreenRatioX <= mButtonRect.right &&
            mInput->getMouseY()*mScreenRatioY >= mButtonRect.top &&
            mInput->getMouseY()*mScreenRatioY <= mButtonRect.bottom)
        {
            mVisible = false;            // メッセージダイアログを非表示
            mButtonClicked = 1;          // ボタン1がクリックされた
            return;
        }

		// マウスクリックボタン2（Cancel）の内側で発生した場合
        if (mInput->getMouseX()*mScreenRatioX >= mButton2Rect.left &&
            mInput->getMouseX()*mScreenRatioX <= mButton2Rect.right &&
            mInput->getMouseY()*mScreenRatioY >= mButton2Rect.top &&
            mInput->getMouseY()*mScreenRatioY <= mButton2Rect.bottom)
        {
            mVisible = false;            // メッセージダイアログを非表示
            mButtonClicked = 2;          // ボタン2がクリックされた
        }
    }
}

//=============================================================================
// テキスト文字列を設定、ダイアログの下端をテキストが収まるサイズに変更、
// visible = trueを設定
//=============================================================================
void MessageDialog::print(const std::string &str)         
{
	// 初期化されていない、または既に使用中の場合
    if (!mInitialized || mVisible)
        return;
    mText = str + "\n\n\n\n";        // ボタン用に空間を空ける

	// textRectをダイアログのテキスト領域として設定
    mTextRect.left   = (long)(mX + messageDialogNS::MARGIN);
    mTextRect.right  = (long)(mX + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    mTextRect.top    = (long)(mY + messageDialogNS::MARGIN);
    mTextRect.bottom = (long)(mY + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottomをテキストに必要な高さに設定
	// DT_CALCERCTオプションの場合、テキストは出力されない
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    mHeight = mTextRect.bottom - (int)mY + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // 頂点バッファを準備
    mButtonClicked = 0;             // buttonClickedクリア
    mVisible = true;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出す
//=============================================================================
void MessageDialog::onLostDevice()
{
    if (!mInitialized)
        return;
    mDxFont.onLostDevice();
    SAFE_RELEASE(mDialogVerts);
    SAFE_RELEASE(mBorderVerts);
    SAFE_RELEASE(mButtonVerts);
    SAFE_RELEASE(mButton2Verts);
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出す
//=============================================================================
void MessageDialog::onResetDevice()
{
    if (!mInitialized)
        return;
    prepareVerts();
    mDxFont.onResetDevice();
}

