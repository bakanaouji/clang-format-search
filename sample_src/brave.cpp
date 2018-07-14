//==========================================================
/// @file
/// @brief    brave.hの実装
/// @author   阿部拳之
///
/// @attention  プレイヤーが操作する勇者を表すクラスです。
///				プレイヤーが操作をして移動、攻撃、必殺技、ガードを駆使して敵を倒します。

//==========================================================

#include "brave.h"

//==========================================================
// デフォルトコンストラクタ
//==========================================================
Brave::Brave() : Character()
{
	mSpriteData.width = braveNS::WIDTH;							// 勇者のサイズ
	mSpriteData.height = braveNS::HEIGHT;
	mSpriteData.x = braveNS::X;									// 画面上の位置
	mSpriteData.y = braveNS::Y;
	mSpriteData.rect.bottom = braveNS::HEIGHT;					// 画面の一部を選択
	mSpriteData.rect.right = braveNS::WIDTH;
	mFrameDelay = braveNS::ANIMATION_DELAY;						// アニメーションの各フレームの間隔
	mStartFrame = braveNS::MOVE_UP_START_FRAME;					// アニメーションの最初のフレーム
	mEndFrame = braveNS::MOVE_UP_END_FRAME;						// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;								// 現在のフレームはアニメーションの最初のフレームに設定
	// Boxの衝突判定用
	mEdge.left = (LONG)(-braveNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.right = (LONG)(braveNS::WIDTH * mSpriteData.scale / 2.0);
	mEdge.top = (LONG)(-braveNS::HEIGHT * mSpriteData.scale / 2.0);
	mEdge.bottom = (LONG)(braveNS::HEIGHT * mSpriteData.scale / 2.0);

	mSecondAttackFlag = false;									// 二連撃目の攻撃フラグはオフ
	mMpTimer = 0.0;												// MP回復用のタイマー
	mMagicPoint = 100;											// MPはMAX100でスタート
	mMoveUpStartFrame = braveNS::MOVE_UP_START_FRAME;			// 上方向移動のアニメーションの最初のフレーム
	mMoveUpEndFrame = braveNS::MOVE_UP_END_FRAME;				// 上方向移動のアニメーションの最後のフレーム
	mMoveRightStartFrame = braveNS::MOVE_RIGHT_START_FRAME;		// 右方向移動のアニメーションの最初のフレーム
	mMoveRightEndFrame = braveNS::MOVE_RIGHT_END_FRAME;			// 右方向移動のアニメーションの最後のフレーム
	mMoveDownStartFrame = braveNS::MOVE_DOWN_START_FRAME;		// 下方向移動のアニメーションの最初のフレーム
	mMoveDownEndFrame = braveNS::MOVE_DOWN_END_FRAME;			// 下方向移動のアニメーションの最後のフレーム
	mMoveLeftStartFrame = braveNS::MOVE_LEFT_START_FRAME;		// 左方向移動のアニメーションの最初のフレーム
	mMoveLeftEndFrame = braveNS::MOVE_LEFT_END_FRAME;			// 左方向移動のアニメーションの最後のフレーム
	mFrameDelay = braveNS::ANIMATION_DELAY;						// アニメーションのフレーム間の間隔（秒）
	mAttackUpStartFrame = braveNS::UP_ATTACK_START_FRAME;		// 上方向攻撃のアニメーションの最初のフレーム
	mAttackUpEndFrame = braveNS::UP_ATTACK_END_FRAME;			// 上方向攻撃のアニメーションの最後のフレーム
	mAttackRightStartFrame = braveNS::RIGHT_ATTACK_START_FRAME;	// 右方向攻撃のアニメーションの最初のフレーム
	mAttackRightEndFrame = braveNS::RIGHT_ATTACK_END_FRAME;		// 右方向攻撃のアニメーションの最後のフレーム
	mAttackDownStartFrame = braveNS::DOWN_ATTACK_START_FRAME;	// 下方向攻撃のアニメーションの最初のフレーム
	mAttackDownEndFrame = braveNS::DOWN_ATTACK_END_FRAME;		// 下方向攻撃のアニメーションの最後のフレーム
	mAttackLeftStartFrame = braveNS::LEFT_ATTACK_START_FRAME;	// 左方向攻撃のアニメーションの最初のフレーム
	mAttackLeftEndFrame = braveNS::LEFT_ATTACK_END_FRAME;		// 左方向攻撃のアニメーションの最後のフレーム
}

//==========================================================
// パラメータ初期化
//==========================================================
void Brave::reset()
{
	mMpTimer = 0.0;												// MP回復用のタイマー
	mMagicPoint = 100;											// MPはMAX100でスタート
	mSpriteData.x = braveNS::X;									// 画面上の位置
	mSpriteData.y = braveNS::Y;
	mFrameDelay = braveNS::ANIMATION_DELAY;						// アニメーションのフレーム間の間隔（秒）
	mStartFrame = braveNS::MOVE_UP_START_FRAME;					// アニメーションの最初のフレーム
	mEndFrame = braveNS::MOVE_UP_END_FRAME;						// アニメーションの最後のフレーム
	mCurrentFrame = mStartFrame;								// 現在のフレームはアニメーションの最初のフレームに設定
	mSecondAttackFlag = false;									// 二連撃目の攻撃フラグはオフ
	mFire.reset();												// 炎を初期化
	mBraveAttackCollision.reset();								// 攻撃エンティティを初期化
	Character::reset();
}

//=============================================================================
// Update
// キーボードからの入力を受け付け、それに応じて移動や攻撃などの行動を行う。
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================	
void Brave::update(const float frameTime)
{
	// エンティティが非アクティブなら、何もしない
	if (!mActive)
		return;


	// 勇者の攻撃判定がでている場合はコリジョンを生成して当たり判定をとる
	if (mAttackCollisionFlag)
		mBraveAttackCollision.attack(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), mDirection);

	// FIRE_KEYに対応するキーが押されたら勇者から炎を発射
	if (mInput->isKeyDown(BRAVE_FIRE_KEY))
		mFire.fire(getCenterX(), getCenterY(), (float)getWidth(), (float)getHeight(), getScale(), mMagicPoint, mDirection);

	// 攻撃用のエンティティを出現させるフラグをオフ
	mAttackCollisionFlag = false;

	// ここで状態遷移、座標の移動、アニメーションのリセットを行う
	// それぞれの状態ごとに処理を分岐
	switch (mState)
	{
	case characterNS::MOVE:		// 移動時はすべてのキーの入力を受け付ける
		// 上下左右キーが入力された場合、
		if (mInput->isKeyDown(BRAVE_LEFT_KEY) || mInput->isKeyDown(BRAVE_RIGHT_KEY) || mInput->isKeyDown(BRAVE_UP_KEY) || mInput->isKeyDown(BRAVE_DOWN_KEY))
		{
			// 入力に応じて勇者の向きを変更、移動
			changeWithMove(frameTime);
		}
		if (mInput->isKeyDown(BRAVE_ATTACK_KEY))	// 攻撃キーが押された場合、
		{
			// 攻撃用オーディオを再生
			mAudio->playCue("kill");
			// 状態を攻撃中に遷移
			mState = characterNS::ATTACK;
			// アニメーションフレームの遷移は垂直方向
			mMode = imageNS::VERTICAL;
			// アニメーション終了時にフレームを戻すために保存
			mOldStartFrame = mStartFrame;
			mOldEndFrame = mEndFrame;
			// 攻撃用のエンティティの出現フラグをオン
			mAttackCollisionFlag = true;
			// 向いている方向でアニメーションを分岐
			mLoop = false;
			switch (mDirection)
			{
			case characterNS::DOWN:
				mStartFrame = mAttackDownStartFrame;
				mEndFrame = mAttackDownEndFrame;
				break;
			case characterNS::RIGHT:
				mStartFrame = mAttackRightStartFrame;
				mEndFrame = mAttackRightEndFrame;
				break;
			case characterNS::LEFT:
				mStartFrame = mAttackLeftStartFrame;
				mEndFrame = mAttackLeftEndFrame;
				break;
			case characterNS::UP:
				mStartFrame = mAttackUpStartFrame;
				mEndFrame = mAttackUpEndFrame;
				break;
			}
			setCurrentFrame(mStartFrame);
		}
		else if (mInput->isKeyDown(BRAVE_GAURD_KEY))	// ガードキーが押された場合、
		{
			// 状態をガード中に遷移
			mState = characterNS::GAURD;
			// アニメーション終了時にフレームを戻すために保存
			mOldStartFrame = mStartFrame;
			mOldEndFrame = mEndFrame;
			// 向いている方向でアニメーションを分岐
			switch (mDirection)
			{
			case characterNS::DOWN:
				mStartFrame = braveNS::DOWN_GUARD_START_FRAME;
				mEndFrame = braveNS::DOWN_GUARD_END_FRAME;
				break;
			case characterNS::RIGHT:
				mStartFrame = braveNS::RIGHT_GUARD_START_FRAME;
				mEndFrame = braveNS::RIGHT_GUARD_END_FRAME;
				break;
			case characterNS::LEFT:
				mStartFrame = braveNS::LEFT_GUARD_START_FRAME;
				mEndFrame = braveNS::LEFT_GUARD_END_FRAME;
				break;
			case characterNS::UP:
				mStartFrame = braveNS::UP_GUARD_START_FRAME;
				mEndFrame = braveNS::UP_GUARD_END_FRAME;
				break;
			}
			setCurrentFrame(mStartFrame);
		}
		break;
	case characterNS::ATTACK:	// 攻撃時はアニメーションが終了するまで第二段攻撃か移動の入力しか受け付けない
		// 上下左右キーが入力された場合、
		if (mInput->isKeyDown(BRAVE_LEFT_KEY) || mInput->isKeyDown(BRAVE_RIGHT_KEY) || mInput->isKeyDown(BRAVE_UP_KEY) || mInput->isKeyDown(BRAVE_DOWN_KEY))
		{
			// 入力に応じて勇者の向きを変更、移動
			changeWithMove(frameTime);
		}
		// 攻撃中にさらに攻撃ボタンが押された場合、
		if (mInput->isKeyDown(BRAVE_ATTACK_KEY) && mCurrentFrame > mStartFrame + 2)
		{
			// 第二撃攻撃のフラグをオン
			mSecondAttackFlag = true;
		}
		// 攻撃アニメーションが終了した場合、
		if (mAnimComplete)
		{
			// 第二撃攻撃のフラグがオンなら
			if (mSecondAttackFlag)
			{
				// 状態は第二撃攻撃中へと遷移
				mState = characterNS::SECOND_ATTACK;
				// 現在の向きに合わせてアニメーションを再度リセット
				mLoop = false;
				switch (mDirection)
				{
				case characterNS::DOWN:
					mStartFrame = braveNS::DOWN_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::DOWN_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::RIGHT:
					mStartFrame = braveNS::RIGHT_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::RIGHT_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::UP:
					mStartFrame = braveNS::UP_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::UP_SECOND_ATTACK_END_FRAME;
					break;
				case characterNS::LEFT:
					mStartFrame = braveNS::LEFT_SECOND_ATTACK_START_FRAME;
					mEndFrame = braveNS::LEFT_SECOND_ATTACK_END_FRAME;
					break;
				}
				setCurrentFrame(mStartFrame);
				// 第二撃攻撃のフラグをオフ
				mSecondAttackFlag = false;
				// 第二撃攻撃中は軽くジャンプしながら攻撃する
				mVelocity.y = -sqrt(2 * 2000.0f * braveNS::HEIGHT);
				// ジャンプを開始した直後のY座標を保存
				mTmpY = getY();
				// 攻撃のエンティティの出現フラグをオン
				mAttackCollisionFlag = true;
			}
			else
			// 第二撃攻撃のフラグがオフなら
			{
				// 状態を移動中へと遷移
				changeStateToMove();
			}
			// アニメーションのみ更新（エンティティは更新しない）
			Entity::updateOnlyImage(frameTime);
		}
		break;
	case characterNS::SECOND_ATTACK:	// 第二段攻撃時はアニメーションが終了するまで入力を受け付けない
		// ジャンプ中なので、重力に従って減速をする
		mVelocity.y += frameTime * 2000.0f;
		// y方向の速度 = -ジャンプを開始したときの初速度の場合（ジャンプ前のy座標まで戻っていることが期待される）、
		if (mVelocity.y > sqrt(2 * 2000.0f * braveNS::HEIGHT))
		{
			// 若干のずれを補正するために、強制的にy座標をジャンプ前の座標へと戻す
			setY(mTmpY);
			// 状態を移動中へと遷移
			changeStateToMove();
			// アニメーションのみ更新（エンティティは更新しない）
			Entity::updateOnlyImage(frameTime);
			// y方向への速度を0にしてジャンプ終了
			mVelocity.y = 0.0f;
		}
		break;
	case characterNS::GAURD:	// ボタンが離されたらガード終了
		if (!mInput->isKeyDown(BRAVE_GAURD_KEY))
		{
			// 状態を移動中へと遷移
			changeStateToMove();
		}
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
	case characterNS::GAURD:
		Entity::update(frameTime);
		break;
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
		if (mTotalDamageTime > braveNS::DAMAGE_TIME)
		{
			mDamageTimer = 0.0f;
			mTotalDamageTime = 0.0f;
			mDrawFlag = true;
			mIsDamaged = false;
		}
	}

	// MPを一定時間ごとに回復
	mMpTimer += frameTime;
	if (mMpTimer > BRAVE_MP_RECOVERY_TIME)
	{
		mMpTimer = 0.0f;
		mMagicPoint += BRAVE_MP_RECOVERY;
		// MAX100を超えたら、100にする
		if (mMagicPoint > 100)
			mMagicPoint = 100;
	}

	// 移動可能だったら（第二撃攻撃中はジャンプ中なので移動を強制的に許可）
	if (checkCanMove(mSpriteData.x + frameTime * mVelocity.x, mSpriteData.y + frameTime * mVelocity.y) || mState == characterNS::SECOND_ATTACK)
	{
		mSpriteData.x += frameTime * mVelocity.x;     // キャラをX方向に動かす
		mSpriteData.y += frameTime * mVelocity.y;     // キャラをY方向に動かす
	}

	// 画面の端まで来たら進めない
	if (mSpriteData.x > GAME_WIDTH - braveNS::WIDTH * getScale())	// 画面右端を超えたら
		mSpriteData.x = GAME_WIDTH - braveNS::WIDTH * getScale();	// 画面右端に移動
	if (mSpriteData.x < 0)											// 画面左端を超えたら
		mSpriteData.x = 0;											// 画面左端に移動
	if (mSpriteData.y < rectNS::HEIGHT - 10)							// 画面上端を超えたら
		mSpriteData.y = rectNS::HEIGHT - 10;							// 画面上端に移動
	if (mSpriteData.y > GAME_HEIGHT - braveNS::HEIGHT * getScale())  // 画面下端を超えたら
		mSpriteData.y = GAME_HEIGHT -braveNS::HEIGHT * getScale();	// 画面下端に移動


	// 勇者の攻撃コリジョンを更新
	mBraveAttackCollision.update(frameTime);
	// 炎を更新
	mFire.update(frameTime);
}

//==========================================================
// ダメージ処理
// WEAPONの種類によって受けるダメージが分岐
//==========================================================
void Brave::damage(const WEAPON weapon)
{
	// 非アクティブな場合、ダメージは受けない
	if (!mActive)
		return;

	// WEAPONによってダメージを分岐
	switch (weapon)
	{
	case FIRE:					// 炎
		break;
	case BRAVE_ATTACK:			// 勇者の攻撃
		break;
	case BRAVE_SECOND_ATTACK:	// 勇者の第二撃
		break;
	case ENEMY_ATTACK:			// 雑魚敵
		// ガード中なら、ダメージの代わりにMPを消費
		if (mState == characterNS::GAURD)
			mMagicPoint -= 25;
		else  // ガード中でないなら、
		{
			// 一定ダメージを受ける
			mHealth -= ENEMY_ATTACK_DAMAGE * mDamagePer;
			// ダメージ状態のフラグをオン
			mIsDamaged = true;
		}
		break;
	case MIDBOSS_ATTACK:		// 中ボス
		// ガード中なら、ダメージの代わりにMPを消費
		if (mState == characterNS::GAURD)
			mMagicPoint -= 35;
		else  // ガード中でないなら、
		{
			// 一定ダメージを受ける
			mHealth -= MIDBOSS_ATTACK_DAMAGE * mDamagePer;
			// ダメージ状態のフラグをオン
			mIsDamaged = true;
		}
		break;
	default:
		break;
	}
	// 体力が0以下になったなら、死亡
	if (mHealth <= 0)
		dead();
}

//==========================================================
// 向いている方向とアニメーションを切り替える
// 内部的にのみ使用
//==========================================================
void Brave::changeWithMove(const float frameTime)
{
	float r = 1.0f;
	if (mState == BRAVE_ATTACK)
		r = 2.0f;
	// 左キーが入力された場合、
	if (mInput->isKeyDown(BRAVE_LEFT_KEY))
	{
		// 向いている方向を左に
		setDirection(characterNS::LEFT);
		// 移動可能だったら
		if (checkCanMove(mSpriteData.x - BRAVE_MOVE_SPEED * frameTime, mSpriteData.y)) {
			// 左に移動
			mSpriteData.x -= BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// 右キーが入力された場合、
	if (mInput->isKeyDown(BRAVE_RIGHT_KEY))
	{
		// 向いている方向を右に
		setDirection(characterNS::RIGHT);
		// 移動可能だったら
		if (checkCanMove(mSpriteData.x + BRAVE_MOVE_SPEED * frameTime, mSpriteData.y)) {
			// 右に移動
			mSpriteData.x += BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// 上キーが入力された場合、
	if (mInput->isKeyDown(BRAVE_UP_KEY))
	{
		// 向いている方向を上に
		setDirection(characterNS::UP);
		// 移動可能だったら
		if (checkCanMove(mSpriteData.x, mSpriteData.y - BRAVE_MOVE_SPEED * frameTime)) {
			// 上に移動
			mSpriteData.y -= BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// 下キーが入力された場合、
	if (mInput->isKeyDown(BRAVE_DOWN_KEY))
	{
		// 向いている方向を下に
		setDirection(characterNS::DOWN);
		// 移動可能だったら
		if (checkCanMove(mSpriteData.x, mSpriteData.y + BRAVE_MOVE_SPEED * frameTime)) {
			// 下に移動
			mSpriteData.y += BRAVE_MOVE_SPEED / r * frameTime;
		}
	}
	// アニメーションのみ更新（エンティティは更新しない）
	Entity::updateOnlyImage(frameTime);
}