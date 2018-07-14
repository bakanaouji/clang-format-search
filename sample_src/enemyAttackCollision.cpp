//==========================================================
/// @file
/// @brief    enemyAttackCollision.hの実装
/// @author   阿部拳之
///
/// @attention  敵による攻撃の当たり判定用のエンティティを出現させるクラスです。

//==========================================================

#include "enemyAttackCollision.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
EnemyCollision::EnemyCollision() : Entity()
{
	mActive = false;												// 攻撃はアクティブでない状態から開始
	mVisible = false;
	mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;			// １つの画像のサイズ
	mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
	mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;	// 画面の一部を選択
	mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
	// Boxの衝突判定用
	mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
	mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
	mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
	mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
	mCollisionType = entityNS::BOX;
	mCollisionTimer = 0.0f;										// 攻撃の当たり判定が出ている時間を計測するタイマー
}

//==========================================================
// Update
// 攻撃のアニメーションと自身のエンティティを更新する。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void EnemyCollision::update(const float frameTime)
{
	// 非アクティブなら、このエンティティの更新はしない
	if (mVisible == false)
		return;
	// 攻撃の当たり判定が出ている時間を加算
	mCollisionTimer += frameTime;
	// 攻撃の当たり判定が一定以上出ているなら、このエンティティを非アクティブにする
	if (mCollisionTimer >= enemyCollisionNS::COLLISION_TIME)
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
// 敵から攻撃を発動
// 指定した位置にこのエンティティをアクティブにして出現させる
//==========================================================
void EnemyCollision::attack(const float centerX, const float centerY, const float width, const float height, const characterNS::DIRECTION direction)
{
	// 敵の向きによって出現位置を分岐
	// 敵の画像に偏りがあるため、若干位置を修正している
	switch (direction)
	{
	case characterNS::RIGHT:	// 右
		mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;	// １つの画像のサイズ
		mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
		// Boxの衝突判定用
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mSpriteData.x = centerX + width / 2;
		mSpriteData.y = centerY - height / 2;
		break;
	case characterNS::UP:		// 上
		mSpriteData.width = enemyCollisionNS::ATTACK_HEIGHT;	// １つの画像のサイズ
		mSpriteData.height = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_HEIGHT;
		// Boxの衝突判定用
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mSpriteData.x = centerX - width / 2;
		mSpriteData.y = centerY - height / 2 - mSpriteData.height - 10;
		break;
	case characterNS::LEFT:		// 左
		mSpriteData.width = enemyCollisionNS::ATTACK_WIDTH;	// １つの画像のサイズ
		mSpriteData.height = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_HEIGHT;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_WIDTH;
		// Boxの衝突判定用
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mSpriteData.x = centerX - width / 2 - mSpriteData.width;
		mSpriteData.y = centerY - height / 2;
		break;
	case characterNS::DOWN:		// 下
		mSpriteData.width = enemyCollisionNS::ATTACK_HEIGHT;	// １つの画像のサイズ
		mSpriteData.height = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.bottom = enemyCollisionNS::ATTACK_WIDTH;
		mSpriteData.rect.right = enemyCollisionNS::ATTACK_HEIGHT;
		// Boxの衝突判定用
		mEdge.left = (LONG)(-enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.right = (LONG)(enemyCollisionNS::ATTACK_HEIGHT / 2.0);
		mEdge.top = (LONG)(-enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mEdge.bottom = (LONG)(enemyCollisionNS::ATTACK_WIDTH / 2.0);
		mSpriteData.x = centerX - width / 2;
		mSpriteData.y = centerY + height / 2;
		break;
	}
	// 攻撃の当たり判定が出ている時間をリセット
	mCollisionTimer = 0.0f;
	//visible = true;							// 攻撃を表示
	mActive = true;							// 衝突可能にする	
	setRect();
}