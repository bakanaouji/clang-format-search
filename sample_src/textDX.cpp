//==========================================================
/// @file
/// @brief    textDX.hの実装
/// @author   阿部拳之
///
/// @attention  DXFONTベースのテキストを描画するクラスです。

#include "textDX.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
TextDX::TextDX()
{
    mColor = SETCOLOR_ARGB(255,255,255,255); // デフォルトは白文字

	// フォントの位置をセット
    mFontRect.top = 0;
    mFontRect.left = 0;
    mFontRect.right = GAME_WIDTH;
    mFontRect.bottom = GAME_HEIGHT;
    mDxFont = NULL;
    mAngle  = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
TextDX::~TextDX()
{
    SAFE_RELEASE(mDxFont);
}

//=============================================================================
// DirectXフォントを作成
//=============================================================================
bool TextDX::initialize(Graphics *g, int height, bool bold, bool italic, 
                        const std::string &fontName)
{
    mGraphics = g;                   // グラフィックシステム

    UINT weight = FW_NORMAL;
    if(bold)
        weight = FW_BOLD;

	// DirectXフォントを作成
    if(FAILED(D3DXCreateFont(mGraphics->get3Ddevice(), height, 0, weight, 1, italic,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(),
        &mDxFont))) return false;

    // 変換行列を初期化
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

    return true;
}

//=============================================================================
// テキストをx、yに出力
// 失敗時には0を、成功時にはテキストの高さを戻す
// 実行前：spriteBegin()
// 実行後：spriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, int x, int y)
{
    if(mDxFont == NULL)
        return 0;
	// フォント位置を設定
    mFontRect.top = y;
    mFontRect.left = x;

	// 回転中心
    D3DXVECTOR2 rCenter=D3DXVECTOR2((float)x,(float)y);
    // テキストを特定の角度で回転する行列を準備
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, &rCenter, mAngle, NULL);
    // 行列をスプライトに適用
    mGraphics->getSprite()->SetTransform(&mMatrix);
    return mDxFont->DrawTextA(mGraphics->getSprite(), str.c_str(), -1, &mFontRect, DT_LEFT, mColor);
}

//=============================================================================
// DirectXテキスト書式を使って、RECT内にテキストを出力
// 失敗時には0を、成功時にはテキストの高さを戻す
// 実行前：spriteBegin()
// 実行後：spriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, RECT &rect, UINT format)
{
    if(mDxFont == NULL)
        return 0;

	// テキストを回転させない行列を準備
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	// 行列をスプライトに適用
    mGraphics->getSprite()->SetTransform(&mMatrix);
    return mDxFont->DrawTextA(mGraphics->getSprite(), str.c_str(), -1, &rect, format, mColor);
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void TextDX::onLostDevice()
{
    if(mDxFont == NULL)
        return;
    mDxFont->OnLostDevice();
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出される
//=============================================================================
void TextDX::onResetDevice()
{
    if(mDxFont == NULL)
        return;
    mDxFont->OnResetDevice();
}
