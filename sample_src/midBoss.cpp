//==========================================================
/// @file
/// @brief    midBoss.hの実装
/// @author   阿部拳之
///
/// @attention  中ボスを表すクラスです。
///				Enemyクラスを継承しているため、動作はほとんど雑魚敵と一緒です。

#include "midBoss.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
MidBoss::MidBoss() : Enemy()
{
	// 初期の方向は右
	mStartFrame = midBossNS::MOVE_RIGHT_START_FRAME;				// アニメーションの最初のフレーム
	mEndFrame = midBossNS::MOVE_RIGHT_END_FRAME;					// アニメーションの最後のフレーム
	mOldStartFrame = mStartFrame;									// 攻撃アニメーション終了後に戻るべきアニメーションの最初のフレーム
	mOldEndFrame = mEndFrame;										// 攻撃アニメーション終了後に戻るべきアニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;									// 現在のフレームをアニメーションの最初のフレームにセット
	mAttackAnimationMode = imageNS::HORIZONTAL;					// 攻撃時のアニメーションの遷移モード（水平、垂直）
	mMoveUpStartFrame = midBossNS::MOVE_UP_START_FRAME;			// 上方向移動のアニメーションの最初のフレーム
	mMoveUpEndFrame = midBossNS::MOVE_UP_END_FRAME;				// 上方向移動のアニメーションの最後のフレーム
	mMoveRightStartFrame = midBossNS::MOVE_RIGHT_START_FRAME;	// 右方向移動のアニメーションの最初のフレーム
	mMoveRightEndFrame = midBossNS::MOVE_RIGHT_END_FRAME;		// 右方向移動のアニメーションの最後のフレーム
	mMoveDownStartFrame = midBossNS::MOVE_DOWN_START_FRAME;		// 下方向移動のアニメーションの最初のフレーム
	mMoveDownEndFrame = midBossNS::MOVE_DOWN_END_FRAME;			// 下方向移動のアニメーションの最後のフレーム
	mMoveLeftStartFrame = midBossNS::MOVE_LEFT_START_FRAME;		// 左方向移動のアニメーションの最初のフレーム
	mMoveLeftEndFrame = midBossNS::MOVE_LEFT_END_FRAME;			// 左方向移動のアニメーションの最後のフレーム
	mAttackUpStartFrame = midBossNS::ATTACK_UP_START_FRAME;		// 上方向攻撃のアニメーションの最初のフレーム
	mAttackUpEndFrame = midBossNS::ATTACK_UP_END_FRAME;			// 上方向攻撃のアニメーションの最後のフレーム
	mAttackRightStartFrame = midBossNS::ATTACK_RIGHT_START_FRAME;// 右方向攻撃のアニメーションの最初のフレーム
	mAttackRightEndFrame = midBossNS::ATTACK_RIGHT_END_FRAME;	// 右方向攻撃のアニメーションの最後のフレーム
	mAttackDownStartFrame = midBossNS::ATTACK_DOWN_START_FRAME;	// 下方向攻撃のアニメーションの最初のフレーム
	mAttackDownEndFrame = midBossNS::ATTACK_DOWN_END_FRAME;		// 下方向攻撃のアニメーションの最後のフレーム
	mAttackLeftStartFrame = midBossNS::ATTACK_LEFT_START_FRAME;	// 左方向攻撃のアニメーションの最初のフレーム
	mAttackLeftEndFrame = midBossNS::ATTACK_LEFT_END_FRAME;		// 左方向攻撃のアニメーションの最後のフレーム
	mAttackTime = midBossNS::ATTACK_TIME;						// プレイヤーに隣接してから攻撃に移るまでの溜め時間
	mContinueAttackTime = midBossNS::CONTINUE_ATTACK_TIME;		// 連続で攻撃する場合の溜め時間
}

//==========================================================
// ダメージ処理
// WEAPONの種類によって受けるダメージが分岐
//==========================================================
void MidBoss::damage(const WEAPON weapon)
{
	// 敵のタイプによって、受けるダメージの割合を変化
	if (mType == enemyNS::NORMAL)
		setDamagePer(1.0f);
	else if (mType == enemyNS::RED)
		setDamagePer(0.8f);
	else if (mType == enemyNS::BLUE)
		setDamagePer(0.9f);

	// WEAPONによってダメージを分岐
	switch (weapon)
	{
	case FIRE:					// 炎
		// 一定ダメージを受ける
		mHealth -= BRAVE_FIRE_DAMAGE  * mDamagePer / 3;
		// ダメージ状態のフラグをオン
		mIsDamaged = true;
		break;
	case BRAVE_ATTACK:			// 勇者の攻撃
		// 一定ダメージを受ける
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer / 3;
		// ダメージ状態のフラグをオン
		mIsDamaged = true;
		break;
	case BRAVE_SECOND_ATTACK:	// 勇者の第二撃
		// 一定ダメージを受ける
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer / 3;
		// ダメージ状態のフラグをオン
		mIsDamaged = true;
		break;
	case ENEMY_ATTACK:
		break;
	default:
		break;
	}
	// 体力が0以下になったら、
	if (mHealth <= 0)
	{
		mIsDamaged = false;
		// 状態を死亡時に遷移
		mState = characterNS::DEATH;
		// 適当な方向へジャンプしながら画面外へと落ちていく
		if (rand() % 2 == 0)
			mVelocity.x = 32.0f * 2.0;
		else
			mVelocity.x = -32.0f * 2.0;
		mVelocity.y = -sqrt(2 * 2000.0f * 96 * 2);
		mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
	}
}