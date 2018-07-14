//==========================================================
/// @file
/// @brief    hpTextImage.hの実装
/// @author   阿部拳之
///
/// @attention  HPという文字の画像を表すクラスです。
///				システムグラフィックスとして使用できます。

//==========================================================
#include "hpTextImage.h"
//==========================================================

//==========================================================
// デフォルトコンストラクタ
//==========================================================
HpTextImage::HpTextImage() : Image()
{
	mSpriteData.width = hpTextImageNS::WIDTH;		// テキスト画像のサイズ
	mSpriteData.height = hpTextImageNS::HEIGHT;
	mSpriteData.x = hpTextImageNS::X;				// 画面上の初期位置
	mSpriteData.y = hpTextImageNS::Y;
	mSpriteData.rect.bottom = hpTextImageNS::HEIGHT;// 画面の一部を選択
	mSpriteData.rect.right = hpTextImageNS::WIDTH;
	mStartFrame = hpTextImageNS::START_FRAME;		// アニメーションの最初のフレーム
	mEndFrame = mStartFrame;						// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;					// 現在のフレームはアニメーションの最初のフレームにセット
}