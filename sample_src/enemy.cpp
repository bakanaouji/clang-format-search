//==========================================================
/// @file
/// @brief    enemy.hの実装
/// @author   阿部拳之
///
/// @attention  雑魚敵を表すクラスです。
///				バリケードを破壊しながら城に向かって攻めてくるので、
///				プレイヤーは勇者を操作して倒します。

//==========================================================

#include "enemy.h"
#include <iostream>

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Enemy::Enemy() : Character()
{
	mSpriteData.width = enemyNS::WIDTH;								// 雑魚敵のサイズ
	mSpriteData.height = enemyNS::HEIGHT;
	mSpriteData.rect.bottom = enemyNS::HEIGHT;						// 画面の一部を選択
	mSpriteData.rect.right = enemyNS::WIDTH;
	mFrameDelay = enemyNS::MOVE_ANIMATION_DELAY;						// アニメーションの各フレームの間隔
																	// 初期の方向は右
	mGoalDirection = characterNS::RIGHT;								// 次に進むべき方向
	mOldDirection = mDirection;										// 攻撃直前に向いていた方向
	mStartFrame = enemyNS::MOVE_RIGHT_START_FRAME;					// アニメーションの最初のフレーム
	mEndFrame = enemyNS::MOVE_RIGHT_END_FRAME;						// アニメーションの最後のフレーム
	mOldStartFrame = mStartFrame;										// 攻撃アニメーション終了後に戻るべきアニメーションの最初のフレーム
	mOldEndFrame = mEndFrame;											// 攻撃アニメーション終了後に戻るべきアニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;										// 現在のフレームをアニメーションの最初のフレームにセット
	// Boxの衝突判定用
	mEdge.left = (LONG)(-enemyNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.right = (LONG)(enemyNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.top = (LONG)(-enemyNS::HEIGHT * mSpriteData.scale / 2.0);
	mEdge.bottom = (LONG)(enemyNS::HEIGHT * mSpriteData.scale / 2.0);

	mCanMakeDecesionMove = false;									// 移動に関しての意思決定を行えるかどうか
	mAttackAnimationMode = imageNS::VERTICAL;						// 攻撃時のアニメーションの遷移モード（水平、垂直）
	mMoveUpStartFrame = enemyNS::MOVE_UP_START_FRAME;				// 上方向移動のアニメーションの最初のフレーム
	mMoveUpEndFrame = enemyNS::MOVE_UP_END_FRAME;					// 上方向移動のアニメーションの最後のフレーム
	mMoveRightStartFrame = enemyNS::MOVE_RIGHT_START_FRAME;			// 右方向移動のアニメーションの最初のフレーム
	mMoveRightEndFrame = enemyNS::MOVE_RIGHT_END_FRAME;				// 右方向移動のアニメーションの最後のフレーム
	mMoveDownStartFrame = enemyNS::MOVE_DOWN_START_FRAME;			// 下方向移動のアニメーションの最初のフレーム
	mMoveDownEndFrame = enemyNS::MOVE_DOWN_END_FRAME;				// 下方向移動のアニメーションの最後のフレーム
	mMoveLeftStartFrame = enemyNS::MOVE_LEFT_START_FRAME;			// 左方向移動のアニメーションの最初のフレーム
	mMoveLeftEndFrame = enemyNS::MOVE_LEFT_END_FRAME;				// 左方向移動のアニメーションの最後のフレーム
	mAttackUpStartFrame = enemyNS::ATTACK_UP_START_FRAME;			// 上方向攻撃のアニメーションの最初のフレーム
	mAttackUpEndFrame = enemyNS::ATTACK_UP_END_FRAME;				// 上方向攻撃のアニメーションの最後のフレーム
	mAttackRightStartFrame = enemyNS::ATTACK_RIGHT_START_FRAME;		// 右方向攻撃のアニメーションの最初のフレーム
	mAttackRightEndFrame = enemyNS::ATTACK_RIGHT_END_FRAME;			// 右方向攻撃のアニメーションの最後のフレーム
	mAttackDownStartFrame = enemyNS::ATTACK_DOWN_START_FRAME;		// 下方向攻撃のアニメーションの最初のフレーム
	mAttackDownEndFrame = enemyNS::ATTACK_DOWN_END_FRAME;			// 下方向攻撃のアニメーションの最後のフレーム
	mAttackLeftStartFrame = enemyNS::ATTACK_LEFT_START_FRAME;		// 左方向攻撃のアニメーションの最初のフレーム
	mAttackLeftEndFrame = enemyNS::ATTACK_LEFT_END_FRAME;			// 左方向攻撃のアニメーションの最後のフレーム
	mAttackTime = enemyNS::ATTACK_TIME;								// プレイヤーに隣接してから攻撃に移るまでの溜め時間
	mContinueAttackTime = enemyNS::CONTINUE_ATTACK_TIME;				// 連続で攻撃する場合の溜め時間
	mDamagePer = 1.0f;
}

//==========================================================
// パラメータ初期化
//==========================================================
void Enemy::reset()
{
	Character::reset();
	// 初期の方向は右
	mOldDirection = mDirection;
	mGoalDirection = characterNS::RIGHT;
	// 移動に関しての意思決定は行えない
	mCanMakeDecesionMove = false;
}

//=============================================================================
// Update
// 攻撃完了直後の状態遷移や移動などの行動を行う。
// 全体的な状態遷移については、衝突判定によって行うためstage.collisions()に記載
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Enemy::update(const float frameTime)
{
	// 非アクティブなら何もしない
	if (!mActive)
		return;
	
	// 攻撃判定がでている場合はコリジョンを生成して当たり判定をとる
	if (mAttackCollisionFlag) {
		mAttackCollision.attack(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), mDirection);
	}
	// 攻撃中のエンティティを出すフラグをオフ
	mAttackCollisionFlag = false;
	// 移動に関しての意思決定はデフォルトでは行えない
	mCanMakeDecesionMove = false;

	// ここで状態遷移、座標の移動、アニメーションのリセットを行う
	// それぞれの状態ごとに処理を分岐
	switch (mState)
	{
	case characterNS::MOVE:	// 移動中なら目標の方向にまっすぐ進む
		// 目標の方向に向かって進む
		switch (mGoalDirection)
		{
		case characterNS::LEFT:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
			// 左に移動
			mSpriteData.x -= ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::RIGHT:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
			// 右に移動
			mSpriteData.x += ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::UP:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
			// 上に移動
			mSpriteData.y -= ENEMY_MOVE_SPEED * frameTime;
			break;
		case characterNS::DOWN:
			if (mDirection != mGoalDirection)
				changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
			// 左に移動
			mSpriteData.y += ENEMY_MOVE_SPEED * frameTime;
			break;
		}
		// 攻撃直後ではないはずなのでフラグをオフ
		mIsAttacked = false;
		// 画像のみアップデート
		Entity::updateOnlyImage(frameTime);
		if (isCenterOfTile())	// タイルの中央に来たらマップを更新
		{
			// マップに自分の位置を追加
			mMap->updateMapCol((float)(mTileY * mapNS::TEXTURE_SIZE), (float)(mTileX * mapNS::TEXTURE_SIZE), mMap->getMapCol(mTileY, mTileX) + mapNS::COL_ENEMY_INCLEMENT);
			// 移動に関しての意思決定が可能
			mCanMakeDecesionMove = true;
		}
		break;
	case characterNS::ATTACK:	// 攻撃中
		// アニメーションが終了していたら攻撃終了
		// 状態を移動時に戻す
		if (mAnimComplete)
		{
			// 移動に関しての意思決定が可能
			mCanMakeDecesionMove = true;
			// 攻撃直後なのでフラグをオン
			mIsAttacked = true;
			// 状態を移動中へと遷移
			changeStateToMove();
			// 向いている方向を戻す
			mDirection = mOldDirection;
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case characterNS::PRE_ATTACK:	// 攻撃直前
		// タイマーを加算、一定時間を超えたら攻撃に移る
		mAttackTimer += frameTime;
		// 攻撃直後ならcuntinueAttackTime後に攻撃
		if (mIsAttacked && mAttackTimer > mContinueAttackTime)
		{
			mAttackTimer = 0.0f;
			mState = characterNS::ATTACK;
			mAttackCollisionFlag = true;
		}
		// 攻撃直後でないならattackTime後に攻撃
		else if (!mIsAttacked && mAttackTimer > mAttackTime)
		{
			mAttackTimer = 0.0f;
			mState = characterNS::ATTACK;
			mAttackCollisionFlag = true;
		}
		break;
	case characterNS::WAIT:		// 待機中
		// 移動に関しての意思決定が可能
		mCanMakeDecesionMove = true;
		break;
	case characterNS::DEATH:	// 死亡中
		// 描画は可能
		mDrawFlag = true;
		// ダメージは無効
		mIsDamaged = false;
		// ジャンプ中なので、重力に従って減速をする
		mVelocity.y += frameTime * 2000.0f;
		// xy方向にジャンプ
		mSpriteData.x += frameTime * mVelocity.x;
		mSpriteData.y += frameTime * mVelocity.y;
		// ゲーム画面より下に出たら、エンティティを非アクティブ化
		if (mSpriteData.y > GAME_HEIGHT)
			dead();
	default:
		break;
	}

	// 状態遷移後の処理
	// 移動中はキーが押されている間のみアニメーションが進むようにしたいので、
	// エンティティとアニメーションの更新を独立に行っている
	switch (mState)
	{
	case characterNS::MOVE:
		updateMoving(frameTime);
		break;
	case characterNS::ATTACK:
		updateAttacking(frameTime);
		break;
	case characterNS::PRE_ATTACK:
		Entity::updateWithoutImage(frameTime);
		break;
	case characterNS::WAIT:
		Entity::updateWithoutImage(frameTime);
	default:
		Entity::update(frameTime);
		break;
	}

	// ダメージを受けているなら一定時間ごとにアニメーションを点滅
	if (mIsDamaged)
	{
		// ダメージ時にアニメーション用のタイマーを加算
		mDamageTimer += frameTime;
		mTotalDamageTime += frameTime;
		// 0.15秒ごとに描画フラグを切り替え
		if (mDamageTimer > 0.15f)
		{
			if (mDrawFlag)
				mDrawFlag = false;
			else
				mDrawFlag = true;
			mDamageTimer = 0.0f;
		}
		// DAMAGE_TIME分だけ点滅したら、点滅を終了
		if (mTotalDamageTime > enemyNS::DAMAGE_TIME)
		{
			mDamageTimer = 0.0f;
			mTotalDamageTime = 0.0f;
			mDrawFlag = true;
			mIsDamaged = false;
		}
	}
	// 攻撃用エンティティを更新
	mAttackCollision.update(frameTime);
}

//==========================================================
// ダメージ処理
// WEAPONの種類によって受けるダメージが分岐
//==========================================================
void Enemy::damage(const WEAPON weapon)
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
		mHealth -= BRAVE_FIRE_DAMAGE * mDamagePer;
		// ダメージ状態のフラグをオン
		mIsDamaged = true;
		break;
	case BRAVE_ATTACK:			// 勇者の攻撃
		// 一定ダメージを受ける
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer;
		// ダメージ状態のフラグをオン
		mIsDamaged = true;
		break;
	case BRAVE_SECOND_ATTACK:	// 勇者の第二撃
		// 一定ダメージを受ける
		mHealth -= BRAVE_ATTACK_DAMAGE * mDamagePer;
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

//==========================================================
// 向いている方向を変更するメソッド
//==========================================================
void Enemy::changeDirection(const int strF, const int endF)
{
	mDirection = mGoalDirection;
	mStartFrame = strF;
	mEndFrame = endF;
	setCurrentFrame(mStartFrame);
}

//==========================================================
// 一番近くのバリケードのインデックスを返す関数
//==========================================================
int Enemy::searchNearBarricadeIndex()
{
	// 最小の距離差
	int minX = 0, minY = 0;
	// 最小の距離のインデックス
	int minIndex = 0;
	// アクティブなバリケードの数を数える用
	int count = 0;

	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		// もしバリケードがアクティブなら
		if (mBarricades[i].getActive())
		{
			// 探索1つ目の場合
			if (count == 0)
			{
				minX = (int)abs(mBarricades[i].getTileX() - getTileX());
				minY = (int)abs(mBarricades[i].getTileY() - getTileY());
				minIndex = i;
			}
			else // 2つ目以降
			{
				if ((mBarricades[i].getTileX() - getTileX())*(mBarricades[i].getTileX() - getTileX()) +
					(mBarricades[i].getTileY() - getTileY())*(mBarricades[i].getTileY() - getTileY()) <
					minX * minX + minY * minY)
				{
					// 新たに代入
					minX = (int)abs(mBarricades[i].getTileX() - getTileX());
					minY = (int)abs(mBarricades[i].getTileY() - getTileY());
					minIndex = i;
				}
			}
			// カウントを増やす
			count++;
		}
	}
	mNearBarricadeIndex =  minIndex;
	return minIndex;
}

//==========================================================
// 雑魚敵から城までの直線上にバリケードが存在したらtrue、存在しない場合はfalseを返す。
//==========================================================
bool Enemy::checkBarricadeOnLine() {
	// 雑魚敵の上下左右それぞれに対して、雑魚敵から近い順に城がないかをチェック
	// それぞれの方向について、城以外のオブジェクトが先に見つかった場合はtrueを返す
	// いずれかの方向について、城までの直線状になんのオブジェクトもない場合はfalseを返す
	for (int x = mTileX; x <= mapNS::MAP_WIDTH; ++x)
	{
		if (mMap->getMapObj(mTileY, x) == mapNS::OBJ_BARRICADE || mMap->getMapCol(mTileY, x) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(mTileY, x) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::RIGHT;
			return false;
		}
	}
	for (int x = mTileX; x >= 0; --x)
	{
		if (mMap->getMapObj(mTileY, x) == mapNS::OBJ_BARRICADE || mMap->getMapCol(mTileY, x) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(mTileY, x) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::LEFT;
			return false;
		}
	}
	for (int y = mTileY; y <= mapNS::MAP_HEIGHT; ++y)
	{
		if (mMap->getMapObj(y, mTileX) == mapNS::OBJ_BARRICADE || mMap->getMapCol(y, mTileX) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(y, mTileX) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::DOWN;
			return false;
		}
	}
	for (int y = mTileY; y >= 0; --y)
	{
		if (mMap->getMapObj(y, mTileX) == mapNS::OBJ_BARRICADE || mMap->getMapCol(y, mTileX) == mapNS::COL_ROCK)
			break;
		if (mMap->getMapCol(y, mTileX) == mapNS::COL_CASTLE)
		{
			mGoalDirection = characterNS::UP;
			return false;
		}
	}
	return true;
}

//==========================================================
// 攻撃モードにチェンジするときに呼び出す関数
//==========================================================
void Enemy::changeAttack(VECTOR2 &collisionVector)
{
	// 攻撃直前モードにチェンジ
	mLoop = false;
	// 衝突ベクトルを利用して、衝突対象がある方向を向くように設定
	float yPerx = (float)collisionVector.y / collisionVector.x;
	float hPerw = (float)mSpriteData.height / mSpriteData.width;
	if (collisionVector.x > 0)
	{
		if (yPerx > -hPerw && yPerx < hPerw)
		{
			mGoalDirection = characterNS::RIGHT;
			changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
		}
		else if (yPerx < -hPerw)
		{
			mGoalDirection = characterNS::UP;
			changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		}
		else
		{
			mGoalDirection = characterNS::DOWN;
			changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		}
	}
	else
	{
		if (yPerx > -hPerw && yPerx < hPerw)
		{
			mGoalDirection = characterNS::LEFT;
			changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
		}
		else if (yPerx < -hPerw)
		{
			mGoalDirection = characterNS::DOWN;
			changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		}
		else
		{
			mGoalDirection = characterNS::UP;
			changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		}
	}
	// 次に進むべき方向はリセット
	mGoalDirection = characterNS::NONE;
	// 攻撃直前に状態を遷移
	mState = characterNS::PRE_ATTACK;
	// アニメーションのモードを設定
	mMode = mAttackAnimationMode;
	// アニメーション終了時にフレームを戻すために保存
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	mOldDirection = mDirection;

	// 方向に応じてアニメーションを切り替え
	switch (mDirection)
	{
	case characterNS::LEFT:
		mStartFrame = mAttackLeftStartFrame;
		mEndFrame = mAttackLeftEndFrame;
		break;
	case characterNS::RIGHT:
		mStartFrame = mAttackRightStartFrame;
		mEndFrame = mAttackRightEndFrame;
		break;
	case characterNS::UP:
		mStartFrame = mAttackUpStartFrame;
		mEndFrame = mAttackUpEndFrame;
		break;
	case characterNS::DOWN:
		mStartFrame = mAttackDownStartFrame;
		mEndFrame = mAttackDownEndFrame;
		break;
	default:
		break;
	}
	setCurrentFrame(mStartFrame);
	return;
}

//==========================================================
// 攻撃モードにチェンジするときに呼び出す関数
//==========================================================
void Enemy::changeAttack(const characterNS::DIRECTION dir)
{
	// 攻撃直前モードにチェンジ
	mLoop = false;
	// 方向を変更
	mGoalDirection = dir;
	switch (dir)
	{
	case characterNS::LEFT:
		changeDirection(mMoveLeftStartFrame, mMoveLeftEndFrame);
		break;
	case characterNS::RIGHT:
		changeDirection(mMoveRightStartFrame, mMoveRightEndFrame);
		break;
	case characterNS::UP:
		changeDirection(mMoveUpStartFrame, mMoveUpEndFrame);
		break;
	case characterNS::DOWN:
		changeDirection(mMoveDownStartFrame, mMoveDownEndFrame);
		break;
	case characterNS::NONE:
		break;
	default:
		break;
	}
	// 次に進むべき方向はリセット
	mGoalDirection = characterNS::NONE;
	// 攻撃直前に状態を遷移
	mState = characterNS::PRE_ATTACK;
	// アニメーションのモードを設定
	mMode = mAttackAnimationMode;
	// アニメーション終了時にフレームを戻すために保存
	mOldStartFrame = mStartFrame;
	mOldEndFrame = mEndFrame;
	mOldDirection = mDirection;
	// 方向に応じてアニメーションを切り替え
	switch (mDirection)
	{
	case characterNS::LEFT:
		mStartFrame = mAttackLeftStartFrame;
		mEndFrame = mAttackLeftEndFrame;
		break;
	case characterNS::RIGHT:
		mStartFrame = mAttackRightStartFrame;
		mEndFrame = mAttackRightEndFrame;
		break;
	case characterNS::UP:
		mStartFrame = mAttackUpStartFrame;
		mEndFrame = mAttackUpEndFrame;
		break;
	case characterNS::DOWN:
		mStartFrame = mAttackDownStartFrame;
		mEndFrame = mAttackDownEndFrame;
		break;
	default:
		break;
	}
	setCurrentFrame(mStartFrame);
	return;
}

//==========================================================
// タイルの中央にキャラクターがいるかどうか
//==========================================================
bool Enemy::isCenterOfTile()
{
	bool isCenter = false;
	// 上下左右どこに移動しているかによって中央にいるかの判定を分岐
	// 基本的には、今いるタイルを跨いだらタイルを移動して中央に来たと判定
	switch (mGoalDirection)
	{
	case characterNS::RIGHT:
		if (mSpriteData.x / 32 >= mTileX + 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileX += 1;
			isCenter = true;
		}
		break;
	case characterNS::LEFT:
		if (mSpriteData.x / 32 <= mTileX - 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileX -= 1;
			isCenter = true;
		}
		break;
	case characterNS::UP:
		if (mSpriteData.y / 32 <= mTileY - 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileY -= 1;
			isCenter = true;
		}
		break;
	case characterNS::DOWN:
		if (mSpriteData.y / 32 >= mTileY + 1)
		{
			mMap->updateMapCol((float)mTileY * 32, (float)mTileX * 32, mMap->getMapCol(mTileY, mTileX) - mapNS::COL_ENEMY_INCLEMENT);
			mTileY += 1;
			isCenter = true;
		}
		break;
	}
	return isCenter;
}

//==========================================================
// 指定した方向に進み始められるかどうか
//==========================================================
bool Enemy::canMoveTo(const characterNS::DIRECTION dir)
{
	// 指定した方向のタイルに障害物があれば移動できない
	// なければ移動可能
	switch (dir)
	{
	case characterNS::LEFT:
		if (mMap->getMapCol(mTileY, mTileX - 1) >= 1)
		{
			return false;
		}
		break;
	case characterNS::RIGHT:
		if (mMap->getMapCol(mTileY, mTileX + 1) >= 1)
		{
			return false;
		}
		break;
	case characterNS::UP:
		if (mMap->getMapCol(mTileY - 1, mTileX) >= 1)
		{
			return false;
		}
		break;
	case characterNS::DOWN:
		if (mMap->getMapCol(mTileY + 1, mTileX) >= 1)
		{
			return false;
		}
		break;
	}
	return true;
}