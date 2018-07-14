//==========================================================
/// @file
/// @brief    attackEffect.hの実装
/// @author   阿部拳之
///
/// @attention  攻撃時のアニメーションを表示するクラスです。

//==========================================================

#include "attackEffect.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
AttackEffect::AttackEffect() : Image()
{
	mVisible = false;								// 非表示がデフォルト
	mSpriteData.width = attackEffectNS::WIDTH;		// １つの画像のサイズ
	mSpriteData.height = attackEffectNS::HEIGHT;
	mSpriteData.rect.bottom = attackEffectNS::HEIGHT;// 画像内の選択する部分
	mSpriteData.rect.right = attackEffectNS::WIDTH;
	mCols = attackEffectNS::TEXTURE_COLS;			// テクスチャの列数
	mFrameDelay = attackEffectNS::ANIMATION_DELAY;	// アニメーションの各フレームの間隔
	mStartFrame = attackEffectNS::START_FRAME;		// アニメーションの最初のフレーム
	mEndFrame = attackEffectNS::END_FRAME;			// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;						// 現在のフレームはアニメーションの最初のフレームに設定
	mSpriteData.scale = attackEffectNS::SCALE;		// スプライトのスケール
	mLoop = false;									// アニメーションはループさせない
}

//==========================================================
// Update
// アニメーションを更新する。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void AttackEffect::update (const float frameTime)
{
	// スプライトが非表示なら何もしない
	if (getVisible() == false)
		return;
	// 画像を更新
	Image::update(frameTime);
	// アニメーションが終了した場合、スプライトを非表示にする
	if (getAnimationComplete())
		setVisible(false);
}

//==========================================================
// Attack
// 攻撃の衝撃波のアニメーションを指定された位置に発動
//==========================================================
void AttackEffect::attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction)
{
	// brave（プレイヤー）の向きによって表示させる位置を分岐
	// プレイヤーの元々の画像が偏っているので、±10程度の補正をかけている
	switch (direction)
	{
	case characterNS::RIGHT:	// 右
		setX(centerX - getWidth() * getScale() / 2.0f);
		setY(centerY - getHeight() * getScale() / 2.0f - 10);
		setDegrees(0);
		flipHorizontal(false);
		break;
	case characterNS::LEFT:		// 左
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10);
		setDegrees(0);
		flipHorizontal(true);
		break;
	case characterNS::UP:		// 上
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10 - 10);
		setDegrees(40.0f);
		flipHorizontal(true);
		break;
	case characterNS::DOWN:		// 下
		setX(centerX - getWidth() * getScale() / 2.0f - 10);
		setY(centerY - getHeight() * getScale() / 2.0f - 10 + 10);
		setDegrees(230.0f);
		flipHorizontal(true);
		break;
	}
	// スプライトを表示
	mVisible = true;
	// 現在のフレームをアニメーションの最初のフレームにセット
	setCurrentFrame(mStartFrame);
}