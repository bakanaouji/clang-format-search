//==========================================================
/// @file
/// @brief    console.hの実装
/// @author   阿部拳之
///
/// @attention  デバッグ用のコンソールを表すクラスです。
///				「`」で表示され、特定のコマンドを打つことでデバッグが可能です。

//==========================================================

#include "console.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Console::Console()
{
    mInitialized = false;                // 正常に初期化された場合、trueを設定
    mGraphics = NULL;
    mVisible = false;                    // 非表示にする
    mFontColor = consoleNS::FONT_COLOR;
    mBackColor = consoleNS::BACK_COLOR;

    mX = consoleNS::X;                   // 最初のコンソール位置
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
// デストラクタ
//=============================================================================
Console::~Console()
{
    onLostDevice();
}

//=============================================================================
// コンソールを初期化
//=============================================================================
bool Console::initialize(Graphics *g, Input *in)
{
    try {
        mGraphics = g;                    // グラフィックシステム
        mInput = in;

        // 左上
        mVtx[0].x = mX;
        mVtx[0].y = mY;
        mVtx[0].z = 0.0f;
        mVtx[0].rhw = 1.0f;
        mVtx[0].color = mBackColor;

        // 右上
        mVtx[1].x = mX + consoleNS::WIDTH;
        mVtx[1].y = mY;
        mVtx[1].z = 0.0f;
        mVtx[1].rhw = 1.0f;
        mVtx[1].color = mBackColor;

        // 右下
        mVtx[2].x = mX + consoleNS::WIDTH;
        mVtx[2].y = mY + consoleNS::HEIGHT;
        mVtx[2].z = 0.0f;
        mVtx[2].rhw = 1.0f;
        mVtx[2].color = mBackColor;

        // 左下
        mVtx[3].x = mX;
        mVtx[3].y = mY + consoleNS::HEIGHT;
        mVtx[3].z = 0.0f;
        mVtx[3].rhw = 1.0f;
        mVtx[3].color = mBackColor;

        mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

        // DirectXフォントを初期化
        if(mDxFont.initialize(mGraphics, consoleNS::FONT_HEIGHT, false,
                             false, consoleNS::FONT) == false)
            return false;      // 失敗の場合
        mDxFont.setFontColor(mFontColor);

    } catch(...) {
        return false;
    }

    mInitialized = true;
    return true;
}

//=============================================================================
// コンソールを描画
// 実行前：BeginSceneとEndSceneの内側にする
//=============================================================================
const void Console::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mVertexBuffer);       // 背景を描画
    if(mText.size() == 0)
        return;

    mGraphics->spriteBegin();                // スプライトの描画を開始

    // コンソールにテキストを表示
    mTextRect.left = 0;
    mTextRect.top = 0;

	// textRectの下端を1行の高さに設定
    mDxFont.print("|",mTextRect,DT_CALCRECT); // 「|」を、全高を使う文字として使用
    int rowHeight = mTextRect.bottom + 2;    // 1行の高さ（+2は行間スペース）
    if(rowHeight <= 0)                      // これはtrueにはならないと想定される
        rowHeight = 20;                     // 機能する値を強制的に設定

	// コンソール上に表示可能な行数
    mRows = (consoleNS::HEIGHT - 2*consoleNS::MARGIN) / rowHeight;
    mRows -= 2;                              // 下部に、入力プロンプト用のスペースを空ける
    if (mRows <= 0)                          // これはtrueにはならないと想定される
        mRows = 5;                           // 機能する値を強制的に設定

	// 1行のテキスト表示rectを設定
    mTextRect.left = (long)(mX + consoleNS::MARGIN);
    mTextRect.right = (long)(mTextRect.right + consoleNS::WIDTH - consoleNS::MARGIN);
    // -2*rowHeightは、入力プロンプト用のスペース
    mTextRect.bottom = (long)(mY + consoleNS::HEIGHT - 2*consoleNS::MARGIN - 2*rowHeight);
    // すべての行（最大でtext.size())を下から上へ処理
    for(int r=mScrollAmount; r<mRows+mScrollAmount && r<(int)(mText.size()); r++)
    {
		// この行のテキスト表示rectの上端を設定
        mTextRect.top = mTextRect.bottom - rowHeight; 
        // テキストの1行を表示
        mDxFont.print(mText[r],mTextRect,DT_LEFT);     
		// テキスト表示rectの下端を次の行用に調整
        mTextRect.bottom -= rowHeight;               
    }

	// コマンド用プロンプトと、現在のコマンド文字列を表示
	// プロンプト用のテキスト表示rectを設定
    mTextRect.bottom = (long)(mY + consoleNS::HEIGHT - consoleNS::MARGIN);
    mTextRect.top = mTextRect.bottom - rowHeight;
    std::string prompt = ">";                   // プロンプト文字列を作成
    prompt += mInput->getTextIn();
    mDxFont.print(prompt,mTextRect,DT_LEFT);      // プロンプトとコマンドを表示

    mGraphics->spriteEnd();                      // スプライトの描画を終了
}

//=============================================================================
// コンソールの表示と非表示
//=============================================================================
void Console::showHide() 
{
    if (!mInitialized)
        return;
    mVisible = !mVisible;
    mInput->clear(inputNS::KEYS_PRESSED|inputNS::TEXT_IN);    // 古い入力を消去
}            


//=============================================================================
// コンソールにテキストを追加
// strの先頭行のテキストのみ表示される
//=============================================================================
void Console::print(const std::string &str)     // コンソールにテキストを追加
{
    if (!mInitialized)
        return;
    mText.push_front(str);                       // strを、デックtextに追加
    if(mText.size() > consoleNS::MAX_LINES)
        mText.pop_back();                        // 最も古い行を削除
}

//=============================================================================
// コンソールコマンドを戻す
// シングルキーのコンソールコマンドを処理する
// その他のコマンドはすべてゲームに戻す
//=============================================================================
std::string Console::getCommand()
{
	// コンソールが初期化されていない場合、または表示されていない場合
    if (!mInitialized || !mVisible)               
        return "";

	// コンソールキーをチェック
    if (mInput->wasKeyPressed(CONSOLE_KEY))
        hide();                                 // コンソールを非表示

	// Escキーをチェック
    if (mInput->wasKeyPressed(ESC_KEY))
        return "";

	// スクロールをチェック
    if (mInput->wasKeyPressed(VK_UP))            // 上方向キー
        mScrollAmount++;
    else if (mInput->wasKeyPressed(VK_DOWN))     // 下方向キー
        mScrollAmount--;
    else if (mInput->wasKeyPressed(VK_PRIOR))    // Page Upキー
        mScrollAmount += mRows;
    else if (mInput->wasKeyPressed(VK_NEXT))     // Page Downキー
        mScrollAmount -= mRows;
    if (mScrollAmount < 0)
        mScrollAmount = 0;
    if (mScrollAmount > consoleNS::MAX_LINES-1)
        mScrollAmount = consoleNS::MAX_LINES-1;
    if (mScrollAmount > (int)(mText.size())-1)
        mScrollAmount = (int)(mText.size())-1;

    mCommandStr = mInput->getTextIn();            // ユーザーが入力したテキストを取得
    // キーがゲームに渡さない
    mInput->clear(inputNS::KEYS_DOWN|inputNS::KEYS_PRESSED|inputNS::MOUSE);

    if (mCommandStr.length() == 0)               // コマンドが入力されていない場合
        return "";
	// Enterキーが押されていない場合
    if (mCommandStr.at(mCommandStr.length()-1) != '\r')
        return "";                              // 戻る（コマンドでない）

	// コマンドの末尾から「\r」を削除
    mCommandStr.erase(mCommandStr.length()-1);
    mInput->clearTextIn();						// 入力行をクリア
    mInputStr = mCommandStr;
    return mCommandStr;                          // コマンドを戻す
}


//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void Console::onLostDevice()
{
    if (!mInitialized)
        return;
    mDxFont.onLostDevice();
    SAFE_RELEASE(mVertexBuffer);
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出される
//=============================================================================
void Console::onResetDevice()
{
    if (!mInitialized)
        return;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);
    mDxFont.onResetDevice();
}

