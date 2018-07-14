//==========================================================
/// @file
/// @brief    inputDialog.hの実装
/// @author   阿部拳之
///
/// @attention ユーザーの入力用のダイアログボックスを表すクラスです。

//==========================================================
#include "inputDialog.h"
//==========================================================

//=============================================================================
// コンストラクタ
//=============================================================================
InputDialog::InputDialog()
{
    mTextBackColor = inputDialogNS::TEXT_BACK_COLOR;
    mTextFontColor = inputDialogNS::TEXT_COLOR;
    mInTextVerts = NULL;
    mInText = "";
}

//=============================================================================
// デストラクタ
//=============================================================================
InputDialog::~InputDialog()
{
    onLostDevice();            // すべてのグラフィックアイテムについてonLostDevice()を呼び出す
}

//=============================================================================
// ダイアログの背景とボタンを描画するために頂点バッファを準備
//=============================================================================
void InputDialog::prepareVerts()
{
    MessageDialog::prepareVerts();  // 基本クラスのprepareVertsを呼び出す
    SAFE_RELEASE(mInTextVerts);

    // inTextの左上
    mVtx[0].x = mX + messageDialogNS::BORDER*2;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT*2.5f;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mTextBackColor;
    // inTextの右上
    mVtx[1].x = mX + mWidth - messageDialogNS::BORDER*2;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mTextBackColor;
    // inText右下
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mTextBackColor;
    // inText左下
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mTextBackColor;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mInTextVerts);

    // inTextRectを設定
    mInTextRect.left   = (long)mVtx[0].x;
    mInTextRect.right  = (long)mVtx[1].x;
    mInTextRect.top    = (long)mVtx[0].y;
    mInTextRect.bottom = (long)mVtx[2].y;
}

//=============================================================================
// InputDialogを描画
//=============================================================================
const void InputDialog::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mBorderVerts);       // 境界の描画
    mGraphics->drawQuad(mDialogVerts);       // 背景の描画
    mGraphics->drawQuad(mButtonVerts);       // ボタンの描画
    mGraphics->drawQuad(mButton2Verts);      // ボタン2の描画
    mGraphics->drawQuad(mInTextVerts);       // テキスト入力領域の描画

    mGraphics->spriteBegin();				 // スプライト描画開始

    if(mText.size() == 0)
        return;
	// メッセージダイアログ上のテキストを表示
    mDxFont.setFontColor(mFontColor);
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK);

	// ボタン上のテキストを描画
    mDxFont.setFontColor(mButtonFontColor);
    mDxFont.print(messageDialogNS::BUTTON1_TEXT[mButtonType],mButtonRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    mDxFont.print(messageDialogNS::BUTTON2_TEXT[mButtonType],mButton2Rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	// 入力されたテキストを描画
    mDxFont.setFontColor(mTextFontColor);
    mTempRect = mInTextRect;      // 保存
	// DT_CALCRECTオプションが指定されるとテキストは表示されない。RECT.rightを移動
    mDxFont.print(mInText,mTempRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT);
    if(mTempRect.right > mInTextRect.right)   // テキストが長すぎる場合、右側を調整
        mDxFont.print(mInText,mInTextRect,DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
    else    // それ以外の場合、左側を調整
        mDxFont.print(mInText,mInTextRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER);

    mGraphics->spriteEnd();                  // スプライト描画終了
}

//=============================================================================
// DIALOG_CLOSE_KEYとボタンのマウスクリックをチェック
//=============================================================================
void InputDialog::update()
{
    MessageDialog::update();        // スーパークラスのupdateを呼び出し
    if (!mInitialized || !mVisible)
    {
        if(mButtonClicked == 2)     // キャンセルボタンが押されたら
            mInText = "";           // 入力テキストをクリア
        return;
    }
    mInText = mInput->getTextIn();  // 入力テキストを確保
}

//=============================================================================
// テキストstrをInputDialogに表示
//=============================================================================
void InputDialog::print(const std::string &str)         
{
    if (!mInitialized || mVisible)  // 初期化がされていない、または使用中の場合
        return;
    mText = str + "\n\n\n\n\n";		// 入力テキストやボタンの余白を残す

	// ダイアログのテキスト領域用のtextRectをセット
    mTextRect.left   = (long)(mX + messageDialogNS::MARGIN);
    mTextRect.right  = (long)(mX + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    mTextRect.top    = (long)(mY + messageDialogNS::MARGIN);
    mTextRect.bottom = (long)(mY + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// テキストに必要とされる高さにtextRect.bottomをセット
	// DT_CALCRECTオプションが指定されるとテキストは表示されない。
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    mHeight = mTextRect.bottom - (int)mY + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // 頂点バッファを準備
    mInText = "";                   // 古い入力をクリア
    mInput->clearTextIn();
    mButtonClicked = 0;             // buttonClickedをクリア
    mVisible = true;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出す
//=============================================================================
void InputDialog::onLostDevice()
{
    if (!mInitialized)
        return;
    MessageDialog::onLostDevice();
    SAFE_RELEASE(mInTextVerts);
}

