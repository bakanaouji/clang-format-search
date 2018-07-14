//==========================================================
/// @file
/// @brief    rect.hの実装
/// @author   阿部拳之
///
/// @attention  四角形を描画するクラスです。
///				システムグラフィックスとして使用できます。

//==========================================================
#include "rect.h"
//==========================================================

//=============================================================================
// コンストラクタ
//=============================================================================
Rect::Rect()
{
	mInitialized = false;                // 正常に初期化された場合、trueを設定
	mGraphics = NULL;
	mBackColor = rectNS::BACK_COLOR;

	mX = rectNS::X;						// 最初の四角形の位置
	mY = rectNS::Y;

	mWidth = rectNS::WIDTH;
	mHeight = rectNS::HEIGHT;

	mVertexBuffer = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
Rect::~Rect()
{
	onLostDevice();
}

//=============================================================================
// Rectを初期化
//=============================================================================
bool Rect::initialize(Graphics *g)
{
	try {
		mGraphics = g;                    // グラフィックシステム

		// 左上
		mVtx[0].x = mX;
		mVtx[0].y = mY;
		mVtx[0].z = 0.0f;
		mVtx[0].rhw = 1.0f;
		mVtx[0].color = mBackColor;

		// 右上
		mVtx[1].x = mX + mWidth;
		mVtx[1].y = mY;
		mVtx[1].z = 0.0f;
		mVtx[1].rhw = 1.0f;
		mVtx[1].color = mBackColor;

		// 右下
		mVtx[2].x = mX + mWidth;
		mVtx[2].y = mY + mHeight;
		mVtx[2].z = 0.0f;
		mVtx[2].rhw = 1.0f;
		mVtx[2].color = mBackColor;

		// 左下
		mVtx[3].x = mX;
		mVtx[3].y = mY + mHeight;
		mVtx[3].z = 0.0f;
		mVtx[3].rhw = 1.0f;
		mVtx[3].color = mBackColor;

		mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

	}
	catch (...) {
		return false;
	}

	mInitialized = true;
	return true;
}


//=============================================================================
// reset
//=============================================================================
void Rect::reset()
{
	// 左上
	mVtx[0].x = mX;
	mVtx[0].y = mY;
	mVtx[0].z = 0.0f;
	mVtx[0].rhw = 1.0f;
	mVtx[0].color = mBackColor;

	// 右上
	mVtx[1].x = mX + mWidth;
	mVtx[1].y = mY;
	mVtx[1].z = 0.0f;
	mVtx[1].rhw = 1.0f;
	mVtx[1].color = mBackColor;

	// 右下
	mVtx[2].x = mX + mWidth;
	mVtx[2].y = mY + mHeight;
	mVtx[2].z = 0.0f;
	mVtx[2].rhw = 1.0f;
	mVtx[2].color = mBackColor;

	// 左下
	mVtx[3].x = mX;
	mVtx[3].y = mY + mHeight;
	mVtx[3].z = 0.0f;
	mVtx[3].rhw = 1.0f;
	mVtx[3].color = mBackColor;

	mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

}

//=============================================================================
// 四角形を描画
// 実行前：BeginSceneとEndSceneの内側にする
//=============================================================================
const void Rect::draw()
{
	mGraphics->drawQuad(mVertexBuffer);       // 背景を描画

	mGraphics->spriteBegin();                // スプライトの描画を開始

	mGraphics->spriteEnd();                      // スプライトの描画を終了
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void Rect::onLostDevice()
{
	if (!mInitialized)
		return;
	SAFE_RELEASE(mVertexBuffer);
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出される
//=============================================================================
void Rect::onResetDevice()
{
	if (!mInitialized)
		return;
	mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);
}

