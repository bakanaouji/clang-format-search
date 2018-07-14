//==========================================================
/// @file
/// @brief    braveAttackCollision.hの実装
/// @author   阿部拳之
///
/// @attention	プレイヤー（勇者）による攻撃の当たり判定用のエンティティを出現させるクラスです。

//==========================================================

#include "braveAttackCollision.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
BraveAttackCollision::BraveAttackCollision() : Entity()
{
	mActive = false;											// 攻撃はアクティブでない状態から開始
	mVisible = false;
	mSpriteData.width = braveAttackCollisionNS::WIDTH;			// １つの画像のサイズ
	mSpriteData.height = braveAttackCollisionNS::HEIGHT;
	mSpriteData.rect.bottom = braveAttackCollisionNS::HEIGHT;	// 画面の一部を選択
	mSpriteData.rect.right = braveAttackCollisionNS::WIDTH;
	// Boxの衝突判定用
	mEdge.left = (LONG)(-braveAttackCollisionNS::WIDTH / 2.0);
	mEdge.right = (LONG)(braveAttackCollisionNS::WIDTH / 2.0);
	mEdge.top = (LONG)(-braveAttackCollisionNS::HEIGHT / 2.0);
	mEdge.bottom = (LONG)(braveAttackCollisionNS::HEIGHT / 2.0);
	mCollisionType = entityNS::BOX;
	mCollisionTimer = 0.0f;										// 攻撃の当たり判定が出ている時間を計測するタイマー
}

//==========================================================
// パラメータ初期化
//==========================================================
void BraveAttackCollision::reset()
{
	Entity::reset();												// エンティティを初期化
	setVisible(false);												// エンティティは非表示
	mAttackEffect.setCurrentFrame(mAttackEffect.getStartFrame());	// アニメーションを初期化
	mAttackEffect.setVisible(false);								// アニメーションは最初は表示しない
}

//==========================================================
// Update
// 攻撃のアニメーションと自身のエンティティを更新する。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void BraveAttackCollision::update(const float frameTime)
{
	// 攻撃の衝撃派を更新
	mAttackEffect.update(frameTime);
	// 非アクティブなら、このエンティティの更新はしない
	if (mVisible == false)
		return;
	// 攻撃の当たり判定が出ている時間を加算
	mCollisionTimer += frameTime;
	// 攻撃の当たり判定が一定以上出ているなら、このエンティティを非アクティブにする
	if (mCollisionTimer >= braveAttackCollisionNS::COLLISION_TIME)
	{
		mCollisionTimer = 0.0f;
		mVisible = false;
		mActive = false;
	}
	// エンティティを更新
	Entity::update(frameTime);
}

//==========================================================
// Attack
// 勇者から攻撃を発動
// 指定した位置にこのエンティティをアクティブにして出現させる
//==========================================================
void BraveAttackCollision::attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction)
{
	// 攻撃の衝撃派のアニメーションを表示
	mAttackEffect.attack(centerX, centerY, width, height, direction);
	// 勇者の向きによって出現位置を分岐
	// 勇者の画像に偏りがあるため、若干位置を修正している
	switch (direction)
	{
		case characterNS::RIGHT:	// 右
			mSpriteData.width = braveAttackCollisionNS::WIDTH;	// １つの画像のサイズ
			mSpriteData.height = braveAttackCollisionNS::HEIGHT;
			mSpriteData.rect.bottom = braveAttackCollisionNS::HEIGHT;
			mSpriteData.rect.right = braveAttackCollisionNS::WIDTH;
			// Boxの衝突判定用
			mEdge.left = (LONG)(-braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.right = (LONG)(braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.top = (LONG)(-braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.bottom = (LONG)(braveAttackCollisionNS::HEIGHT / 2.0);
			mSpriteData.x = centerX + width / 2;
			mSpriteData.y = centerY - mSpriteData.height / 2;
			break;
		case characterNS::UP:		// 上
			mSpriteData.width = braveAttackCollisionNS::HEIGHT;	// １つの画像のサイズ
			mSpriteData.height = braveAttackCollisionNS::WIDTH;
			mSpriteData.rect.bottom = braveAttackCollisionNS::WIDTH;
			mSpriteData.rect.right = braveAttackCollisionNS::HEIGHT;
			// Boxの衝突判定用
			mEdge.left = (LONG)(-braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.right = (LONG)(braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.top = (LONG)(-braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.bottom = (LONG)(braveAttackCollisionNS::WIDTH / 2.0);
			mSpriteData.x = centerX - mSpriteData.width / 2;
			mSpriteData.y = centerY - height / 2 - mSpriteData.height;
			break;
		case characterNS::LEFT:		// 左
			mSpriteData.width = braveAttackCollisionNS::WIDTH;	// １つの画像のサイズ
			mSpriteData.height = braveAttackCollisionNS::HEIGHT;
			mSpriteData.rect.bottom = braveAttackCollisionNS::HEIGHT;
			mSpriteData.rect.right = braveAttackCollisionNS::WIDTH;
			// Boxの衝突判定用
			mEdge.left = (LONG)(-braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.right = (LONG)(braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.top = (LONG)(-braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.bottom = (LONG)(braveAttackCollisionNS::HEIGHT / 2.0);
			mSpriteData.x = centerX - mSpriteData.width / 2 - width;
			mSpriteData.y = centerY - mSpriteData.height / 2;
			break;
		case characterNS::DOWN:		// 下
			mSpriteData.width = braveAttackCollisionNS::HEIGHT;	// １つの画像のサイズ
			mSpriteData.height = braveAttackCollisionNS::WIDTH;
			mSpriteData.rect.bottom = braveAttackCollisionNS::WIDTH;
			mSpriteData.rect.right = braveAttackCollisionNS::HEIGHT;
			// Boxの衝突判定用
			mEdge.left = (LONG)(-braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.right = (LONG)(braveAttackCollisionNS::HEIGHT / 2.0);
			mEdge.top = (LONG)(-braveAttackCollisionNS::WIDTH / 2.0);
			mEdge.bottom = (LONG)(braveAttackCollisionNS::WIDTH / 2.0);
			mSpriteData.x = centerX - mSpriteData.width / 2;
			mSpriteData.y = centerY + 10;
			break;
	}
	// 攻撃の当たり判定が出ている時間をリセット
	mCollisionTimer = 0.0f;
	//visible = true;						// 攻撃を表示
	mActive = true;							// 衝突可能にする
}