//==========================================================
/// @file
/// @brief    barricade.hの実装
/// @author   阿部拳之
///
/// @attention  バリケードを表すクラスです。
///				城の周りに配置され、破壊されると城へ突破されます。

//==========================================================
#include "barricade.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Barricade::Barricade() : Entity()
{
	mSpriteData.width = barricadeNS::WIDTH;			// １つの画像のサイズ
	mSpriteData.height = barricadeNS::HEIGHT;
	mSpriteData.rect.bottom = barricadeNS::HEIGHT;	// 画像内の選択する部分
	mSpriteData.rect.right = barricadeNS::WIDTH;
	mCols = barricadeNS::TEXTURE_COLS;				// テクスチャの列数
	mStartFrame = barricadeNS::START_FRAME;			// アニメーションの最初のフレーム
	mEndFrame = barricadeNS::END_FRAME;				// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;						// 現在のフレームはアニメーションの最初のフレームに設定
	// Boxの衝突判定用
	mEdge.left = (LONG)(-barricadeNS::WIDTH / 2.0);
	mEdge.right = (LONG)(barricadeNS::WIDTH / 2.0);
	mEdge.top = (LONG)(-barricadeNS::HEIGHT / 2.0);
	mEdge.bottom = (LONG)(barricadeNS::HEIGHT / 2.0);
	mCollisionType = entityNS::BOX;
}

//==========================================================
// Update
// 残り体力に応じてアニメーションを更新する。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void Barricade::update(const float frameTime)
{
	// 非アクティブな場合、何もしない
	if (mActive == false)
		return;

	// 残り体力ごとに、アニメーションを分岐
	// 境界を残り体力が上下した場合、アニメーションを遷移させる
	if ((mCurrentFrame == barricadeNS::START_FRAME && mHealth < 66) || (mCurrentFrame == barricadeNS::START_FRAME + 1 && mHealth < 33))
	{
		// 体力が境界を下回った場合、アニメーションを進める
		setCurrentFrame(mCurrentFrame + 1);
	}
	if ((mCurrentFrame == barricadeNS::END_FRAME && mHealth >= 33) || (mCurrentFrame == barricadeNS::START_FRAME + 1 && mHealth >= 66))
	{
		// 体力が境界を上回った場合、アニメーションを戻す
		setCurrentFrame(mCurrentFrame - 1);
	}
	// 攻撃がヒットしたときのアニメーションを更新
	hitEffect.update(frameTime);
	// アニメーション以外を更新
	updateWithoutImage(frameTime);
	
}

//==========================================================
// パラメータリセット
//==========================================================
void Barricade::reset()
{
	Entity::reset();												// エンティティを初期化
	hitEffect.setCurrentFrame(hitEffect.getStartFrame());			// 攻撃がヒットしたときのアニメーションを初期化
	hitEffect.setVisible(false);									// 攻撃がヒットしたときのアニメーションは最初は表示しない
}

//==========================================================
// ダメージ処理
// WEAPONの種類によって受けるダメージが分岐
//==========================================================
void Barricade::damage(const WEAPON weapon)
{
	// 非アクティブな場合、ダメージは受けない
	if (!mActive)
		return;
	
	// WEAPONによってダメージを分岐
	switch (weapon)
	{
	case FIRE:					// 炎（プレイヤーの必殺技）
		// 変化なし
		break;
	case BRAVE_ATTACK:			// プレイヤーの攻撃
	case BRAVE_SECOND_ATTACK:	// プレイヤーの攻撃
		// バリケードを回復
		mHealth += BRAVE_RECOVERY_TO_BARRICADE;
		// MAX100を超えた場合は100にする
		if (mHealth > 100)
		{
			mHealth = 100;
		}
		// 攻撃がヒットしたときのアニメーションを発動
		hitEffect.hit(getCenterX(), getCenterY());
		break;
	case ENEMY_ATTACK:
		// バリケードにダメージ
		mHealth -= ENEMY_ATTACK_DAMEGE_BARRICADE * mDamagePer;
		// 攻撃がヒットしたときのアニメーションを発動
		hitEffect.hit(getCenterX(), getCenterY());
		// 残り体力がなくなった場合、
		if (mHealth <= 0)
		{
			// 自身と攻撃がヒットしたときのアニメーションを非アクティブにする
			mVisible = false;
			mActive = false;
			hitEffect.setVisible(false);
		}
		break;
	case MIDBOSS_ATTACK:
		// バリケードにダメージ
		mHealth -= MIDBOSS_ATTACK_DAMEGE_BARRICADE * mDamagePer;
		// 攻撃がヒットしたときのアニメーションを発動
		hitEffect.hit(getCenterX(), getCenterY());
		// 残り体力がなくなった場合、
		if (mHealth <= 0)
		{
			// 自身と攻撃がヒットしたときのアニメーションを非アクティブにする
			mVisible = false;
			mActive = false;
			hitEffect.setVisible(false);
		}
		break;
	default:
		break;
	}
}