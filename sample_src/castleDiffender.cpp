//==========================================================
/// @file
/// @brief    castleDiffender.h�̎���
/// @author   �������V
///
/// @attention  �Q�[���̃��C���N���X�ł��B
///				1�t���[�����Ƃ̍X�V��`��͂��̃N���X�ōs���܂��B

//==========================================================

#include "castleDiffender.h"

//==========================================================
// �R���X�g���N�^
//==========================================================
CastleDiffender::CastleDiffender()
{
	mInitialized = false;	// �Q�[�������������ꂽ�ꍇ�Atrue�ɂȂ�
	mFontCK = new Text();	// �X�v���C�g�t�H���g
	mMenuOn = true;			// ���j���[�\���t���O
	mStageSelectOn = false;	// �X�e�[�W�I����ʕ\���t���O
	mDescriptionOn = false;	// ���������ʕ\���t���O
	mRect = NULL;			// �V�X�e���O���t�B�b�N�X�p�̎l�p�`
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CastleDiffender::~CastleDiffender()
{
	safeDelete(mRect);		// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�̃����������
	safeDelete(mFontCK);		// �X�v���C�g�t�H���g�̃����������
	releaseAll();			// ���ׂẴO���t�B�b�N�X�A�C�e���ɂ��āAonLostDevice()���Ăяo��
}

//==========================================================
// �Q�[����������
// �G���[����GameError���X���[
//==========================================================
void CastleDiffender::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	// GameError���X���[

	// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�𐶐��A������
	mRect = new Rect();
	mRect->initialize(mGraphics);

	// �X�v���C�g�e�L�X�g��������
	if (!mFontCK->initialize(mGraphics, FONT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));


	// ���j���[�̃e�N�X�`��
	if (!mMenuTexture.initialize(mGraphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	// ���j���[�̉摜
	if (!mMenu.initialize(mGraphics, 0, 0, 0, &mMenuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// �X�e�[�W�I����ʂ̃e�N�X�`��
	if (!mStageSelectTexture.initialize(mGraphics, STAGE_SELECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage select texture"));
	// �X�e�[�W�I����ʂ̉摜
	if (!mStageSelect.initialize(mGraphics, 0, 0, 0, &mStageSelectTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage select"));

	// ���U���g�̃e�N�X�`��
	if (!mResultTexture.initialize(mGraphics, RESULT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing result texture"));
	// ���U���g�̉摜
	if (!mResult.initialize(mGraphics, 0, 0, 0, &mResultTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing result"));
	mResult.setX(GAME_WIDTH / 2.0f - mResult.getWidth() / 2.0f);
	mResult.setY((float)-mResult.getHeight());

	// �X�e�[�W�N���A��ʂ̃e�N�X�`��
	if (!mStageClearTexture.initialize(mGraphics, STAGE_CLEAR_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage clear texture"));
	// �X�e�[�W�N���A�摜
	if (!mStageClear.initialize(mGraphics, 0, 0, 0, &mStageClearTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing stage clear"));
	mStageClear.setX(GAME_WIDTH / 2.0f - mStageClear.getWidth() / 2.0f);
	mStageClear.setY((float)GAME_HEIGHT);

	// ��������̃e�N�X�`��
	if (!mDescriptionTexture.initialize(mGraphics, DESCRIPTION_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing description texture"));
	// ��������̉摜
	if (!mDescription.initialize(mGraphics, 0, 0, 0, &mDescriptionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing description"));

	// �}�b�v�̃e�N�X�`��
	if (!mTileTexture.initialize(mGraphics, TILE_IMAGES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing map texture"));
	// �}�b�v�̉摜
	if (!mStage.getMap().initialize(this, mapNS::TEXTURE_SIZE, mapNS::TEXTURE_SIZE, mapNS::TEXTURE_COLS, &mTileTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));

	// �o���P�[�h�̃e�N�X�`��
	if (!mBarricadeTexture.initialize(mGraphics, BARRICADE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing barricade texture"));
	if (!mHitEffectTexture.initialize(mGraphics, HIT_EFFECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hit effect texture"));
	// �o���P�[�h�̉摜
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		if (!mStage.getBarricade(i).initialize(this, barricadeNS::WIDTH, barricadeNS::HEIGHT, barricadeNS::TEXTURE_COLS, &mBarricadeTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing barricade"));
		mStage.getBarricade(i).setScale(1);
		if (!mStage.getBarricade(i).getHitEffect().initialize(mGraphics, hitEffectNS::WIDTH, hitEffectNS::HEIGHT, hitEffectNS::TEXTURE_COLS, &mHitEffectTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hitEffect"));
	}

	// ��̃e�N�X�`��
	if (!mCastleTexture.initialize(mGraphics, CASTLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle texture"));
	// ��̉摜
	if (!mStage.getCastle().initialize(this, castleNS::WIDTH, castleNS::HEIGHT, castleNS::TEXTURE_COLS, &mCastleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle"));

	// ��̃A�C�R���̃e�N�X�`��
	if (!mCastleIconTexture.initialize(mGraphics, CASTLE_ICON_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle icon texture"));
	// ��̃A�C�R���̉摜
	if (!mCastleIcon.initialize(mGraphics, castleIconNS::WIDTH, castleIconNS::HEIGHT, castleIconNS::TEXTURE_COLS, &mCastleIconTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing castle icon"));

	// �E�҂̃e�N�X�`��
	if (!mBraveTexture.initialize(mGraphics, BRAVE_MOVE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move texture"));
	// �E��
	if (!mStage.getBrave().initialize(this, braveNS::WIDTH, braveNS::HEIGHT, braveNS::TEXTURE_COLS, &mBraveTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave_move"));
	mStage.getBrave().setScale(1.5);
	mStage.getBrave().setFrames(braveNS::MOVE_UP_START_FRAME, braveNS::MOVE_UP_END_FRAME);
	mStage.getBrave().setCurrentFrame(braveNS::MOVE_UP_START_FRAME);
	mStage.getBrave().setMapPointer(mStage.getMap());

	// �E�҂̃A�C�R���̃e�N�X�`��
	if (!mBraveIconTexture.initialize(mGraphics, BRAVE_ICON_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave icon texture"));
	// �E�҂̃A�C�R���̉摜
	if (!mBraveIcon.initialize(mGraphics, braveIconNS::WIDTH, braveIconNS::HEIGHT, braveIconNS::TEXTURE_COLS, &mBraveIconTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing brave icon"));
	mBraveIcon.linkEntity(mStage.getBrave());

	// �E�҂̓����蔻��p�̃e�N�X�`��
	if (!mAttackCollisionTexture.initialize(mGraphics, COLLISION_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack collision texture"));
	// �E�҂̂����蔻��p
	if (!mStage.getBrave().getBraveAttackCollision().initialize(this, braveAttackCollisionNS::WIDTH, braveAttackCollisionNS::HEIGHT, 0, &mAttackCollisionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack collision"));
	if (!mAttackEffectTexture.initialize(mGraphics, ATTACK_EFFECT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack effect texture"));
	if (!mStage.getBrave().getBraveAttackCollision().getAttackEffect().initialize(mGraphics, attackEffectNS::WIDTH, attackEffectNS::HEIGHT, attackEffectNS::TEXTURE_COLS, &mAttackEffectTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing attack effect"));

	// ���̃e�N�X�`��
	if (!mFireTexture.initialize(mGraphics, FIRE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fire texture"));
	if (!mStage.getBrave().getFire().initialize(this, fireNS::WIDTH, fireNS::HEIGHT, fireNS::TEXTURE_COLS, &mFireTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fire"));
	mStage.getBrave().getFire().setScale(1.5);

	// �G���G�̃e�N�X�`��
	if (!mEnemyTexture.initialize(mGraphics, ENEMY_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy texture"));
	// ���{�X�̃e�N�X�`��
	if (!mMidBossTexture.initialize(mGraphics, MID_BOSS_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing midBoss texture"));

	// �_�b�V���{�[�h
	if (!mDashboardTextures.initialize(mGraphics, DASHBOARD_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dashboard textures"));
	mBraveHealthBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::BRAVE_HEALTH_BAR_X, castleDiffenderNS::BRAVE_HEALTH_BAR_Y, 0.5f, 100, graphicsNS::RED);
	mBraveHealthBar.set(mStage.getBrave().getHealth());
	mBraveMpBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::BRAVE_MP_BAR_X, castleDiffenderNS::BRAVE_MP_BAR_Y, 0.5f, 100, graphicsNS::GREEN);
	mBraveMpBar.set((float)mStage.getBrave().getMP());
	mCastleHealthBar.initialize(mGraphics, &mDashboardTextures, castleDiffenderNS::CASTLE_HEALTH_BAR_X + 40, castleDiffenderNS::CASTLE_HEALTH_BAR_Y, 0.5f, 100, graphicsNS::BLUE);
	mCastleHealthBar.set(mStage.getCastle().getHealth());

	// �e�L�X�g�摜
	if (!mTextTexture.initialize(mGraphics, TEXT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing text textures"));
	if (!mBraveHpText.initialize(mGraphics, hpTextImageNS::WIDTH, hpTextImageNS::HEIGHT, hpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	if (!mBraveMpText.initialize(mGraphics, mpTextImageNS::WIDTH, mpTextImageNS::HEIGHT, mpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	if (!mCastleHpText.initialize(mGraphics, hpTextImageNS::WIDTH, hpTextImageNS::HEIGHT, hpTextImageNS::TEXTURE_COLS, &mTextTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hp text"));
	mCastleHpText.setX(830);

	// �^�C�g��BGM�Đ�
	mAudio->playCue("title");

	// �e�X�e�[�W�̃n�C�X�R�A�ǂݎ��
	loadHighScore();
	return;
}

//==========================================================
// �v���C�̐V�������E���h���J�n
//==========================================================
void CastleDiffender::roundStart()
{
	// ���݂̃X�e�[�W�ԍ��ŃQ�[���J�n
	mStage.roundStart();
	// ���U���g��ʂ̈ʒu�����Z�b�g
	mResult.setY((float)-mResult.getHeight());
	// �X�e�[�W�N���A��ʂ̈ʒu�����Z�b�g
	mStageClear.setY((float)GAME_HEIGHT);
	// �E�҂̃A�C�R���̏�����
	mBraveIcon.reset();
	// ���E���h���J�n����܂ł̎��Ԃ����Z�b�g
	mRoundTimer = castleDiffenderNS::ROUND_TIME;
	// �X�e�[�W��BGM�Đ�
	mAudio->playCue("stage");
}

//==========================================================
// ���ׂẴQ�[���A�C�e�����X�V
//==========================================================
void CastleDiffender::update()
{
	if (mMenuOn)		// ���j���[��ʕ\����
	{
		if (mInput->isKeyDown('Z')) // Z�L�[�ŃX�e�[�W�I����ʂɈȍ~
		{
			// ���j���[��ʃI�t
			mMenuOn = false;
			// �X�e�[�W�I����ʂɑJ��
			mStageSelectOn = true;
			// ���͂��N���A
			mInput->clearAll();
			// �X�e�[�W�������Z�b�g
			mStage.reset();
			// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�����Z�b�g
			mRect->setX(380);
			mRect->setY(80);
			mRect->setWidth(510);
			mRect->setHeight(81);
			mRect->setBackColor(SETCOLOR_ARGB(50, 120, 120, 255));
			mRect->reset();
		}
		else if (mInput->isKeyDown('X')) // X�L�[�ő�������\��
		{
			// ���j���[��ʃI�t
			mMenuOn = false;
			// ���������ʂɑJ��
			mDescriptionOn = true;
			// ���͂��N���A
			mInput->clearAll();
		}
		else if (mInput->isKeyDown('E'))	// E�L�[�ŃQ�[���I��
		{
			// �Q�[���I��
			exitGame();
		}
	}
	else if (mStageSelectOn)	// �X�e�[�W�I����ʕ\����
	{
		if (mInput->isKeyDown('Z'))	// Z�L�[�ŃX�e�[�W����
		{
			// �X�e�[�W�I����ʃI�t
			mStageSelectOn = false;
			// �^�C�g��BGM���I�t
			mAudio->stopCue("title");
			// �Q�[���X�^�[�g
			roundStart();
			// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�����Z�b�g
			mRect->setX((float)rectNS::X);
			mRect->setY((float)rectNS::Y);
			mRect->setWidth(rectNS::WIDTH);
			mRect->setHeight(rectNS::HEIGHT);
			mRect->setBackColor(rectNS::BACK_COLOR);
			mRect->reset();
		}
		else if (mInput->isKeyDown(BRAVE_DOWN_KEY))	// ���L�[�œ���X�e�[�W��I��
		{
			// �X�e�[�W�ԍ����ő�X�e�[�W����菭�Ȃ���΁A
			if (mStage.getStageNum() < 3 - 1)
			{
				// �X�e�[�W�����C���N�������g
				mStage.setStageNum(mStage.getStageNum() + 1);
				// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�����Z�b�g
				mRect->setY(mRect->getY() + 240);
				mRect->reset();
			}
			// ���͂��N���A
			mInput->clearAll();
		}
		else if (mInput->isKeyDown(BRAVE_UP_KEY))	// ��L�[�ŊȒP�ȃX�e�[�W��I��
		{
			// �X�e�[�W�ԍ����ŏ��X�e�[�W����葽����΁A
			if (mStage.getStageNum() > 0)
			{
				// �X�e�[�W�����f�N�������g
				mStage.setStageNum(mStage.getStageNum() - 1);
				// �V�X�e���O���t�B�b�N�X�p�̎l�p�`�����Z�b�g
				mRect->setY(mRect->getY() - 240);
				mRect->reset();
			}
			// ���͂��N���A
			mInput->clearAll();
		}
	}
	else if (mDescriptionOn) // ���������
	{
		// X�L�[�Ń^�C�g���ɖ߂�
		if (mInput->isKeyDown('X'))
		{
			// ���������ʃI�t
			mDescriptionOn = false;
			// ���j���[��ʃI��
			mMenuOn = true;
			// ���͂��N���A
			mInput->clearAll();
		}
	}
	else if (mStage.getRoundOver())	// �Q�[���I�[�o�[��
	{
		// ���U���g��ʂ����ȏ�~��������
		if (mResult.getY() > 50)
		{
			// X�������ꂽ�烁�j���[��ʂɖ߂�
			if (mInput->isKeyDown('X'))
			{
				// �Q�[���I�[�o�[�t���O�����Z�b�g
				mStage.setRoundOver(false);
				// ���j���[��ʃI��
				mMenuOn = true;
				// ���͂��N���A
				mInput->clearAll();
				// �Q�[���I�[�o�[BGM���I�t
				mAudio->stopCue("gameover");
				// �^�C�g��BGM���I��
				mAudio->playCue("title");
			}
			// E�������ꂽ��Q�[���I��
			else if (mInput->isKeyDown('E'))
			{
				// �Q�[���I��
				exitGame();
			}
			// Z�������ꂽ��X�e�[�W�����g���C
			else if (mInput->isKeyDown('Z'))
			{
				// ���͂��N���A
				mInput->clearAll();
				// �Q�[���I�[�o�[BGM���I�t
				mAudio->stopCue("gameover");
				// �X�e�[�W�X�^�[�g
				roundStart();
			}
		}
		else // ���U���g��ʂ����ȏ�~�����Ă��Ȃ����
		{
			// ���U���g��ʂ��~��
			mResult.setY(mResult.getY() + mFrameTime * 120.0f);
		}
	}
	else if (mStage.getClearedStage())	// �X�e�[�W���N���A���Ȃ�
	{
		// �X�e�[�W�N���A��ʂ����ȏ�㏸������
		if (mStageClear.getY() < 250)
		{
			// X�������ꂽ�烁�j���[��ʂɖ߂�
			if (mInput->isKeyDown('X'))
			{
				// ���j���[��ʃI��
				mMenuOn = true;
				// ���͂��N���A
				mInput->clearAll();
				// �N���ABGM���I�t
				mAudio->stopCue("clear");
				// �^�C�g��BGM���I��
				mAudio->playCue("title");
				// �X�e�[�W�N���A�t���O�����Z�b�g
				mStage.setClearedStage(false);
			}
			// E�������ꂽ��Q�[���I��
			else if (mInput->isKeyDown('E'))
			{
				// �Q�[���I��
				exitGame();
			}
			// Z�������ꂽ��X�e�[�W�����g���C
			else if (mInput->isKeyDown('Z'))
			{
				// ���͂��N���A
				mInput->clearAll();
				// �N���ABGM���I�t
				mAudio->stopCue("clear");
				// �X�e�[�W�X�^�[�g
				roundStart();
			}
		}
		else // �X�e�[�W�N���A��ʂ����ȏ�㏸���Ă��Ȃ����
		{
			// �X�e�[�W�N���A��ʂ��㏸
			mStageClear.setY(mStageClear.getY() - mFrameTime * 320.0f);
		}
	}
	else // �Q�[�����̏ꍇ�A
	{
		// ���E���h�J�n�܂ł̎��Ԃ��߂��Ă��Ȃ���΃X�e�[�W���͍X�V���Ȃ�
		if (mRoundTimer > 0.0f)
		{
			mRoundTimer -= mFrameTime;
			return;
		}
		// �X�e�[�W�����X�V
		mStage.runStage(mFrameTime);
		// �G����������������Ȃ�
		if (mStage.getInitializedEnemies())
		{
			// �G�̃e�N�X�`����������
			initializeEnemiesTexture();
		}
		// �Q�[���I�[�o�[�Ȃ�
		if (mStage.getRoundOver())
		{
			// �Q�[���I�[�o�[���̏��������s
			gameOver();
		}
		// �X�e�[�W���N���A������
		if (mStage.getClearedStage())
		{
			// �X�e�[�W�N���A���̏��������s
			clearStage();
		}

		// �E�҂̃A�C�R���摜���X�V
		mBraveIcon.update(mFrameTime);
	}
}

//==========================================================
// �l�H�m�\
//==========================================================
void CastleDiffender::ai()
{
	// �X�e�[�W���̃Q�[���A�C�e���̐l�H�m�\
	mStage.ai(mFrameTime);
}

//==========================================================
// �Փ˂�����
//==========================================================
void CastleDiffender::collisions()
{
	// �X�e�[�W���̃Q�[���A�C�e���̏Փ˂�����
	mStage.collisions();
}

//==========================================================
// �Q�[���A�C�e���������_�[
// �v���C���̃��C���[�F�}�b�v�����w�i�����̑��A�C�e���̏�
//==========================================================
void CastleDiffender::render()
{
	// �X�v���C�g�̕`����J�n
	mGraphics->spriteBegin();	
	if (mMenuOn)	// ���j���[��ʃI���̏ꍇ�A
	{
		// ���j���[��ʕ`��
		drawMenu();
	}
	else if (mStageSelectOn)	// �X�e�[�W�I����ʃI���̏ꍇ�A
	{
		// �X�e�[�W�I����ʕ`��
		drawStageSelect();
	}
	else if (mDescriptionOn)	// ���������ʃI���̏ꍇ�A
	{
		// ���������ʕ`��
		drawDescription();
	}
	else // �X�e�[�W���̏ꍇ�A
	{
		// �X�e�[�W��ʂ�`��
		// �X�e�[�W�v���C���͒ʏ킱���ŕ`�悳���
		drawOnStage();
	}

	// �Q�[���I�[�o�[��ʂƃX�e�[�W�N���A��ʂ̓Q�[����ʂ̈ꕔ�ɕ\������邽�߁A�Q�[����ʂƏd�˂ĕ\��
	if (mStage.getRoundOver())	// �Q�[���I�[�o�[�̏ꍇ�A
	{
		// �Q�[���I�[�o�[��ʂ�`��
		drawOnRoundOver();
	}
	else if (mStage.getClearedStage())	// �X�e�[�W�N���A�̏ꍇ�A
	{
		// �X�e�[�W�N���A��ʂ�`��
		drawOnClearedStage();
	}
	// �X�v���C�g�̕`����J�n
	mGraphics->spriteEnd();
}

//==========================================================
// ���j���[��ʎ��̕`��
//==========================================================
void CastleDiffender::drawMenu()
{
	// ���j���[��ʂ�`��
	mMenu.draw();
	// �V�X�e���O���t�B�b�N�X��`��
	char str[128] = "PUSH Z KEY : STAGE SELECT";
	char str2[128] = "PUSH X KEY : OPERATION DESCRIPTION";
	char str3[128] = "PUSH E KEY : EXIT";
	mFontCK->setFontHeight(35);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
	mFontCK->print(str, 273, 353);
	mFontCK->print(str2, 273, 453);
	mFontCK->print(str3, 273, 553);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));
	mFontCK->print(str, 273, 350);
	mFontCK->print(str2, 273, 450);
	mFontCK->print(str3, 273, 550);
}

//==========================================================
// �X�e�[�W�I����ʎ��̕`��
//==========================================================
void CastleDiffender::drawStageSelect()
{
	// �X�e�[�W�I����ʂ�`��
	mStageSelect.draw();
	// �X�v���C�g�̕`����I��
	mGraphics->spriteEnd();
	// �V�X�e���O���t�B�b�N�X�p�̎l�p�`��`��
	mRect->draw();
	// �X�v���C�g�̕`����J�n
	mGraphics->spriteBegin();
	// �V�X�e���O���t�B�b�N�X��`��
	char str[128] = "PUSH Z KEY : START STAGE!";
	mFontCK->setFontHeight(35);
	mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
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
// ���������ʎ��̕`��
//==========================================================
void CastleDiffender::drawDescription()
{
	// ���������ʂ��I��
	mDescription.draw();
}

//==========================================================
// �X�e�[�W���̕`��
//==========================================================
void CastleDiffender::drawOnStage()
{
	// �}�b�v�^�C����`��
	for (int row = 0; row < mapNS::MAP_HEIGHT; row++)       // �}�b�v�̊e�s������
	{
		mStage.getMap().setY((float)(row*mapNS::TEXTURE_SIZE));     // �^�C����Y��ݒ�
		for (int col = 0; col < mapNS::MAP_WIDTH; col++)			// �}�b�v�̊e�������
		{
			if (mStage.getMap().getMapData(row, col) >= 0)          // �^�C�������݂���ꍇ�i���̏����͂���Ȃ��Ǝv���邪�O�̂��߁j
			{
				mStage.getMap().setCurrentFrame(mStage.getMap().getMapData(row, col));                      // �^�C���̃e�N�X�`����ݒ�
				mStage.getMap().setX((float)(col*mapNS::TEXTURE_SIZE));										// �^�C����X��ݒ�
				if (mStage.getMap().getX() > -mapNS::TEXTURE_SIZE && mStage.getMap().getX() < GAME_WIDTH)   // �^�C������ʏ�ɂ��邩�ǂ���
					mStage.getMap().draw();    // �^�C����`��
			}
		}
	}
	// �o���P�[�h��`��
	for (int i = 0; i < 8; ++i)
	{
		mStage.getBarricade(i).draw();				// �I�u�W�F�N�g��`��
		mStage.getBarricade(i).getHitEffect().draw();// �o���P�[�h�ɍU�����q�b�g�����Ƃ��̃A�j���[�V������`��
	}

	// �X�v���C�g�̕`����I��
	mGraphics->spriteEnd();
	// �V�X�e���O���t�B�b�N�X�p�̎l�p�`��`��
	mRect->draw();
	// �X�v���C�g�̕`����J�n
	mGraphics->spriteBegin();

	// �G��`��
	for (int i = 0; i < mStage.getEnemyNum(); i++) {
		// �摜�ɕ΂肪���邽�߁A�ʒu���C��
		float tmpX = mStage.getEnemy(i).getX();
		float tmpY = mStage.getEnemy(i).getY();
		mStage.getEnemy(i).setX((float)(tmpX - mStage.getEnemy(i).getWidth() * (mStage.getEnemy(i).getScale() - 1) / 2.0));
		mStage.getEnemy(i).setY((float)(tmpY - mStage.getEnemy(i).getHeight() * (mStage.getEnemy(i).getScale() - 1) / 2.0 - 10.0));
		// �G�̎�ނɉ����ăJ���[�t�B���^�[��ݒ�
		// �G��`��
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
		// �G�̍U������p�̃G���e�B�e�B��`��i�f�o�b�O�p�ɐF��t���ĕ\���ł���悤�ɂ��Ă���j
		mStage.getEnemy(i).getAttackCollision().draw();
		// �ʒu�����Ƃɖ߂�
		mStage.getEnemy(i).setX(tmpX);
		mStage.getEnemy(i).setY(tmpY);
	}
	// �v���C���[�̃A�C�R����`��
	mBraveIcon.draw();
	// ��̃A�C�R����`��
	mCastleIcon.draw();
	// ���`��
	mStage.getCastle().draw();
	// �v���C���[�̉���`��
	mStage.getBrave().getFire().draw();
	// �v���C���[��`��
	// �摜�ɕ΂肪���邽�߁A�ʒu���C��
	float tmpX = mStage.getBrave().getX();
	float tmpY = mStage.getBrave().getY();
	mStage.getBrave().setX((float)(tmpX - mStage.getBrave().getWidth() * (mStage.getBrave().getScale() - 1) / 2.0));
	mStage.getBrave().setY((float)(tmpY - mStage.getBrave().getHeight() * (mStage.getBrave().getScale() - 1) / 2.0 - 10));
	mStage.getBrave().draw(graphicsNS::WHITE);
	mStage.getBrave().setX(tmpX);
	mStage.getBrave().setY(tmpY);
	// �v���C���[�̍U������p�̃G���e�B�e�B��`��i�f�o�b�O�p�ɐF��t���ĕ\���ł���悤�ɂ��Ă���j
	mStage.getBrave().getBraveAttackCollision().draw();
	// �v���C���[�̍U���̏Ռ��h��`��
	mStage.getBrave().getBraveAttackCollision().getAttackEffect().draw();
	// �v���C���[��HP��_�O���t�ŕ`��
	mBraveHealthBar.set(mStage.getBrave().getHealth());
	mBraveHealthBar.draw(graphicsNS::FILTER);
	// �v���C���[��MP��_�O���t�ŕ`��
	mBraveMpBar.set((float)mStage.getBrave().getMP());
	mBraveMpBar.draw(graphicsNS::FILTER);
	// ���HP��_�O���t�ŕ`��
	mCastleHealthBar.set(mStage.getCastle().getHealth());
	mCastleHealthBar.draw(graphicsNS::FILTER);

	// �V�X�e���O���t�B�b�N�X��`��
	// �E�җp��HP�e�L�X�g��`��
	mBraveHpText.draw();
	// �E�җp��MP�e�L�X�g��`��
	mBraveMpText.draw();
	// ��p��HP�e�L�X�g��`��
	mCastleHpText.draw();
	// �V�X�e���e�L�X�g��`��
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
		// ���E���h���J�n����܂ł́A�uSTAGE START!�v�ƈ�莞�ԕ\��������
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
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 303);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 403);
			mFontCK->setFontColor(graphicsNS::BLUE);
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 300);
			mFontCK->setFontColor(graphicsNS::RED);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer) * fastSpeed), 400);
		}
		else if (mRoundTimer > castleDiffenderNS::ROUND_TIME - T * 2.0f)
		{
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 303);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 403);
			mFontCK->setFontColor(graphicsNS::BLUE);
			mFontCK->print(str1, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 300);
			mFontCK->setFontColor(graphicsNS::RED);
			mFontCK->print(str2, (int)(initX + (castleDiffenderNS::ROUND_TIME - mRoundTimer - T) * secondSpeed + fastSpeed * T), 400);
		}
		else
		{
			mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
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
// �Q�[���I�[�o�[���̕`��
//==========================================================
void CastleDiffender::drawOnRoundOver()
{
	// �Q�[���I�[�o�[��ʂ�`��
	mResult.draw();
	// ���U���g��ʂ����ȏ�~��������
	if (mResult.getY() > 50)
	{
		// �V�X�e���O���t�B�b�N�X�̃e�L�X�g��\��
		char str[128] = "CONTINUE?";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 35 * 9 / 2), (int)(mResult.getHeight() + mResult.getY()));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 35 * 9 / 2), (int)(mResult.getHeight() + mResult.getY() - 3));
		char str2[128] = "PUSH Z KEY : RETRY STAGE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55));
		char str3[128] = "PUSH X KEY : RETURN TITLE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55 * 2));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55 * 2));
		char str4[128] = "PUSH E KEY : EXIT";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() + 55 * 3));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mResult.getHeight() + mResult.getY() - 3 + 55 * 3));
	}
}

//==========================================================
// �X�e�[�W�N���A���̕`��
//==========================================================
void CastleDiffender::drawOnClearedStage()
{
	// �X�e�[�W�N���A��ʂ�`��
	mStageClear.draw();
	// �X�e�[�W�N���A��ʂ����ȏ�㏸������
	if (mStageClear.getY() < 250)
	{
		// �V�X�e���O���t�B�b�N�X�̃e�L�X�g��\��
		char str[128] = "YOUR SCORE IS ";
		char time[5] = { 0 };
		sprintf_s(time, "%04d", (int)mStage.getRemainingTime());
		strcat_s(str, time);
		mFontCK->setFontHeight(45);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 45 * 16 / 2), (int)(mStageClear.getHeight() + mStageClear.getY()));
		mFontCK->setFontHeight(45);
		mFontCK->setFontColor(graphicsNS::RED);
		mFontCK->print(str, (int)(GAME_WIDTH / 2 - 45 * 16 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3));
		char str2[128] = "PUSH Z KEY : RETRY STAGE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str2, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55));
		char str3[128] = "PUSH X KEY : RETURN TITLE";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55 * 2));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str3, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55 * 2));
		char str4[128] = "PUSH E KEY : EXIT";
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 0, 0, 0));  // �e
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() + 55 * 3));
		mFontCK->setFontHeight(35);
		mFontCK->setFontColor(SETCOLOR_ARGB(255, 255, 255, 50));
		mFontCK->print(str4, (int)(GAME_WIDTH / 2 - 35 * 20 / 2), (int)(mStageClear.getHeight() + mStageClear.getY() - 3 + 55 * 3));
	}
}

//==========================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
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
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
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
// �R���\�[���R�}���h������
// �f�o�b�O�p
//=============================================================================
void CastleDiffender::consoleCommand()
{
	mCommand = mConsole->getCommand();    // �R���\�[������̃R�}���h���擾
	if (mCommand == "")                  // �R�}���h���Ȃ��ꍇ
		return;

	if (mCommand == "help")              // �uhelp�v�R�}���h�̏ꍇ
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
		mFpsOn = !mFpsOn;                 // �t���[�����[�g�̕\����؂�ւ���
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
// GameOver���ɌĂяo��
//==========================================================
void CastleDiffender::gameOver()
{
	// �X�e�[�W����BGM���I�t
	mAudio->stopCue("stage");
	// �Q�[���I�[�o�[BGM���I��
	mAudio->playCue("gameover");
}

//==========================================================
// �X�e�[�W�N���A���ɌĂяo��
//==========================================================
void CastleDiffender::clearStage()
{
	// �X�e�[�W����BGM���I�t
	mAudio->stopCue("stage");
	// �X�e�[�W�N���ABGM���I��
	mAudio->playCue("clear");
	// �X�e�[�W�̃n�C�X�R�A���X�V�����ꍇ�A�V�����n�C�X�R�A����������
	if (mHighScores[mStage.getStageNum()] < (int)mStage.getRemainingTime())
	{
		mHighScores[mStage.getStageNum()] = (int)mStage.getRemainingTime();
	}
	// �e�X�e�[�W�̃n�C�X�R�A����������
	ofstream ofs("savedata\\highscore.csv");
	for (int i = 0; i < 3; ++i)
	{
		ofs << mHighScores[i] << std::endl;
	}

}

//==========================================================
// �G�̃e�N�X�`��������������
//==========================================================
void CastleDiffender::initializeEnemiesTexture()
{
	// ���ꂼ��̓G�ɂ���
	for (int i = 0; i < mStage.getEnemyNum(); i++) {
		// �G���G�����{�X���ŏ������𕪊�
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

		// �G���G�̓����蔻��p�G���e�B�e�B��������
		if (!mStage.getEnemy(i).getAttackCollision().initialize(this, enemyCollisionNS::ATTACK_WIDTH, enemyCollisionNS::ATTACK_HEIGHT, 0, &mAttackCollisionTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy attack collision"));
	}
	// �G�̏������t���O���I�t
	mStage.setInitializedEnemies(false);
}

//==========================================================
// �e�X�e�[�W�̃n�C�X�R�A��ǂݍ���
//==========================================================
void CastleDiffender::loadHighScore()
{
	ifstream ifs("savedata\\highscore.csv");
	//csv�t�@�C����1�s���ǂݍ���
	string str;
	if (getline(ifs, str))
	{
		string token;
		istringstream stream(str);
		// ���ׂẴX�e�[�W�ɂ��ăn�C�X�R�A��ǂݍ���
		for (int i = 0; i < 3; ++i)
		{
			if (getline(stream, token, ','))
			{
				mHighScores[i] = (int)stof(token);
			}
		}
	}
}