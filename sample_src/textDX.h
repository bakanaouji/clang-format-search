//==========================================================
/// @file
/// @brief    TextDXクラス
/// @author   阿部拳之
///
/// @attention  DXFONTベースのテキストを描画するクラスです。

//==========================================================
#ifndef _TEXTDX_H               // このファイルが複数の箇所でインクルードされる場合に、
#define _TEXTDX_H               // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include "constants.h"
#include "graphics.h"

//==========================================================
// DXFONTベースのテキストを描画するクラスです。
class TextDX
{
private:
    Graphics    *mGraphics;		// graphicsへのポインタ
    COLOR_ARGB  mColor;         // フォントカラー(a,r,g,b)
    LP_DXFONT   mDxFont;		// DXFONT
    RECT        mFontRect;      // テキストの四角形
    D3DXMATRIX  mMatrix;		// テキストの回転行列
    float       mAngle;         // テキストの回転角度

public:
	// コンストラクタ
    TextDX();

	// デストラクタ
    virtual ~TextDX();

	// DirectXフォントを作成
    // 実行前：*g = Graphicsオブジェクトへのポインタ
    //		   height = 高さ
    //         bold = true/false
    //         italic = true/false
    //         &fontName = 使用するフォント名
    virtual bool initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName);

	// テキストをx、yに出力
	// 失敗時には0を、成功時にはテキストの高さを戻す
	// 実行前：spriteBegin()
	// 実行後：spriteEnd()
    virtual int print(const std::string &str, int x, int y);

	// DirectXテキスト書式を使って、RECT内にテキストを出力
	// 失敗時には0を、成功時にはテキストの高さを戻す
	// 実行前：spriteBegin()
	// 実行後：spriteEnd()
    virtual int print(const std::string &str, RECT &rect, UINT format);

	// 回転角を戻す（度）
    virtual float getDegrees()      {return mAngle*(180.0f/(float)PI);}

    // 回転角を戻す（ラジアン）
    virtual float getRadians()      {return mAngle;}

	// フォントカラーを戻す
    virtual COLOR_ARGB getFontColor() {return mColor;}

	// 回転角をセット（度）
    virtual void setDegrees(float deg)  {mAngle = deg*((float)PI/180.0f);}

	// 回転角をセット（ラジアン）
    virtual void setRadians(float rad)  {mAngle = rad;}

    // フォントカラーをセット
	virtual void setFontColor(COLOR_ARGB c) {mColor = c;}

	// グラフィックスデバイスが消失したときに呼び出される
    virtual void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
    virtual void onResetDevice();
};

#endif

