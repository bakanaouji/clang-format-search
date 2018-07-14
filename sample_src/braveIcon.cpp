//==========================================================
/// @file
/// @brief    braveIcon.hの実装
/// @author   阿部拳之
///
/// @attention  勇者のアイコン画像を表すクラスです。

//==========================================================

#include "braveIcon.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
BraveIcon::BraveIcon() : Image()
{
	mSpriteData.width = braveIconNS::WIDTH;			// 勇者のアイコンのサイズ
	mSpriteData.height = braveIconNS::HEIGHT;
	mSpriteData.x = braveIconNS::X;					// 画面上の位置
	mSpriteData.y = braveIconNS::Y;
	mSpriteData.rect.bottom = braveIconNS::HEIGHT;	// 画面の一部を選択
	mSpriteData.rect.right = braveIconNS::WIDTH;
	mStartFrame = braveIconNS::START_FRAME;			// アニメーションの最初のフレーム
	mEndFrame = braveIconNS::END_FRAME;				// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;						// 現在のフレームはアニメーションの最初のフレームに設定
}

//=============================================================================
// Update
// プレイヤーの残り体力に応じてアニメーションを更新する。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================	
void BraveIcon::update(const float frameTime)
{
	// プレイヤーの体力に応じて、アニメーションを分岐
	if (mCurrentFrame == mStartFrame && mBrave->getHealth() < 75)
		setCurrentFrame(mStartFrame + 1);
	if (mCurrentFrame == mStartFrame + 1 && mBrave->getHealth() < 50)
		setCurrentFrame(mStartFrame + 2);
	if (mCurrentFrame == mStartFrame + 2 && mBrave->getHealth() < 25)
		setCurrentFrame(mStartFrame + 3);
}

//==========================================================
// パラメータリセット
//==========================================================
void BraveIcon::reset()
{
	// 現在のフレームはアニメーションの最初のフレームに設定
	setCurrentFrame(mStartFrame);
}
