//==========================================================
/// @file
/// @brief    Consoleクラス
/// @author   阿部拳之
///
/// @attention  デバッグ用のコンソールを表すクラスです。
///				「`」で表示され、特定のコマンドを打つことでデバッグが可能です。

//==========================================================
#ifndef _CONSOLE_H              // このファイルが複数の箇所でインクルードされる場合に、
#define _CONSOLE_H              // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

//==========================================================

// Consoleクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace consoleNS
{
    const UINT WIDTH = 1000;										// コンソールの矩形の幅（ピクセル単位）
    const UINT HEIGHT = 400;										// コンソールの矩形の高さ（ピクセル単位）
    const UINT X = 5;												// コンソールの左上隅からの画面位置
    const UINT Y = 5;
    const UINT MARGIN = 4;											// コンソールの端からテキストまでの余白
    const char FONT[] = "Courier New";								// コンソールのテキストに使用するフォント
    const int FONT_HEIGHT = 14;										// コンソールのフォントの高さ
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;				// コンソールのフォントの色
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192,128,128,128);   // コンソールの背景の色
    const int MAX_LINES = 256;										// テキストバッファ内の表示可能なテキストの最大行数
}

// デバッグ用のコンソールを表すクラスです。
//「`」で表示され、特定のコマンドを打つことでデバッグが可能です。
class Console
{
private:
    Graphics    *mGraphics;              // グラフィックシステムへのポインタ
    Input       *mInput;                 // 入力システムへのポインタ
    TextDX      mDxFont;                 // DirectXフォント
    float       mX,mY;                   // コンソールの位置
    int         mRows;                   // コンソールに収まるテキストの行数
    std::string mCommandStr;             // コンソールコマンド
    std::string mInputStr;               // コンソールへの入力テキスト
    std::deque<std::string> mText;       // デックtext
    RECT        mTextRect;               // テキスト表示rect
    COLOR_ARGB  mFontColor;              // フォントカラー(a,r,g,b)
    COLOR_ARGB  mBackColor;              // 背景色(a,r,g,b)
    VertexC mVtx[4];                     // 背景用のvertexデータ
    LP_VERTEXBUFFER mVertexBuffer;       // vertexデータ保存用のバッファ
    int         mScrollAmount;           // スクロールされている行数
    bool        mInitialized;            // 初期化に成功した場合、true
    bool        mVisible;                // コンソールを表示させる場合、true

public:
	// コンストラクタ
    Console();

	// デストラクタ
    virtual ~Console();

	// コンソールを初期化
	// 引数：g	グラフィックシステムへのポインタ
	// 引数：in	入力システムへのポインタ
    bool initialize(Graphics *g, Input *in);

	// コンソールを描画
	// 実行前：BeginSceneとEndSceneの内側にする
    const void draw();

	// コンソールの表示/非表示を制御するプロパティを反転させます。
	// 表示されているコンソールは非表示になり、
	// 非表示のコンソールは表示されるようになります。
    void showHide();

	// 表示/非表示を制御するプロパティの現在の状態を戻します。
	// 戻り値：表示状態か非表示状態か
    bool getVisible() {return mVisible;}

	// コンソールを表示します。
    void show() {mVisible = true;}

	// コンソールを非表示にします。
    void hide() {mVisible = false;}

	// コンソールディスプレイにテキスト文字列を追加します。
	// 引数：str	コンソールディスプレイに表示したい文字列
    void print(const std::string &str);

	// シングルキーのコンソールコマンドを処理します。
	// ユーザーが入力したコマンド文字列を戻します。
	// 戻り値：ユーザーが入力したコマンド
    std::string getCommand();

	// コンソールの入力テキストを戻します
	// 戻り値：コンソールの入力テキスト
    std::string getInput() {return mInputStr;}

	// コンソールの入力テキストをクリアします
    void clearInput()   {mInputStr = "";}

	// グラフィックスデバイスが消失したときに呼び出される
    void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
    void onResetDevice();
};

#endif

