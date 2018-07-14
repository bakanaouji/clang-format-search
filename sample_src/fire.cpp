//==========================================================
/// @file
/// @brief    fire.hの実装
/// @author   阿部拳之
///
/// @attention  プレイヤーの必殺技によって発射される炎を表すクラスです。

//==========================================================

#include "fire.h"

//==========================================================

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Fire::Fire() : Entity()
{
	mActive = false;								// 炎はアクティブでない状態から開始
	mVisible = false;
	mSpriteData.width = fireNS::WIDTH;			// １つの画像のサイズ
	mSpriteData.height = fireNS::HEIGHT;
	mSpriteData.rect.bottom = fireNS::HEIGHT;	// 画像内の選択する部分
	mSpriteData.rect.right = fireNS::WIDTH;
	mCols = fireNS::TEXTURE_COLS;				// テクスチャの列数
	mFrameDelay = fireNS::ANIMATION_DELAY;		// アニメーションの各フレームの間隔
	mStartFrame = fireNS::START_FRAME;			// ファイアーアニメーションの最初のフレーム
	mEndFrame = fireNS::END_FRAME;				// ファイアーアニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;					// 現在のフレームはアニメーションの最初のフレームに設定
	mCollisionType = entityNS::CIRCLE;			// 円の衝突判定
	mRadius = fireNS::COLLISION_RADIUS;			// 円の衝突判定用
	mFireTimer = 0.0f;							// 炎が発射可能になるまでの残り時間
	mMode = imageNS::VERTICAL;					// アニメーションのモード
}

//==========================================================
// パラメータ初期化
//==========================================================
void Fire::reset()
{
	mActive = false;								// 炎はアクティブでない状態から開始	
	mVisible = false;
	mFrameDelay = fireNS::ANIMATION_DELAY;		// アニメーションの各フレームの間隔
	mStartFrame = fireNS::START_FRAME;			// ファイアーアニメーションの最初のフレーム
	mEndFrame = fireNS::END_FRAME;				// ファイアーアニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;					// 現在のフレームはアニメーションの最初のフレームに設定
	mCollisionType = entityNS::CIRCLE;			// 円の衝突判定
	mRadius = fireNS::COLLISION_RADIUS;			// 円の衝突判定用
	mFireTimer = 0.0f;							// 炎が発射可能になるまでの残り時間
	mMode = imageNS::VERTICAL;					// アニメーションのモード
}

//==========================================================
// Update
// エンティティがアクティブな場合、一定時間まっすぐ移動
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
// 引数：frameTime　1フレームで経過した時間
//==========================================================
void Fire::update(const float frameTime)
{
	mFireTimer -= frameTime;						// 発射が可能になるまでの残り時間
	
	if (mVisible == false)
		return;

	if (mFireTimer < 0)							// 発射可能な場合
	{
		mVisible = false;						// 古い炎を無効化
		mActive = false;
	}

	Image::update(frameTime);

	mSpriteData.x += frameTime * mVelocity.x;		// X方向に移動
	mSpriteData.y += frameTime * mVelocity.y;		// Y方向に移動
	// 画面の端まで来たら消去
	if (mSpriteData.x > GAME_WIDTH || mSpriteData.x < 0 || mSpriteData.y < 0 || mSpriteData.y > GAME_HEIGHT)
	{
		mVisible = false;
		mActive = false;
	}
}

//==========================================================
// Fire
// Braveから炎を発射
//==========================================================
void Fire::fire(const float centerX, const float centerY, const float width, const float height, const float scale, int& mp, const characterNS::DIRECTION direction)
{
	if (mFireTimer <= 0.0f && mp > fireNS::FIRE_MP)	// 発射可能な場合
	{
		mAudio->playCue("fire");
		switch (direction)
		{
		case characterNS::UP:
			mVelocity.x = 0.0f;
			mVelocity.y = -fireNS::SPEED;
			mSpriteData.angle = (float)PI / 2.0f;
			break;
		case characterNS::RIGHT:
			mVelocity.x = fireNS::SPEED;
			mVelocity.y = 0.0f;
			mSpriteData.angle = (float)-PI;
			break;
		case characterNS::DOWN:
			mVelocity.x = 0.0f;
			mVelocity.y = fireNS::SPEED;
			mSpriteData.angle = (float)-PI / 2;
			break;
		case characterNS::LEFT:
			mVelocity.x = -fireNS::SPEED;
			mVelocity.y = 0.0f;
			mSpriteData.angle = 0.0f;
			break;
		}
		mSpriteData.x = (float)(centerX - mSpriteData.width / 2 - width * (scale - 1) / 2.0);
		mSpriteData.y = (float)(centerY - mSpriteData.height / 2 - height * (scale - 1) / 2.0f - 10.0f);
		mVisible = true;									// 炎を表示
		mActive = true;									// 衝突可能にする
		mFireTimer = fireNS::FIRE_DELAY;					// 発射の間隔
		mp -= fireNS::FIRE_MP;							// MPを減少
	}
}