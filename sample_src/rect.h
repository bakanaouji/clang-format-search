//==========================================================
/// @file
/// @brief    Rectクラス
/// @author   阿部拳之
///
/// @attention  四角形を描画するクラスです。
///				システムグラフィックスとして使用できます。

//==========================================================
#ifndef _RECT_H              // このファイルが複数の箇所でインクルードされる場合に、
#define _RECT_H              // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

//==========================================================

// Rectクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace rectNS
{
	const UINT WIDTH = GAME_WIDTH;						// 矩形の幅（ピクセル単位）
	const UINT HEIGHT = 50;								// 矩形の高さ（ピクセル単位）
	const UINT X = 0;									// 左上隅からの画面位置
	const UINT Y = 0;
	const COLOR_ARGB BACK_COLOR = graphicsNS::BLACK;	// 背景の色
}

// 四角形を描画するクラスです。
// システムグラフィックスとして使用できます。
class Rect
{
private:
	Graphics    *mGraphics;             // grapicsへのポインタ
	float       mX, mY;                 // rectの位置
	int mWidth, mHeight;				// 幅、高さ（ピクセル単位）
	COLOR_ARGB  mBackColor;             // 背景色(a,r,g,b)
	VertexC mVtx[4];                    // 背景用のvertexデータ
	LP_VERTEXBUFFER mVertexBuffer;      // vertexデータ保存用のバッファ
    bool        mInitialized;           // 初期化に成功した場合、true

public:
	// コンストラクタ
	Rect();

	// デストラクタ
	virtual ~Rect();

	// Rectの初期化
	bool initialize(Graphics *g);

	// 四角形を描画
	// 実行前：BeginSceneとEndSceneの内側にする
	const void draw();

	// グラフィックスデバイスが消失したときに呼び出される
	void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
	void onResetDevice();

	// パラメータリセット
	void reset();

	//==========================================================
	// setter
	//==========================================================

	// X位置をセット
	// 引数：x	位置X
	void setX(const float x) { mX = x; }
	// Y位置をセット
	// 引数：y	位置Y
	void setY(const float y) { mY = y; }
	// 幅をセット
	// 引数：w	幅
	void setWidth(const int w) { mWidth = w; }
	// 高さをセット
	// 引数：h	高さ
	void setHeight(const int h) { mHeight = h; }
	// 背景色をセット
	// 引数：color	背景色
	void setBackColor(const COLOR_ARGB color) { mBackColor = color; }
	
	//==========================================================
	// getter
	//==========================================================

	// X位置を戻す
	// 戻り値：X位置
	float getX() { return mX; }
	// Y位置を戻す
	// 戻り値：Y位置
	float getY() { return mY; }
};

#endif

