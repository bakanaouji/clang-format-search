//==========================================================
/// @file
/// @brief    castleDiffender.hの実装
/// @author   阿部拳之
///
/// @attention  ゲームのメインクラスです。
///				1フレームごとの更新や描画はこのクラスで行います。

//==========================================================

#include "castleDiffender.h"

//==========================================================
// コンストラクタ
//==========================================================
CastleDiffender::CastleDiffender()
{
	mInitialized = false;	// ゲームが初期化された場合、trueになる
	mFontCK = new Text();	// スプライトフォント
	mMenuOn = true;			// メニュー表示フラグ
	mStageSelectOn = false;	// ステージ選択画面表示フラグ
	mDescriptionOn = false;	// 操作説明画面表示フラグ
	mRect = NULL;			// システムグラフィックス用の四角形
}

//==========================================================
// デストラクタ
//==========================================================
CastleDiffender::~CastleDiffender()
{
	safeDelete(mRect);		// システムグラフィックス用の四角形のメモリを解放
	safeDelete(mFontCK);		// スプライトフォントのメモリを解放
	releaseAll();			// すべてのグラフィックスアイテムについて、onLostDevice()を呼び出す
}

//==========================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//==========================================================
void CastleDiffender::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	// GameErrorをスロー

	// システムグラフィックス用の四角形を生成、初期化
	mRect = new Rect();
	mRect->initialize(mGraphics);

	// スプライトテキストを初期化
	if (!mFontCK->initialize(mGraphics, FONT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));


	// メニューのテクスチャ
	if (!mMenuTexture.initialize(mGraphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	// メニューの画像
	if (!mMenu.initialize(mGraphics, 0, 0, 0, &mMenuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// ステージ選択画面のテクスチャ
	if (!mStageSelectTexture.initialize(mGraphics, STAGE_SELECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage select texture"));
	// ステージ選択画面の画像
	if (!mStageSelect.initialize(mGraphics, 0, 0, 0, &mStageSelectTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage select"));

	// リザルトのテクスチャ
	if (!mResultTexture.initialize(mGraphics, RESULT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing result texture"));
	// リザルトの画像
	if (!mResult.initialize(mGraphics, 0, 0, 0, &mResultTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing result"));
	mResult.setX(GAME_WIDTH / 2.0f - mResult.getWidth() / 2.0f);
	mResult.setY((float)-mResult.getHeight());

	// ステージクリア画面のテクスチャ
	if (!mStageClearTexture.initialize(mGraphics, STAGE_CLEAR_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage clear texture"));
	// ステージクリア画像
	if (!mStageClear.initialize(mGraphics, 0, 0, 0, &mStageClearTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage clear"));
	mStageClear.setX(GAME_WIDTH / 2.0f - mStageClear.getWidth() / 2.0f);
	mStageClear.setY((float)GAME_HEIGHT);

	// 操作説明のテクスチャ
	if (!mDescriptionTexture.initialize(mGraphics, DESCRIPTION_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing description texture"));
	// 操作説明の画像
	if (!mDescription.initialize(mGraphics, 0, 0, 0, &mDescriptionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing description"));

	// マップのテクスチャ
	if (!mTileTexture.initialize(mGraphics, TILE_IMAGES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map texture"));
	// マップの画像
	if (!mStage.getMap().initialize(this, mapNS::TEXTURE_SIZE, mapNS::TEXTURE_SIZE, mapNS::TEXTURE_COLS, &mTileTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));

	// バリケードのテクスチャ
	if (!mBarricadeTexture.initialize(mGraphics, BARRICADE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing barricade texture"));
	if (!mHitEffectTexture.initialize(mGraphics, HIT_EFFECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hit effect texture"));
	// バリケードの画像
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		if (!mStage.getBarricade(i).initialize(this, barricadeNS::WIDTH, barricadeNS::HEIGHT, barricadeNS::TEXTURE_COLS, &mBarricadeTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing barricade"));
		mStage.getBarricade(i).setScale(1);
		if (!mStage.getBarricade(i).getHitEffect().initialize(mGraphics, hitEffectNS::WIDTH, hitEffectNS::HEIGHT, hitEffectNS::TEXTURE_COLS, &mHitEffectTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hitEffect"));
	}

	// 城のテクスチャ
	if (!mCastleTexture.initialize(mGraphics, CASTLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle texture"));
	// 城の画像
	if (!mStage.getCastle().initialize(this, castleNS::WIDTH, castleNS::HEIGHT, castleNS::TEXTURE_COLS, &mCastleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle"));

	// 城のアイコンのテクスチャ
	if (!mCastleIconTexture.initialize(mGraphics, CASTLE_ICON_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle icon texture"));
	// 城のアイコンの画像
	if (!mCastleIcon.initialize(mGraphics, castleIconNS::WIDTH, castleIconNS::HEIGHT, castleIconNS::TEXTURE_COLS, &mCastleIconTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle icon"));

	// 勇者のテクスチャ
	if (!mBraveTexture.initialize(mGraphics, BRAVE_MOVE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move texture"));
	// 勇者
	if (!mStage.getBrave().initialize(this, braveNS::WIDTH, braveNS::HEIGHT, braveNS::TEXTURE_COLS, &mBraveTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move"));
	mStage.getBrave().setScale(1.5);
	mStage.getBrave().setFrames(braveNS::MOVE_UP_START_FRAME, braveNS::MOVE_UP_END_FRAME);
	mStage.getBrave().setCurrentFrame(braveNS::MOVE_UP_START_FRAME);
	mStage.getBrave().setMapPointer(mStage.getMap());

	// 勇者のアイコンのテクスチャ
	if (!mBraveIconTexture.initialize(mGraphics, BRAVE_ICON_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave icon texture"));
	// 勇者のアイコンの画像
	if (!mBraveIcon.initialize(mGraphics, braveIconNS::WIDTH, braveIconNS::HEIGHT, braveIconNS::TEXTURE_COLS, &mBraveIconTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave icon"));
	mBraveIcon.linkEntity(mStage.getBrave());

	// 勇者の当たり判定用のテクスチャ
	if (!mAttackCollisionTexture.initialize(mGraphics, COLLISION_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack collision texture"));
	// 勇者のあたり判定用
	if (!mStage.getBrave().getBraveAttackCollision().initialize(this, braveAttackCollisionNS::WIDTH, braveAttackCollisionNS::HEIGHT, 0, &mAttackCollisionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack collision"));
	if (!mAttackEffectTexture.initialize(mGraphics, ATTACK_EFFECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack effect texture"));
	if (!mStage.getBrave().getBraveAttackCollision().getAttackEffect().initialize(mGraphics, attackEffectNS::WIDTH, attackEffectNS::HEIGHT, attackEffectNS::TEXTURE_COLS, &mAttackEffectTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack effect"));

	// 炎のテクスチャ
	if (!mFireTexture.initialize(mGraphics, FIRE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fire texture"));
	if (!mStage.getBrave().getFire().initialize(this, fireNS::WIDTH, fireNS::HEIGHT, fireNS::TEXTURE_COLS, &mFireTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fire"));
	mStage.getBrave().getFire().setScale(1.5);

	// 雑魚敵のテクスチャ
	if (!mEnemyTexture.initialize(mGraphics, ENEMY_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy texture"));
	// 中ボスのテクスチャ
	if (!mMidBossTexture.initialize(mGraphics, MID_BOSS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing midBoss texture"));

	// ダッシュボード
	if (!mDashboardTextures.initialize(mGraphics, DASHBOARD_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dashboard textures"));
	mBraveHealthBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::BRAVE_HEALTH_BAR_X, castleDiffenderNS::BRAVE_HEALTH_BAR_Y, 0.5f, 100, graphicsNS::RED);
	mBraveHealthBar.set(mStage.getBrave().getHealth());
	mBraveMpBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::BRAVE_MP_BAR_X, castleDiffenderNS::BRAVE_MP_BAR_Y, 0.5f, 100, graphicsNS::GREEN);
	mBraveMpBar.set((float)mStage.getBrave().getMP());
	mCastleHealthBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::CASTLE_HEALTH_BAR_X + 40, castleDiffenderNS::CASTLE_HEALTH_BAR_Y, 0.5f, 100, graphicsNS::BLUE);
	mCastleHealthBar.set(mStage.getCastle().getHealth());

	// テキスト画像
	if (!mTextTexture.initialize(mGraphics, TEXT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing text textures"));
	if (!mBraveHpText.initialize(mGraphics, hpTextImageNS::WIDTH, hpTextImageNS::HEIGHT, hpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	if (!mBraveMpText.initialize(mGraphics, mpTextImageNS::WIDTH, mpTextImageNS::HEIGHT, mpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	if (!mCastleHpText.initialize(mGraphics, hpTextImageNS::WIDTH, hpTextImageNS::HEIGHT, hpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	mCastleHpText.setX(830);

	// タイトルBGM再生
	mAudio->playCue("title");

	// 各ステージのハイスコア読み取り
	loadHighScore();
	return;
}

//==========================================================
// プレイの新しいラウンドを開始
//==========================================================
void CastleDiffender::roundStart()
{
	// 現在のステージ番号でゲーム開始
	mStage.roundStart();
	// リザルト画面の位置をリセット
	mResult.setY((float)-mResult.getHeight());
	// ステージクリア画面の位置をリセット
	mStageClear.setY((float)GAME_HEIGHT);
	// 勇者のアイコンの初期化
	mBraveIcon.reset();
	// ラウンドが開始するまでの時間をリセット
	mRoundTimer = castleDiffenderNS::ROUND_TIME;
	// ステージ中BGM再生
	mAudio->playCue("stage");
}

//==========================================================
// すべてのゲームアイテムを更新
//==========================================================
void CastleDiffender::update()
{
	if (mMenuOn)		// メニュー画面表示中
	{
		if (mInput->isKeyDown('Z')) // Zキーでステージ選択画面に以降
		{
			// メニュー画面オフ
			mMenuOn = false;
			// ステージ選択画面に遷移
			mStageSelectOn = true;
			// 入力をクリア
			mInput->clearAll();
			// ステージ情報をリセット
			mStage.reset();
			// システムグラフィックス用の四角形をリセット
			mRect->setX(380);
			mRect->setY(80);
			mRect->setWidth(510);
			mRect->setHeight(81);
			mRect->setBackColor(SETCOLOR_ARGB(50, 120, 120, 255));
			mRect->reset();
		}
		else if (mInput->isKeyDown('X')) // Xキーで操作説明表示
		{
			// メニュー画面オフ
			mMenuOn = false;
			// 操作説明画面に遷移
			mDescriptionOn = true;
			// 入力をクリア
			mInput->clearAll();
		}
		else if (mInput->isKeyDown('E'))	// Eキーでゲーム終了
		{
			// ゲーム終了
			exitGame();
		}
	}
	else if (mStageSelectOn)	// ステージ選択画面表示中
	{
		if (mInput->isKeyDown('Z'))	// Zキーでステージ決定
		{
			// ステージ選択画面オフ
			mStageSelectOn = false;
			// タイトルBGMをオフ
			mAudio->stopCue("title");
			// ゲームスタート
			roundStart();
			// システムグラフィックス用の四角形をリセット
			mRect->setX((float)rectNS::X);
			mRect->setY((float)rectNS::Y);
			mRect->setWidth(rectNS::WIDTH);
			mRect->setHeight(rectNS::HEIGHT);
			mRect->setBackColor(rectNS::BACK_COLOR);
			mRect->reset();
		}
		else if (mInput->isKeyDown(BRAVE_DOWN_KEY))	// 下キーで難しいステージを選択
		{
			// ステージ番号が最大ステージ数より少なければ、
			if (mStage.getStageNum() < 3 - 1)
			{
				// ステージ数をインクリメント
				mStage.setStageNum(mStage.getStageNum() + 1);
				// システムグラフィックス用の四角形をリセット
				mRect->setY(mRect->getY() + 240);
				mRect->reset();
			}
			// 入力をクリア
			mInput->clearAll();
		}
		else if (mInput->isKeyDown(BRAVE_UP_KEY))	// 上キーで簡単なステージを選択
		{
			// ステージ番号が最小ステージ数より多ければ、
			if (mStage.getStageNum() > 0)
			{
				// ステージ数をデクリメント
				mStage.setStageNum(mStage.getStageNum() - 1);
				// システムグラフィックス用の四角形をリセット
				mRect->setY(mRect->getY() - 240);
				mRect->reset();
			}
			// 入力をクリア
			mInput->clearAll();
		}
	}
	else if (mDescriptionOn) // 操作説明中
	{
		// Xキーでタイトルに戻る
		if (mInput->isKeyDown('X'))
		{
			// 操作説明画面オフ
			mDescriptionOn = false;
			// メニュー画面オン
			mMenuOn = true;
			// 入力をクリア
			mInput->clearAll();
		}
	}
	else if (mStage.getRoundOver())	// ゲームオーバー中
	{
		// リザルト画面が一定以上降下したら
		if (mResult.getY() > 50)
		{
			// Xが押されたらメニュー画面に戻る
			if (mInput->isKeyDown('X'))
			{
				// ゲームオーバーフラグをリセット
				mStage.setRoundOver(false);
				// メニュー画面オン
				mMenuOn = true;
				// 入力をクリア
				mInput->clearAll();
				// ゲームオーバーBGMをオフ
				mAudio->stopCue("gameover");
				// タイトルBGMをオン
				mAudio->playCue("title");
			}
			// Eが押されたらゲーム終了
			else if (mInput->isKeyDown('E'))
			{
				// ゲーム終了
				exitGame();
			}
			// Zが押されたらステージをリトライ
			else if (mInput->isKeyDown('Z'))
			{
				// 入力をクリア
				mInput->clearAll();
				// ゲームオーバーBGMをオフ
				mAudio->stopCue("gameover");
				// ステージスタート
				roundStart();
			}
		}
		else // リザルト画面が一定以上降下していなければ
		{
			// リザルト画面を降下
			mResult.setY(mResult.getY() + mFrameTime * 120.0f);
		}
	}
	else if (mStage.getClearedStage())	// ステージをクリア中なら
	{
		// ステージクリア画面が一定以上上昇したら
		if (mStageClear.getY() < 250)
		{
			// Xが押されたらメニュー画面に戻る
			if (mInput->isKeyDown('X'))
			{
				// メニュー画面オン
				mMenuOn = true;
				// 入力をクリア
				mInput->clearAll();
				// クリアBGMをオフ
				mAudio->stopCue("clear");
				// タイトルBGMをオン
				mAudio->playCue("title");
				// ステージクリアフラグをリセット
				mStage.setClearedStage(false);
			}
			// Eが押されたらゲーム終了
			else if (mInput->isKeyDown('E'))
			{
				// ゲーム終了
				exitGame();
			}
			// Zが押されたらステージをリトライ
			else if (mInput->isKeyDown('Z'))
			{
				// 入力をクリア
				mInput->clearAll();
				// クリアBGMをオフ
				mAudio->stopCue("clear");
				// ステージスタート
				roundStart();
			}
		}
		else // ステージクリア画面が一定以上上昇していなければ
		{
			// ステージクリア画面を上昇
			mStageClear.setY(mStageClear.getY() - mFrameTime * 320.0f);
		}
	}
	else // ゲーム中の場合、
	{
		// ラウンド開始までの時間が過ぎていなければステージ情報は更新しない
		if (mRoundTimer > 0.0f)
		{
			mRoundTimer -= mFrameTime;
			return;
		}
		// ステージ情報を更新
		mStage.runStage(mFrameTime);
		// 敵を初期化した直後なら
		if (mStage.getInitializedEnemies())
		{
			// 敵のテクスチャを初期化
			initializeEnemiesTexture();
		}
		// ゲームオーバーなら
		if (mStage.getRoundOver())
		{
			// ゲームオーバー時の処理を実行
			gameOver();
		}
		// ステージをクリアしたら
		if (mStage.getClearedStage())
		{
			// ステージクリア時の処理を実行
			clearStage();
		}

		// 勇者のアイコン画像を更新
		mBraveIcon.update(mFrameTime);
	}
}

//==========================================================
// 人工知能
//==========================================================
void CastleDiffender::ai()
{
	// ステージ内のゲームアイテムの人工知能
	mStage.ai(mFrameTime);
}

//==========================================================
// 衝突を処理
//==========================================================
void CastleDiffender::collisions()
{
	// ステージ内のゲームアイテムの衝突を処理
	mStage.collisions();
}

//==========================================================
// ゲームアイテムをレンダー
// プレイ中のレイヤー：マップ→黒背景→その他アイテムの順
//==========================================================
void CastleDiffender::render()
{
	// スプライトの描画を開始
	mGraphics->spriteBegin();	
	if (mMenuOn)	// メニュー画面オンの場合、
	{
		// メニュー画面描画
		drawMenu();
	}
	else if (mStageSelectOn)	// ステージ選択画面オンの場合、
	{
		// ステージ選択画面描画
		drawStageSelect();
	}
	else if (mDescriptionOn)	// 操作説明画面オンの場合、
	{
		// 操作説明画面描画
		drawDescription();
	}
	else // ステージ中の場合、
	{
		// ステージ画面を描画
		// ステージプレイ中は通常ここで描画される
		drawOnStage();
	}

	// ゲームオーバー画面とステージクリア画面はゲーム画面の一部に表示されるため、ゲーム画面と重ねて表示
	if (mStage.getRoundOver())	// ゲームオーバーの場合、
	{
		// ゲームオーバー画面を描画
		drawOnRoundOver();
	}
	else if (mStage.getClearedStage())	// ステージクリアの場合、
	{
		// ステージクリア画面を描画
		drawOnClearedStage();
	}
	// スプライトの描画を開始
	mGraphics->spriteEnd();
}

//==========================================================
// メニュー画面時の描画
//==========================================================
void CastleDiffender::drawMenu()
{
	// メニュー画面を描画
	mMenu.draw();
	// システムグラフィックスを描画
	char str[128] = "PUSH Z KEY : STAGE SELECT";
	char str2[128] = "PUSH X KEY : OPERATION DESCRIPTION";
	char str3[128] = "PUSH E KEY : EXIT";
	mFontCK->setFontHeight(35);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
	mFontCK->print(str, 273, 353);
	mFontCK->print(str2, 273, 453);
	mFontCK->print(str3, 273, 553);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));
	mFontCK->print(str, 273, 350);
	mFontCK->print(str2, 273, 450);
	mFontCK->print(str3, 273, 550);
}

//==========================================================
// ステージ選択画面時の描画
//==========================================================
void CastleDiffender::drawStageSelect()
{
	// ステージ選択画面を描画
	mStageSelect.draw();
	// スプライトの描画を終了
	mGraphics->spriteEnd();
	// システムグラフィックス用の四角形を描画
	mRect->draw();
	// スプライトの描画を開始
	mGraphics->spriteBegin();
	// システムグラフィックスを描画
	char str[128] = "PUSH Z KEY : START STAGE!";
	mFontCK->setFontHeight(35);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
	mFontCK->print(str, 353, 653);
	mFontCK->setFontHeight(35);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));
	mFontCK->print(str, 350, 650);
	for (int i = 0; i < 3; ++i)
	{
		char str2[128] = "HIGH SCORE IS ";
		char time[5] = { 0 };
		sprintf_s(time, "%04d", (int)mHighScores[i]);
		strcat_s(str2, time);
		mFontCK->setFontHeight(20);
		mFontCK->setFontColor(graphicsNS::ORANGE);
		mFontCK->print(str2, 505, 3 + 50 + i * 240);
	}
}

//==========================================================
// 操作説明画面時の描画
//==========================================================
void CastleDiffender::drawDescription()
{
	// 操作説明画面をオン
	mDescription.draw();
}

//==========================================================
// ステージ中の描画
//==========================================================
void CastleDiffender::drawOnStage()
{
	// マップタイルを描画
	for (int row = 0; row < mapNS::MAP_HEIGHT; row++)       // マップの各行を処理
	{
		mStage.getMap().setY((float)(row*mapNS::TEXTURE_SIZE));     // タイルのYを設定
		for (int col = 0; col < mapNS::MAP_WIDTH; col++)			// マップの各列を処理
		{
			if (mStage.getMap().getMapData(row, col) >= 0)          // タイルが存在する場合（この処理はいらないと思われるが念のため）
			{
				mStage.getMap().setCurrentFrame(mStage.getMap().getMapData(row, col));                      // タイルのテクスチャを設定
				mStage.getMap().setX((float)(col*mapNS::TEXTURE_SIZE));										// タイルのXを設定
				if (mStage.getMap().getX() > -mapNS::TEXTURE_SIZE && mStage.getMap().getX() < GAME_WIDTH)   // タイルが画面上にあるかどうか
					mStage.getMap().draw();    // タイルを描画
			}
		}
	}
	// バリケードを描画
	for (int i = 0; i < 8; ++i)
	{
		mStage.getBarricade(i).draw();				// オブジェクトを描画
		mStage.getBarricade(i).getHitEffect().draw();// バリケードに攻撃がヒットしたときのアニメーションを描画
	}

	// スプライトの描画を終了
	mGraphics->spriteEnd();
	// システムグラフィックス用の四角形を描画
	mRect->draw();
	// スプライトの描画を開始
	mGraphics->spriteBegin();

	// 敵を描画
	for (int i = 0; i < mStage.getEnemyNum(); i++) {
		// 画像に偏りがあるため、位置を修正
		float tmpX = mStage.getEnemy(i).getX();
		float tmpY = mStage.getEnemy(i).getY();
		mStage.getEnemy(i).setX((float)(tmpX - mStage.getEnemy(i).getWidth() * (mStage.getEnemy(i).getScale() - 1) / 2.0));
		mStage.getEnemy(i).setY((float)(tmpY - mStage.getEnemy(i).getHeight() * (mStage.getEnemy(i).getScale() - 1) / 2.0 - 10.0));
		// 敵の種類に応じてカラーフィルターを設定
		// 敵を描画
		switch (mStage.getEnemy(i).getEnemyType())
		{
		case enemyNS::RED:
			mStage.getEnemy(i).draw(graphicsNS::RED);
			break;
		case enemyNS::BLUE:
			mStage.getEnemy(i).draw(graphicsNS::BLUE);
			break;
		default:
			mStage.getEnemy(i).draw(graphicsNS::WHITE);
			break;
		}
		// 敵の攻撃判定用のエンティティを描画（デバッグ用に色を付けて表示できるようにしている）
		mStage.getEnemy(i).getAttackCollision().draw();
		// 位置をもとに戻す
		mStage.getEnemy(i).setX(tmpX);
		mStage.getEnemy(i).setY(tmpY);
	}
	// プレイヤーのアイコンを描画
	mBraveIcon.draw();
	// 城のアイコンを描画
	mCastleIcon.draw();
	// 城を描画
	mStage.getCastle().draw();
	// プレイヤーの炎を描画
	mStage.getBrave().getFire().draw();
	// プレイヤーを描画
	// 画像に偏りがあるため、位置を修正
	float tmpX = mStage.getBrave().getX();
	float tmpY = mStage.getBrave().getY();
	mStage.getBrave().setX((float)(tmpX - mStage.getBrave().getWidth() * (mStage.getBrave().getScale() - 1) / 2.0));
	mStage.getBrave().setY((float)(tmpY - mStage.getBrave().getHeight() * (mStage.getBrave().getScale() - 1) / 2.0 - 10));
	mStage.getBrave().draw(graphicsNS::WHITE);
	mStage.getBrave().setX(tmpX);
	mStage.getBrave().setY(tmpY);
	// プレイヤーの攻撃判定用のエンティティを描画（デバッグ用に色を付けて表示できるようにしている）
	mStage.getBrave().getBraveAttackCollision().draw();
	// プレイヤーの攻撃の衝撃派を描画
	mStage.getBrave().getBraveAttackCollision().getAttackEffect().draw();
	// プレイヤーのHPを棒グラフで描画
	mBraveHealthBar.set(mStage.getBrave().getHealth());
	mBraveHealthBar.draw(graphicsNS::FILTER);
	// プレイヤーのMPを棒グラフで描画
	mBraveMpBar.set((float)mStage.getBrave().getMP());
	mBraveMpBar.draw(graphicsNS::FILTER);
	// 城のHPを棒グラフで描画
	mCastleHealthBar.set(mStage.getCastle().getHealth());
	mCastleHealthBar.draw(graphicsNS::FILTER);

	// システムグラフィックスを描画
	// 勇者用のHPテキストを描画
	mBraveHpText.draw();
	// 勇者用のMPテキストを描画
	mBraveMpText.draw();
	// 城用のHPテキストを描画
	mCastleHpText.draw();
	// システムテキストを描画
	char str[128] = "TIME-";
	char time[5] = { 0 };
	sprintf_s(time, "%04d", (int)mStage.getRemainingTime());
	strcat_s(str, time);
	mFontCK->setFontHeight(40);
	mFontCK->setFontColor(SETCOLOR_ARGB(128, 128, 128, 128));  // shadow grey
	mFontCK->print(str, 512, 10);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));
	mFontCK->print(str, 505, 7);
	mFontCK->setFontHeight(14);
	if (mRoundTimer > 0.0f)
	{
		// ラウンドが開始するまでは、「STAGE START!」と一定時間表示させる
		string str1 = "STAGE" + to_string(mStage.getStageNum() + 1);
		char str2[128] = "START!";
		float fontSize = 60.0f;
		float fastSpeed = 900.0f;
		float secondSpeed = 50.0f;
		float initX = -fontSize * 6.0f;
		mFontCK->setFontHeight((UINT)fontSize);
		float T = (GAME_WIDTH - initX * 2.0f - fontSize * 6) / (2.0f * fastSpeed);
		if (mRoundTimer > castleDiffenderNS::ROUND_TIME - T)
		{
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 303);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 403);
			mFontCK->setFontColor(graphicsNS::BLUE);
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 300);
			mFontCK->setFontColor(graphicsNS::RED);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 400);
		}
		else if (mRoundTimer > castleDiffenderNS::ROUND_TIME - T * 2.0f)
		{
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 303);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 403);
			mFontCK->setFontColor(graphicsNS::BLUE);
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 300);
			mFontCK->setFontColor(graphicsNS::RED);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 400);
		}
		else
		{
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T * 2.0f) * fastSpeed + (fastSpeed + secondSpeed) * T), 303);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T * 2.0f) * fastSpeed + (fastSpeed + secondSpeed) * T), 403);
			mFontCK->setFontColor(graphicsNS::BLUE);
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T * 2.0f) * fastSpeed + (fastSpeed + secondSpeed) * T), 300);
			mFontCK->setFontColor(graphicsNS::RED);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T * 2.0f) * fastSpeed + (fastSpeed + secondSpeed) * T), 400);
		}
	}
}

//==========================================================
// ゲームオーバー時の描画
//==========================================================
void CastleDiffender::drawOnRoundOver()
{
	// ゲームオーバー画面を描画
	mResult.draw();
	// リザルト画面が一定以上降下したら
	if (mResult.getY() > 50)
	{
		// システムグラフィックスのテキストを表示
		char str[128] = "CONTINUE?";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 35 * 9 / 2), (int)(mResult.getHeight() + mResult.getY()));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 35 * 9 / 2), (int)(mResult.getHeight() + mResult.getY() - 3));
		char str2[128] = "PUSH Z KEY : RETRY STAGE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55));
		char str3[128] = "PUSH X KEY : RETURN TITLE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55 * 2));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55 * 2));
		char str4[128] = "PUSH E KEY : EXIT";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55 * 3));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55 * 3));
	}
}

//==========================================================
// ステージクリア時の描画
//==========================================================
void CastleDiffender::drawOnClearedStage()
{
	// ステージクリア画面を描画
	mStageClear.draw();
	// ステージクリア画面が一定以上上昇したら
	if (mStageClear.getY() < 250)
	{
		// システムグラフィックスのテキストを表示
		char str[128] = "YOUR SCORE IS ";
		char time[5] = { 0 };
		sprintf_s(time, "%04d", (int)mStage.getRemainingTime());
		strcat_s(str, time);
		mFontCK->setFontHeight(45);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 45 * 16 / 2), (int)(mStageClear.getHeight() + mStageClear.getY()));
		mFontCK->setFontHeight(45);
		mFontCK->setFontColor(graphicsNS::RED);
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 45 * 16 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3));
		char str2[128] = "PUSH Z KEY : RETRY STAGE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55));
		char str3[128] = "PUSH X KEY : RETURN TITLE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55 * 2));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55 * 2));
		char str4[128] = "PUSH E KEY : EXIT";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // 影
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55 * 3));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55 * 3));
	}
}

//==========================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//==========================================================
void CastleDiffender::releaseAll()
{
	SAFE_ON_LOST_DEVICE(mFontCK);
	mMenuTexture.onLostDevice();
	mStageSelectTexture.onLostDevice();
	mBraveTexture.onLostDevice();
	mDashboardTextures.onLostDevice();
	safeOnLostDevice(mRect);
	Game::releaseAll();
	return;
}

//==========================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
//==========================================================
void CastleDiffender::resetAll()
{
	SAFE_ON_RESET_DEVICE(mFontCK);
	mDashboardTextures.onLostDevice();
	mBraveTexture.onResetDevice();
	mMenuTexture.onResetDevice();
	mStageSelectTexture.onResetDevice();
	safeOnResetDevice(mRect);
	Game::resetAll();
	return;
}


//=============================================================================
// コンソールコマンドを処理
// デバッグ用
//=============================================================================
void CastleDiffender::consoleCommand()
{
	mCommand = mConsole->getCommand();    // コンソールからのコマンドを取得
	if (mCommand == "")                  // コマンドがない場合
		return;

	if (mCommand == "help")              // 「help」コマンドの場合
	{
		mConsole->print("Console Commands:");
		mConsole->print("fps - toggle display of frames per second");
		mConsole->print("mapobj - display map object data");
		mConsole->print("mapcol - display map collision data");
		mConsole->print("mapdata - display map data");
		return;
	}
	if (mCommand == "fps")
	{
		mFpsOn = !mFpsOn;                 // フレームレートの表示を切り替える
		if (mFpsOn)
			mConsole->print("fps On");
		else
			mConsole->print("fps Off");
	}
	if (mCommand == "mapobj")
	{
		for (int i = 0; i < mapNS::MAP_HEIGHT; ++i)
		{
			string str = "";
			for (int j = 0; j < mapNS::MAP_WIDTH; ++j)
			{
				if (mStage.getMap().getMapObj(i, j) >= mapNS::OBJ_BARRICADE)
				{
					str += " " + to_string(mStage.getMap().getMapObj(i, j)) + ",";
				}
				else
				{
					str += to_string(mStage.getMap().getMapObj(i, j)) + ",";
				}
			}
			mConsole->print(str);
		}
	}
	if (mCommand == "mapcol")
	{
		for (int i = 0; i < mapNS::MAP_HEIGHT; ++i)
		{
			string str = "";
			for (int j = 0; j < mapNS::MAP_WIDTH; ++j)
			{
				if (mStage.getMap().getMapCol(i, j) >= 0)
				{
					str += " " + to_string(mStage.getMap().getMapCol(i, j)) + ",";
				}
				else
				{
					str += to_string(mStage.getMap().getMapCol(i, j)) + ",";
				}
			}
			mConsole->print(str);
		}
	}
	if (mCommand == "barricade")
	{
		for (int i = 0; i < 8; ++i)
		{
			mConsole->print(to_string(mStage.getBarricade(i).getActive()));
		}
	}
	if (mCommand == "enemystate")
	{
		for (int i = 0; i < mStage.getEnemyNum(); ++i)
		{
			mConsole->print(to_string(mStage.getEnemy(i).getState()));
		}
	}
	if (mCommand == "enemytile")
	{
		for (int i = 0; i < mStage.getEnemyNum(); ++i)
		{
			mConsole->print(to_string(mStage.getEnemy(i).getTileX()) + "," + to_string(mStage.getEnemy(i).getTileY()));
		}
	}
}

//==========================================================
// GameOver時に呼び出す
//==========================================================
void CastleDiffender::gameOver()
{
	// ステージ中のBGMをオフ
	mAudio->stopCue("stage");
	// ゲームオーバーBGMをオン
	mAudio->playCue("gameover");
}

//==========================================================
// ステージクリア時に呼び出す
//==========================================================
void CastleDiffender::clearStage()
{
	// ステージ中のBGMをオフ
	mAudio->stopCue("stage");
	// ステージクリアBGMをオン
	mAudio->playCue("clear");
	// ステージのハイスコアを更新した場合、新しくハイスコアを書き込む
	if (mHighScores[mStage.getStageNum()] < (int)mStage.getRemainingTime())
	{
		mHighScores[mStage.getStageNum()] = (int)mStage.getRemainingTime();
	}
	// 各ステージのハイスコアを書き込む
	ofstream ofs("savedata\\highscore.csv");
	for (int i = 0; i < 3; ++i)
	{
		ofs << mHighScores[i] << std::endl;
	}

}

//==========================================================
// 敵のテクスチャを初期化する
//==========================================================
void CastleDiffender::initializeEnemiesTexture()
{
	// それぞれの敵について
	for (int i = 0; i < mStage.getEnemyNum(); i++) {
		// 雑魚敵か中ボスかで初期化を分岐
		if (typeid(mStage.getEnemy(i)) == typeid(Enemy))
		{
			if (!mStage.getEnemy(i).initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, enemyNS::TEXTURE_COLS, &mEnemyTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy"));
		}
		else if (typeid(mStage.getEnemy(i)) == typeid(MidBoss))
		{
			if (!mStage.getEnemy(i).initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, enemyNS::TEXTURE_COLS, &mMidBossTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing midBoss"));
		}

		// 雑魚敵の当たり判定用エンティティを初期化
		if (!mStage.getEnemy(i).getAttackCollision().initialize(this, enemyCollisionNS::ATTACK_WIDTH, enemyCollisionNS::ATTACK_HEIGHT, 0, &mAttackCollisionTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy attack collision"));
	}
	// 敵の初期化フラグをオフ
	mStage.setInitializedEnemies(false);
}

//==========================================================
// 各ステージのハイスコアを読み込み
//==========================================================
void CastleDiffender::loadHighScore()
{
	ifstream ifs("savedata\\highscore.csv");
	//csvファイルを1行ずつ読み込む
	string str;
	if (getline(ifs, str))
	{
		string token;
		istringstream stream(str);
		// すべてのステージについてハイスコアを読み込む
		for (int i = 0; i < 3; ++i)
		{
			if (getline(stream, token, ','))
			{
				mHighScores[i] = (int)stof(token);
			}
		}
	}
}