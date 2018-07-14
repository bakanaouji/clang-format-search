//==========================================================
/// @file
/// @brief    hitEffect.hの実装
/// @author   阿部拳之
///
/// @attention  攻撃がヒットしたときのアニメーションを表示するクラスです。

//==========================================================

#include "hitEffect.h"

//==========================================================

//==========================================================
// デフォルトコンストラクタ
//==========================================================
HitEffect::HitEffect() : Image()
{
	mVisible = false;								// 非表示がデフォルト
	mSpriteData.width = hitEffectNS::WIDTH;			// １つの画像のサイズ
	mSpriteData.height = hitEffectNS::HEIGHT;
	mSpriteData.rect.bottom = hitEffectNS::HEIGHT;	// 画像内の選択する部分
	mSpriteData.rect.right = hitEffectNS::WIDTH;
	mCols = hitEffectNS::TEXTURE_COLS;				// テクスチャの列数
	mFrameDelay = hitEffectNS::ANIMATION_DELAY;		// アニメーションの各フレームの間隔
	mStartFrame = hitEffectNS::START_FRAME;			// アニメーションの最初のフレーム
	mEndFrame = hitEffectNS::END_FRAME;				// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;						// 現在のフレームはアニメーションの最初のフレームに設定
	setScale(0.5);									// スケール
	mLoop = false;									// アニメーションはループさせない
}


//==========================================================
// Update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void HitEffect::update(const float frameTime)
{
	// 非表示時には何もしない
	if (mVisible == false)
		return;
	// 画像を更新
	Image::update(frameTime);
	// アニメーションが終了した場合、非表示にする
	if (mAnimComplete)
		mVisible = false;
}

//==========================================================
// Hit
// この関数が呼ばれると攻撃がヒットしたときのエフェクトが発動
//==========================================================
void HitEffect::hit(const float x, const float y)
{
	// 指定された座標の四方にランダムに表示
	mSpriteData.x = x - mSpriteData.width * mSpriteData.scale / 2.0f + (rand() % 2) * 20 - 10;
	mSpriteData.y = y - mSpriteData.height * mSpriteData.scale / 2.0f + (rand() % 2) * 20 - 10;
	mVisible = true;
	setCurrentFrame(mStartFrame);
}