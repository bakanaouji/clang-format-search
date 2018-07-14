//==========================================================
/// @file
/// @brief    character.hの実装
/// @author   阿部拳之
///
/// @attention  プレイヤーや敵クラスの基本となるクラス。
///				継承することで使用可能です。
///				継承なしではオブジェクトの生成はできません。

//==========================================================

#include "character.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Character::Character() : Entity()
{
	// 初期の方向は右
	mDirection = characterNS::RIGHT;
	// Boxの衝突判定を使用
	mCollisionType = entityNS::BOX;
	// 攻撃判定のコリジョンは最初はなし
	mAttackCollisionFlag = false;
	// 状態は何もしない状態からスタート
	mState = characterNS::STATE::MOVE;
}

//==========================================================
// パラメータ初期化
//==========================================================
void Character::reset()
{
	// 攻撃アニメーション終了後に戻るべきアニメーションフレーム保存用
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	// 初期の方向は右
	mDirection = characterNS::RIGHT;
	// 攻撃判定のコリジョンはなしでリセット
	mAttackCollisionFlag = false;
	// 状態は何もしない状態からスタート
	mState = characterNS::STATE::MOVE;
	// エンティティをリセット
	Entity::reset();
}

//==========================================================
// 移動時のアップデート関数
// アニメーション以外をアップデート
//==========================================================
void Character::updateMoving(const float frameTime)
{
	// アニメーションのアップデートは単独で行われるのでそれ以外をアップデート
	Entity::updateWithoutImage(frameTime);
}

//==========================================================
// 攻撃時のアップデート関数
// 現状では通常のアップデート
//==========================================================
void Character::updateAttacking(const float frameTime)
{
	Entity::update(frameTime);
}

//==========================================================
// 死亡時に呼び出される関数
//==========================================================
void Character::dead()
{
	// キャラクターを非アクティブにする
	mActive = false;
	mVisible = false;
	mHealth = 0;
}

//==========================================================
// 移動可能かチェック
//==========================================================
bool Character::checkCanMove(const float x, const float y)
{
	// 1マス32pixelのため32で割る
	// -16はめり込みを防止するために半マス分引いてる
	// +αは微調整…
	int map_x = (int)((x - 16) / 32) + 1;
	int map_y = (int)((y - 16) / 32) + 1;
	if (map_x <= 0)
		map_x = 0;
	if (map_x >= mapNS::MAP_WIDTH)
		map_x = mapNS::MAP_WIDTH - 1;
	if (map_y <= 0)
		map_y = 0;
	if (map_y >= mapNS::MAP_HEIGHT)
		map_y = mapNS::MAP_HEIGHT - 1;

	// 進もうとするマップタイルに障害物があった場合、
	if (mMap->getMapCol(map_y, map_x) == mapNS::COL_ROCK || mMap->getMapCol(map_y, map_x) == mapNS::COL_CASTLE)
	{
		// 移動不可
		return false;
	}
	else  // 進もうとするマップタイルに障害物がない場合、
	{
		// 移動可能
		return true;
	}
}

//==========================================================
// 向いている方向をセットする
//==========================================================
void Character::setDirection(const characterNS::DIRECTION dir)
{
	// セットするキャラクターの向きによって分岐
	switch (dir)
	{
	case characterNS::LEFT:
		// 左方向を向いていなければ左方向にアニメーションをリセット
		if (mDirection != characterNS::DIRECTION::LEFT)
		{
			mDirection = characterNS::DIRECTION::LEFT;
			mStartFrame = mMoveLeftStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveLeftStartFrame;
			mEndFrame = mMoveLeftEndFrame;
			setRect();
		}
		break;
	case characterNS::RIGHT:
		// 右方向を向いていなければ右方向にアニメーションをリセット
		if (mDirection != characterNS::DIRECTION::RIGHT)
		{
			mDirection = characterNS::DIRECTION::RIGHT;
			mStartFrame = mMoveRightStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveRightStartFrame;
			mEndFrame = mMoveRightEndFrame;
			setRect();
		}
		break;
	case characterNS::UP:
		// 上方向を向いていなければ上方向にアニメーションをリセット
		if (mDirection != characterNS::DIRECTION::UP)
		{
			mDirection = characterNS::DIRECTION::UP;
			mStartFrame = mMoveUpStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveUpStartFrame;
			mEndFrame = mMoveUpEndFrame;
			setRect();
		}
		break;
	case characterNS::DOWN:
		// 下方向を向いていなければ下方向にアニメーションをリセット
		if (mDirection != characterNS::DIRECTION::DOWN)
		{
			mDirection = characterNS::DIRECTION::DOWN;
			mStartFrame = mMoveDownStartFrame;
			mCurrentFrame = mEndFrame - mCurrentFrame + mMoveDownStartFrame;
			mEndFrame = mMoveDownEndFrame;
			setRect();
		}
		break;
	default:
		break;
	}
}

//==========================================================
// 移動中へと状態を遷移
//==========================================================
void Character::changeStateToMove()
{
	// 状態は移動中へと遷移
	mState = characterNS::MOVE;
	// アニメーションフレームの遷移は水平方向
	mMode = imageNS::HORIZONTAL;
	// 移動中はアニメーションはループさせる
	mLoop = true;
	// アニメーションを向いている方向に合わせてセット
	mStartFrame = mOldStartFrame;
	mEndFrame = mOldEndFrame;
	setCurrentFrame(mStartFrame);
}