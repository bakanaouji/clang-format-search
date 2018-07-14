//==========================================================
/// @file
/// @brief    stage.hの実装
/// @author   阿部拳之
///
/// @attention  ステージ情報を管理するクラスです。
///				勇者、敵、マップ情報、バリケード、城などステージに関する情報はここで管理します。

#include "stage.h"
#include <time.h>

//==========================================================
// コンストラクタ
//==========================================================
Stage::Stage()
{
	mRoundOver = false;			// ラウンドが終了しているかどうか（ゲームオーバーかどうか）
	mClearedStage = false;		// ステージをクリアしたかどうか
	mEnemyNum = 0;				// 敵の数
	mEnemy = NULL;				// 敵へのポインタの配列
	mEnemyX = NULL;				// 敵の初期位置Xの配列
	mEnemyY = NULL;				// 敵の初期位置Yの配列
	mEnemyWaveNum = 0;			// 現在の敵の波の番号（第一波、第二派、・・・）
	mRemainingTime = 1500.0f;	// ゲーム内の残り時間
	mStageNum = 0;				// 選択しているステージ番号
}

//==========================================================
// デストラクタ
//==========================================================
Stage::~Stage()
{
	// 敵のメモリを解放
	for (int i = 0; i < mEnemyNum; ++i)
	{
		safeDelete(mEnemy[i]);
	}
	safeDeleteArray(mEnemy);
	safeDeleteArray(mEnemyX);
	safeDeleteArray(mEnemyY);
}
//==========================================================
// ステージ情報を初期化
//==========================================================
void Stage::reset() 
{
	mRoundOver = false;			// ラウンドが終了しているかどうか（ゲームオーバーかどうか）
	mClearedStage = false;		// ステージをクリアしたかどうか
	mEnemyNum = 0;				// 敵の数
	mEnemy = NULL;				// 敵へのポインタの配列
	mEnemyX = NULL;				// 敵の初期位置Xの配列
	mEnemyY = NULL;				// 敵の初期位置Yの配列
	mEnemyWaveNum = 0;			// 現在の敵の波の番号（第一波、第二派、・・・）
	mRemainingTime = 1500.0f;	// ゲーム内の残り時間
	mStageNum = 0;				// 選択しているステージ番号
}

//==========================================================
// ゲームの新しいラウンドを開始
//==========================================================
void Stage::roundStart()
{
	// 敵情報をリセット
	mEnemyWaveNum = 0;
	mEnemyNum = 0;
	// マップデータを読み込み
	mMap.readMapFile(mStageNum + 1);
	// 敵データの読み込み
	initializeEnemies(mStageNum + 1, mEnemyWaveNum + 1);
	// 各バリケードを初期化
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		mBarricades[i].reset();
	}
	// 初期化
	// マップとバリケードを初期化
	int count = 0;  // バリケードの数数える用
	for (int row = 0; row<mapNS::MAP_HEIGHT; row++)       // マップの各行を処理
	{
		mMap.setY((float)(row*mapNS::TEXTURE_SIZE));      // タイルのYを設定
		for (int col = 0; col<mapNS::MAP_WIDTH; col++)    // マップの各列を処理
		{
			if (mMap.getMapData(row, col) >= 0)           // タイルが存在する場合（この処理はいらないと思われるが念のため）
			{
				mMap.setCurrentFrame(mMap.getMapData(row, col));                // タイルのテクスチャを設定
				mMap.setX((float)(col*mapNS::TEXTURE_SIZE));                    // タイルのXを設定
			}
			if (mMap.getMapObj(row, col) == mapNS::OBJ_BARRICADE)
			{
				mBarricades[count].setX((float)(col*mapNS::TEXTURE_SIZE));		// オブジェクトのXを設定
				mBarricades[count].setY((float)(row*mapNS::TEXTURE_SIZE));		// オブジェクトのYを設定
				mBarricades[count].setTileX(col);								// オブジェクトのXを設定
				mBarricades[count].setTileY(row);								// オブジェクトのYを設定
				if (mBarricades[count].getX() > -mapNS::TEXTURE_SIZE && mBarricades[count].getX() < GAME_WIDTH)	// オブジェクトが画面上にあるかどうか
				{
					count++;
				}
			}
		}
	}
	// 勇者を初期化
	mBrave.reset();
	// 各敵を初期化
	for (int i = 0; i < mEnemyNum; i++)
	{
		mEnemy[i]->reset();
		mEnemy[i]->setX(mEnemyX[i]);
		mEnemy[i]->setY(mEnemyY[i]);
		mEnemy[i]->initTileXY();
	}
	// 城を初期化
	mCastle.reset();
	// 残り時間を初期化
	mRemainingTime = 1500.0f;
	// ゲームオーバーのフラグを初期化
	mRoundOver = false;
	mClearedStage = false;

	// 乱数のシードを設定
	srand((unsigned int)time(NULL));
}

//==========================================================
// 人工知能
//==========================================================
void Stage::ai(float frameTime)
{
	// 各敵を更新
	for (int i = 0; i < mEnemyNum; i++)
		mEnemy[i]->ai(frameTime, mBrave);
}

//==========================================================
// ステージを1ステップ分進める
//==========================================================
void Stage::runStage(float frameTime)
{
	// それぞれの敵を更新
	for (int i = 0; i < mEnemyNum; i++) {
		mEnemy[i]->update(frameTime);
	}
	// 勇者を更新
	mBrave.update(frameTime);
	// 城を更新
	mCastle.update(frameTime);
	// それぞれのバリケードを更新
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		mBarricades[i].update(frameTime);
	}
	// 敵の数をチェックして、マップ上に存在しなければ新たに生成
	checkCurrentEnemyNum();
	// 残り時間が0ならゲームオーバー
	mRemainingTime -= frameTime;
	
	if (mRemainingTime < 0)
	{
		mRoundOver = true;
	}

	// 死亡チェック
	if ((mCastle.isDeath() || mBrave.getActive() == false) && !mRoundOver)
	{
		mRoundOver = true;
	}
}

//==========================================================
// 衝突を処理
// 処理する衝突は以下の通り：
// 勇者の攻撃と各敵との衝突
// 各敵の攻撃と勇者や城、バリケードとの衝突
// 勇者の必殺技（炎）と各敵の衝突
// 各敵と勇者の衝突
// 勇者の攻撃とバリケードとの衝突
// 各敵とほかのエンティティとの衝突状況によって、敵の状態を分岐
// ①攻撃中はそのまま攻撃
// ②勇者と衝突している場合は勇者に対しての攻撃状態に遷移
// ③城と衝突している場合は城に対しての攻撃状態に遷移
// ④最近接のバリケードに衝突している場合はバリケードに対しての攻撃状態に遷移
// ⑤いずれかのマップタイルの中央まで移動した場合、
//   最近接のバリケードに向かって移動状態に遷移（1/3くらいの確率で方向をランダムに決めるようにしている）
//==========================================================
void Stage::collisions()
{
	VECTOR2 collisionVector;	// 衝突判定したときの衝突相手へのベクトル
	bool braveAttackCollidesWithAnyEnemy = false;
	// 各敵について衝突判定
	for (int i = 0; i < mEnemyNum; i++) {
		// プレイヤーの攻撃コリジョンと雑魚敵の衝突の場合
		if (mBrave.getBraveAttackCollision().collidesWith(*mEnemy[i], collisionVector))
		{
			// 敵にダメージを与える
			mEnemy[i]->damage(BRAVE_ATTACK);
			// いずれかの敵に攻撃があたったのでループを抜けた後に攻撃コリジョンをなくす
			braveAttackCollidesWithAnyEnemy = true;
		}

		if (mEnemy[i]->getState() == characterNS::DEATH)
		{
			continue;
		}

		// 敵が攻撃時は、城とプレイヤーとバリケードとの当たり判定をそれぞれチェック
		if (mEnemy[i]->getState() == characterNS::ATTACK)
		{
			// 敵の攻撃コリジョンとプレイヤーの衝突の場合
			if (mEnemy[i]->getAttackCollision().collidesWith(mBrave, collisionVector))
			{
				// プレイヤーにダメージを与える
				if (mEnemy[i]->getEnemyType() == enemyNS::NORMAL) mBrave.setDamagePer(1.0f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::RED) mBrave.setDamagePer(1.2f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::BLUE) mBrave.setDamagePer(1.1f);
				if (typeid(*mEnemy[i]) == typeid(Enemy))
				{
					mBrave.damage(ENEMY_ATTACK);
				}
				else
				{
					mBrave.damage(MIDBOSS_ATTACK);
				}
				// 敵の攻撃コリジョンをなくす
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
			}
			// 敵の攻撃コリジョンと城の衝突の場合
			if (mEnemy[i]->getAttackCollision().collidesWith(mCastle, collisionVector))
			{
				// 城にダメージを与える
				if (mEnemy[i]->getEnemyType() == enemyNS::NORMAL) mCastle.setDamagePer(1.0f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::RED) mCastle.setDamagePer(1.2f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::BLUE) mCastle.setDamagePer(1.1f);
				if (typeid(*mEnemy[i]) == typeid(Enemy))
				{
					mCastle.damage(ENEMY_ATTACK);
				}
				else
				{
					mCastle.damage(MIDBOSS_ATTACK);
				}
				// 敵の攻撃コリジョンをなくす
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
			}
			// 敵の攻撃コリジョンとバリケードの衝突の場合
			if (mEnemy[i]->getAttackCollision().collidesWith(mBarricades[mEnemy[i]->getNearBarricadeIndex()], collisionVector)) {
				// バリケードにダメージを与える（barricades[enemy[i].getNearBarricadeIndex()]が攻撃対象になるはず）
				if (mEnemy[i]->getEnemyType() == enemyNS::NORMAL) mBarricades[mEnemy[i]->getNearBarricadeIndex()].setDamagerPer(1.0f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::RED) mBarricades[mEnemy[i]->getNearBarricadeIndex()].setDamagerPer(1.2f);
				else if (mEnemy[i]->getEnemyType() == enemyNS::BLUE) mBarricades[mEnemy[i]->getNearBarricadeIndex()].setDamagerPer(1.1f);
				if (typeid(*mEnemy[i]) == typeid(Enemy))
				{
					mBarricades[mEnemy[i]->getNearBarricadeIndex()].damage(ENEMY_ATTACK);
				}
				else
				{
					mBarricades[mEnemy[i]->getNearBarricadeIndex()].damage(MIDBOSS_ATTACK);
				}
				// 敵の攻撃コリジョンをなくす
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
				// 攻撃されたバリケードが破壊された場合、
				if (!mBarricades[mEnemy[i]->getNearBarricadeIndex()].getActive())
				{
					// マップデータを更新
					mMap.updateMapObjInt(mBarricades[mEnemy[i]->getNearBarricadeIndex()].getTileY(), mBarricades[mEnemy[i]->getNearBarricadeIndex()].getTileX(), mapNS::OBJ_NONE);
					if (mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 + 1) != mapNS::OBJ_BARRICADE
						&& mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 + 1) != mapNS::OBJ_NEAR_CASTLE)
					{
						mMap.updateMapObj(mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY(), mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() + 32, mapNS::OBJ_NONE);
					}
					if (mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 - 1) != mapNS::OBJ_BARRICADE
						&& mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 - 1) != mapNS::OBJ_NEAR_CASTLE)
					{
						mMap.updateMapObj(mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY(), mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() - 32, mapNS::OBJ_NONE);
					}
					if (mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 + 1, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32) != mapNS::OBJ_BARRICADE
						&& mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 + 1, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32) != mapNS::OBJ_NEAR_CASTLE)
					{
						mMap.updateMapObj(mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() + 32, mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX(), mapNS::OBJ_NONE);
					}
					if (mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 - 1, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32) != mapNS::OBJ_BARRICADE
						&& mMap.getMapObj((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 - 1, (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32) != mapNS::OBJ_NEAR_CASTLE)
					{
						mMap.updateMapObj(mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() - 32, mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX(), mapNS::OBJ_NONE);
					}
				}
			}
		}

		// 炎と雑魚敵の衝突の場合
		if (mBrave.getFire().collidesWith(*mEnemy[i], collisionVector))
		{
			mEnemy[i]->damage(FIRE);
			mBrave.getFire().setVisible(false);
			mBrave.getFire().setActive(false);
		}

		// 攻撃中ならば行動選択は行わない
		if (mEnemy[i]->getState() == characterNS::ATTACK)
		{
		}
		else if (mEnemy[i]->collidesWith(mBrave, collisionVector))// 雑魚敵とプレイヤーが衝突している場合、
		{
			// 勇者がいる方向に応じて攻撃する方向を変更
			mEnemy[i]->changeAttack(collisionVector);
		}
		else if (mMap.getMapObj(mEnemy[i]->getTileY(), mEnemy[i]->getTileX()) == mapNS::OBJ_NEAR_CASTLE)	// 雑魚敵と城が衝突していたら攻撃、
		{
			// 城がある方向に応じて攻撃する方向を変更
			if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() + 1) == mapNS::COL_CASTLE)
			{
				mEnemy[i]->changeAttack(characterNS::RIGHT);
			}
			else if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() - 1) == mapNS::COL_CASTLE)
			{
				mEnemy[i]->changeAttack(characterNS::LEFT);
			}
			else if (mMap.getMapCol(mEnemy[i]->getTileY() + 1, mEnemy[i]->getTileX()) == mapNS::COL_CASTLE)
			{
				mEnemy[i]->changeAttack(characterNS::DOWN);
			}
			else if (mMap.getMapCol(mEnemy[i]->getTileY() - 1, mEnemy[i]->getTileX()) == mapNS::COL_CASTLE)
			{
				mEnemy[i]->changeAttack(characterNS::UP);
			}
		}
		else if (mMap.getMapObj(mEnemy[i]->getTileY(), mEnemy[i]->getTileX()) == mapNS::OBJ_NEAR_BARRICADE)	// 最近接のバリケードに衝突していたら攻撃
		{
			// バリケードがある方向に応じて攻撃する方向を変更
			if (mEnemy[i]->getTileX() < (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32)
				mEnemy[i]->changeAttack(characterNS::RIGHT);
			else if (mEnemy[i]->getTileX() > (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32)
				mEnemy[i]->changeAttack(characterNS::LEFT);
			else if (mEnemy[i]->getTileY() < (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32)
				mEnemy[i]->changeAttack(characterNS::DOWN);
			else if (mEnemy[i]->getTileY() > (int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32)
				mEnemy[i]->changeAttack(characterNS::UP);
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error enemy and barricades"));
		}
		else if (mEnemy[i]->canMakeDecesionMoving())	// 敵が方向転換可能な状態にあるとき、意思決定を行い進む方向を決める
		{
			bool changeGoalDirectionFlag = false;	// 進みたい方向を変える必要があるかどうか
			if (rand() % 3 == 0)
			{
				mEnemy[i]->setState(characterNS::MOVE);
				mEnemy[i]->setGoalDirection((characterNS::DIRECTION) (rand() % 4));
				switch (mEnemy[i]->getGoalDirection())
				{
				case characterNS::LEFT:
					if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() - 1) >= mapNS::COL_ROCK)
					{
						changeGoalDirectionFlag = true;
					}
					break;
				case characterNS::RIGHT:
					if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() + 1) >= mapNS::COL_ROCK)
					{
						changeGoalDirectionFlag = true;
					}
					break;
				case characterNS::UP:
					if (mMap.getMapCol(mEnemy[i]->getTileY() - 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK)
					{
						changeGoalDirectionFlag = true;
					}
					break;
				case characterNS::DOWN:
					if (mMap.getMapCol(mEnemy[i]->getTileY() + 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK)
					{
						changeGoalDirectionFlag = true;
					}
					break;
				}
				// 進みたい方向に進めない場合、ランダムに方向を決めなおす
				if (changeGoalDirectionFlag)
				{
					// ランダムに進みたい方向を修正
					characterNS::DIRECTION newDirection = (characterNS::DIRECTION) (rand() % 4);
					// それでも進めない場合、敵を待機状態にして静止させる
					if (mEnemy[i]->canMoveTo(newDirection))
					{
						mEnemy[i]->setGoalDirection(newDirection);
					}
					else
					{
						mEnemy[i]->setState(characterNS::WAIT);
					}
				}
			}
			else
			{
				// 最近接のバリケードを探索
				mEnemy[i]->searchNearBarricadeIndex();
				if (!mEnemy[i]->checkBarricadeOnLine())	// 城までの直線上にバリケードがない場合、城へと進行する
				{
					// 敵の状態を城移動中へと変更
					mEnemy[i]->setState(characterNS::MOVE);
					// 進みたい方向に進めない場合、方向を進みたい方向をランダムに変換
					switch (mEnemy[i]->getGoalDirection())
					{
					case characterNS::LEFT:
						if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() - 1) >= mapNS::COL_ROCK)
						{
							changeGoalDirectionFlag = true;
						}
						break;
					case characterNS::RIGHT:
						if (mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() + 1) >= mapNS::COL_ROCK)
						{
							changeGoalDirectionFlag = true;
						}
						break;
					case characterNS::UP:
						if (mMap.getMapCol(mEnemy[i]->getTileY() - 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK)
						{
							changeGoalDirectionFlag = true;
						}
						break;
					case characterNS::DOWN:
						if (mMap.getMapCol(mEnemy[i]->getTileY() + 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK)
						{
							changeGoalDirectionFlag = true;
						}
						break;
					}
				}
				else // 城までの直線状にバリケードが存在する場合、
				{
					// 敵の状態をバリケードに進行中へと変更
					mEnemy[i]->setState(characterNS::MOVE);
					// 確率50%でx方向に進むかどうかを先に決定し、そのあとにy方向に進むかどうかを決定
					bool canMoveXDirection = false;
					bool canMoveYDirection = false;
					characterNS::DIRECTION xDirection;
					characterNS::DIRECTION yDirection;
					if ((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 < mEnemy[i]->getTileX())
					{
						if (!(mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() - 1) >= mapNS::COL_ROCK))
						{
							xDirection = characterNS::LEFT;
							mEnemy[i]->setGoalDirection(characterNS::LEFT);
							canMoveXDirection = true;
						}
						else
							changeGoalDirectionFlag = true;
					}
					else if ((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getX() / 32 > mEnemy[i]->getTileX())
					{
						if (!(mMap.getMapCol(mEnemy[i]->getTileY(), mEnemy[i]->getTileX() + 1) >= mapNS::COL_ROCK))
						{
							xDirection = characterNS::RIGHT;
							mEnemy[i]->setGoalDirection(characterNS::RIGHT);
							canMoveXDirection = true;
						}
						else
							changeGoalDirectionFlag = true;
					}
					if ((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 < mEnemy[i]->getTileY())
					{
						if (!(mMap.getMapCol(mEnemy[i]->getTileY() - 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK))
						{
							yDirection = characterNS::UP;
							mEnemy[i]->setGoalDirection(characterNS::UP);
							canMoveYDirection = true;
						}
						else
							changeGoalDirectionFlag = true;
					}
					else if ((int)mBarricades[mEnemy[i]->getNearBarricadeIndex()].getY() / 32 > mEnemy[i]->getTileY())
					{
						if (!(mMap.getMapCol(mEnemy[i]->getTileY() + 1, mEnemy[i]->getTileX()) >= mapNS::COL_ROCK))
						{
							yDirection = characterNS::DOWN;
							mEnemy[i]->setGoalDirection(characterNS::DOWN);
							canMoveYDirection = true;
						}
						else
							changeGoalDirectionFlag = true;
					}
					if (canMoveXDirection && canMoveYDirection)
					{
						if (rand() % 2 == 0)
							mEnemy[i]->setGoalDirection(xDirection);
						else
							mEnemy[i]->setGoalDirection(yDirection);
					}
				}
				// 進みたい方向に進めない場合、ランダムに方向を決めなおす
				if (changeGoalDirectionFlag)
				{
					// ランダムに進みたい方向を修正
					characterNS::DIRECTION newDirection = (characterNS::DIRECTION) (rand() % 4);
					// それでも進めない場合、敵を待機状態にして静止させる
					if (mEnemy[i]->canMoveTo(newDirection))
					{
						mEnemy[i]->setGoalDirection(newDirection);
					}
					else
					{
						mEnemy[i]->setState(characterNS::WAIT);
					}
				}
			}
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		// プレイヤーの攻撃コリジョンとの衝突の場合
		if (mBrave.getBraveAttackCollision().collidesWith(mBarricades[i], collisionVector))
		{
			// バリケードにダメージを与える
			mBarricades[i].damage(BRAVE_ATTACK);
			// いずれかのバリケードに攻撃があたったのでループを抜けた後に攻撃コリジョンをなくす
			braveAttackCollidesWithAnyEnemy = true;
		}
	}

	// 勇者の攻撃がいずれかの敵に当たった場合、攻撃コリジョンをなくす
	if (braveAttackCollidesWithAnyEnemy)
	{
		mBrave.getBraveAttackCollision().setVisible(false);
		mBrave.getBraveAttackCollision().setActive(false);
	}
}

//==========================================================
// 敵の数をチェックし、マップ上に敵がいなければ新たに生成
//==========================================================
void Stage::checkCurrentEnemyNum()
{
	// 雑魚敵が存在したら
	for (int i = 0; i < mEnemyNum; i++)
	{
		if (mEnemy[i]->getActive())
			return;
	}

	// 第2波
	for (int i = 0; i < mEnemyNum; i++)
	{
		safeDelete(mEnemy[i]);
	}
	safeDeleteArray(mEnemy);
	safeDeleteArray(mEnemyX);
	safeDeleteArray(mEnemyY);
	mMap.resetMapCol();
	mEnemyWaveNum += 1;
	initializeEnemies(mStageNum + 1, mEnemyWaveNum + 1);
}

//==========================================================
// 指定されたステージの敵データを読み込む
//==========================================================
bool Stage::readEnemyFile(int stageNum, int enemyWave)
{
	string enemyDataFilename = "stageData\\stage" + std::to_string(stageNum) + "\\enemy" + std::to_string(enemyWave) + "\\enemydata.csv";

	mEnemyNum = 0;
	ifstream* ifs = new ifstream(enemyDataFilename);
	if (!*ifs)
	{
		mClearedStage = true;
		return false;
	}
	//csvファイルを1行ずつ読み込む
	string str;
	if (getline(*ifs, str))
	{
		string token;
		istringstream stream(str);
		if (getline(stream, token, ','))
		{
			mEnemyNum = (int)stof(token);
			mEnemy = new Enemy*[mEnemyNum];
			mEnemyX = new float[mEnemyNum];
			mEnemyY = new float[mEnemyNum];
		}
	}
	if (getline(*ifs, str)) 
	{
		string token;
		istringstream stream(str);
		for (int i = 0; i < mEnemyNum; ++i)
		{
			if (getline(stream, token, ','))
			{
				if ((int)stof(token) <= 3)
				{
					mEnemy[i] = new Enemy();
					switch ((int)stof(token))
					{
					case 1:
						mEnemy[i]->setEnemyType(enemyNS::NORMAL);
						break;
					case 2:
						mEnemy[i]->setEnemyType(enemyNS::RED);
						break;
					case 3:
						mEnemy[i]->setEnemyType(enemyNS::BLUE);
					}
				}
				else if ((int)stof(token) == 4)
					mEnemy[i] = new MidBoss();
			}
		}
	}
	if (getline(*ifs, str))
	{
		string token;
		istringstream stream(str);
		for (int i = 0; i < mEnemyNum; ++i)
		{
			if (getline(stream, token, ','))
			{
				mEnemyX[i] = stof(token);
			}
		}
	}
	if (getline(*ifs, str))
	{
		string token;
		istringstream stream(str);
		for (int i = 0; i < mEnemyNum; ++i)
		{
			if (getline(stream, token, ','))
			{
				mEnemyY[i] = stof(token);
			}
		}
	}

	safeDelete(ifs);
	return true;
}

//==========================================================
// 指定されたステージ、派の敵データを読み込み敵を初期化する
//==========================================================
void Stage::initializeEnemies(int stageNum, int enemyWave)
{
	if (!readEnemyFile(stageNum, enemyWave))
	{
		return;
	}
	for (int i = 0; i < mEnemyNum; i++) {
		mEnemy[i]->setScale(1.5);
		mEnemy[i]->setMapPointer(mMap);
		mEnemy[i]->setBarricadesPointer(mBarricades);
		mEnemy[i]->reset();
		mEnemy[i]->setX(mEnemyX[i]);
		mEnemy[i]->setY(mEnemyY[i]);
		mEnemy[i]->initTileXY();
	}
	mInitializedEnemies = true;
}