//==========================================================
/// @file
/// @brief    InputDialogクラス
/// @author   阿部拳之
///
/// @attention  ユーザーの入力用のダイアログボックスを表すクラスです。

//==========================================================
#ifndef _INPUTDIALOG_H          // このファイルが複数の箇所でインクルードされる場合に、 
#define _INPUTDIALOG_H          // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"
#include "messageDialog.h"

//==========================================================

// InputDailogクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace inputDialogNS
{
    const COLOR_ARGB TEXT_BACK_COLOR = graphicsNS::WHITE;   // 入力テキストの背景
    const COLOR_ARGB TEXT_COLOR = graphicsNS::BLACK;        // 入力テキストの色
}

//==========================================================
// メッセージダイアログを継承する入力ダイアログ
// ユーザーの入力用のダイアログボックスを表すクラスです。
class InputDialog : public MessageDialog
{
private:
    std::string mInText;                 // 入力テキスト
    RECT        mInTextRect;
    RECT        mTempRect;
    COLOR_ARGB  mTextBackColor;          // テキスト領域の背景の色
    COLOR_ARGB  mTextFontColor;          // テキスト領域のフォントの色
    LP_VERTEXBUFFER mInTextVerts;        // テキスト領域の頂点バッファ

public:
	// コンストラクタ
    InputDialog();
	// デストラクタ
    virtual ~InputDialog();

	// 頂点バッファを準備
    void prepareVerts();

	// InputDialogを表示
    const void draw();

	// 入力テキストを戻す
    std::string getText()   
    {
        if(!mVisible)
            return mInText;
        else
            return "";
    }

	// 入力テキストのフォントの色を設定
    void setTextFontColor(COLOR_ARGB fc)  {mTextFontColor = fc;}

	// 入力テキストの背景の色を設定
    void setTextBackColor(COLOR_ARGB bc)  {mTextBackColor = bc;}

	// テキストstrをInputDialogに表示
    void print(const std::string &str);

	// 「閉じる」イベントをチェック
    void update();

	// グラフィックスデバイスが消失したときに呼び出す
    void onLostDevice();
};

#endif

