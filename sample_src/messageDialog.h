//==========================================================
/// @file
/// @brief    MessageDialogクラス
/// @author   阿部拳之
///
/// @attention  ユーザーへのメッセージダイアログを表すクラスです。

//==========================================================
#ifndef _MESSAGEDIALOG_H        // このファイルが複数の箇所でインクルードされる場合に、
#define _MESSAGEDIALOG_H        // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

//==========================================================
// MessageDialogクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace messageDialogNS
{
    const UINT WIDTH = 400;             // ダイアログのデフォルトの幅
    const UINT HEIGHT = 100;            // デフォルトの高さ
    const UINT BORDER = 5;
    const UINT MARGIN = 5;              // 枠からテキストまでの余白
    const char FONT[] = "Arial";        // フォント
    const int FONT_HEIGHT = 18;         // フォントの高さ
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;        // テキストの色
    const COLOR_ARGB BORDER_COLOR = D3DCOLOR_ARGB(192,192,192,192); // 枠の色
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(255,100,100,192);   // 背景の色
    const UINT X = GAME_WIDTH/2 - WIDTH/2; // デフォルトの位置
    const UINT Y = GAME_HEIGHT/4 - HEIGHT/2;
    const UINT BUTTON_WIDTH = (UINT)(FONT_HEIGHT * 4.5);
    const UINT BUTTON_HEIGHT = FONT_HEIGHT + 4;
    const int MAX_TYPE = 2;
    const int OK_CANCEL = 0;            // OK/Cancelタイプのボタン
    const int YES_NO = 1;               // Yes/Noタイプのボタン
    static const char* BUTTON1_TEXT[MAX_TYPE] = {"OK", "YES"};
    static const char* BUTTON2_TEXT[MAX_TYPE] = {"CANCEL", "NO"};
    const byte DIALOG_CLOSE_KEY = VK_RETURN;    // Enterキー
    const COLOR_ARGB BUTTON_COLOR = graphicsNS::GRAY;       // ボタン背景
    const COLOR_ARGB BUTTON_FONT_COLOR = graphicsNS::WHITE; // ボタンテキストの色
}

// ユーザーへのメッセージダイアログを表すクラスです。
class MessageDialog
{
protected:
    Graphics    *mGraphics;              // グラフィックシステム
    Input       *mInput;                 // 入力システム
    TextDX      mDxFont;                 // DirectXフォント
    float       mX,mY;                   // 画面の位置
    UINT        mHeight;                 // ダイアログの高さ、print()で計算
    UINT        mWidth;                  // ダイアログの幅
    std::string mText;                   // ダイアログテキスト
    RECT        mTextRect;               // テキストの矩形
    RECT        mButtonRect;             // ボタンの矩形
    RECT        mButton2Rect;            // ボタン2の矩形
    COLOR_ARGB  mFontColor;              // フォントの色(a,r,g,b)
    COLOR_ARGB  mBorderColor;            // 枠の色(a,r,g,b)
    COLOR_ARGB  mBackColor;              // 背景の色(a,r,g,b)
    COLOR_ARGB  mButtonColor;            // ボタンの色
    COLOR_ARGB  mButtonFontColor;        // ボタンフォントの色
    VertexC mVtx[4];                     // 頂点データ
    LP_VERTEXBUFFER mDialogVerts;        // ダイアログの頂点バッファ
    LP_VERTEXBUFFER mBorderVerts;        // 枠の頂点バッファ
    LP_VERTEXBUFFER mButtonVerts;        // ボタンの頂点バッファ
    LP_VERTEXBUFFER mButton2Verts;       // ボタン2の頂点バッファ
    int mButtonClicked;                  // どのボタンがクリックされたか（1または2）
    int mButtonType;                     // 0 = OK/Cancel, 1 = Yes/No
    bool    mInitialized;                // 正常に初期化された場合、true
    bool    mVisible;                    // trueの場合は表示する
    HWND    mHwnd;                       // ウィンドウへのハンドル
    float   mScreenRatioX, mScreenRatioY;

public:
	// コンストラクタ
    MessageDialog();
    // デストラクタ
    virtual ~MessageDialog();

    // MessageDialogを初期化
	// 実行前：*gはGraphicsオブジェクトを指す
	//		   *inはInputオブジェクトを指す
	//		   hwnd = ウィンドウハンドル
    bool initialize(Graphics *g, Input *in, HWND hwnd);

	// 頂点バッファを準備
    void prepareVerts();

	// MessageDialogを表示
    const void draw();

	// クリックされたボタンを戻す
	// 0 = ボタンはクリックされていない
	// 1は左のボタン、2は右のボタン
    int getButtonClicked()  {return mButtonClicked;}

	// 表示/非表示の状態を戻す
    bool getVisible() {return mVisible;}

	// フォントの色を設定
    void setFontColor(COLOR_ARGB fc)    {mFontColor = fc;}

	// 枠の色を設定
    void setBorderColor(COLOR_ARGB bc)  {mBorderColor = bc;}

	// 背景の色を設定
    void setBackColor(COLOR_ARGB bc)    {mBackColor = bc;}

	// ボタンの色を設定
    void setButtonColor(COLOR_ARGB bc)  {mButtonColor = bc;}

	// ボタンフォントの色を設定
    void setButtonFontColor(COLOR_ARGB bfc) {mButtonFontColor = bfc;}

	// 表示/非表示を設定
    void setVisible(bool v) {mVisible = v;}

    // ボタンタイプを設定（0 = OK/CANCEL, 1 = YES/NO）
    void setButtonType(UINT t)
    {
        if(t < messageDialogNS::MAX_TYPE)
            mButtonType = t;
    }

	// テキストstrをMessageDialogに表示
    void print(const std::string &str);

	// 「閉じる」イベントをチェック
    void update();

	// グラフィックスデバイスが消失したときに呼び出す
    void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出す
    void onResetDevice();
};

#endif

