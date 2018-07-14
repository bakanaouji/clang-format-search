//==========================================================
/// @file
/// @brief    stage.h�̎���
/// @author   �������V
///
/// @attention  �X�e�[�W�����Ǘ�����N���X�ł��B
///				�E�ҁA�G�A�}�b�v���A�o���P�[�h�A��ȂǃX�e�[�W�Ɋւ�����͂����ŊǗ����܂��B

#include "stage.h"
#include <time.h>

//==========================================================
// �R���X�g���N�^
//==========================================================
Stage::Stage()
{
	mRoundOver = false;			// ���E���h���I�����Ă��邩�ǂ����i�Q�[���I�[�o�[���ǂ����j
	mClearedStage = false;		// �X�e�[�W���N���A�������ǂ���
	mEnemyNum = 0;				// �G�̐�
	mEnemy = NULL;				// �G�ւ̃|�C���^�̔z��
	mEnemyX = NULL;				// �G�̏����ʒuX�̔z��
	mEnemyY = NULL;				// �G�̏����ʒuY�̔z��
	mEnemyWaveNum = 0;			// ���݂̓G�̔g�̔ԍ��i���g�A���h�A�E�E�E�j
	mRemainingTime = 1500.0f;	// �Q�[�����̎c�莞��
	mStageNum = 0;				// �I�����Ă���X�e�[�W�ԍ�
}

//==========================================================
// �f�X�g���N�^
//==========================================================
Stage::~Stage()
{
	// �G�̃����������
	for (int i = 0; i < mEnemyNum; ++i)
	{
		safeDelete(mEnemy[i]);
	}
	safeDeleteArray(mEnemy);
	safeDeleteArray(mEnemyX);
	safeDeleteArray(mEnemyY);
}
//==========================================================
// �X�e�[�W����������
//==========================================================
void Stage::reset() 
{
	mRoundOver = false;			// ���E���h���I�����Ă��邩�ǂ����i�Q�[���I�[�o�[���ǂ����j
	mClearedStage = false;		// �X�e�[�W���N���A�������ǂ���
	mEnemyNum = 0;				// �G�̐�
	mEnemy = NULL;				// �G�ւ̃|�C���^�̔z��
	mEnemyX = NULL;				// �G�̏����ʒuX�̔z��
	mEnemyY = NULL;				// �G�̏����ʒuY�̔z��
	mEnemyWaveNum = 0;			// ���݂̓G�̔g�̔ԍ��i���g�A���h�A�E�E�E�j
	mRemainingTime = 1500.0f;	// �Q�[�����̎c�莞��
	mStageNum = 0;				// �I�����Ă���X�e�[�W�ԍ�
}

//==========================================================
// �Q�[���̐V�������E���h���J�n
//==========================================================
void Stage::roundStart()
{
	// �G�������Z�b�g
	mEnemyWaveNum = 0;
	mEnemyNum = 0;
	// �}�b�v�f�[�^��ǂݍ���
	mMap.readMapFile(mStageNum + 1);
	// �G�f�[�^�̓ǂݍ���
	initializeEnemies(mStageNum + 1, mEnemyWaveNum + 1);
	// �e�o���P�[�h��������
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		mBarricades[i].reset();
	}
	// ������
	// �}�b�v�ƃo���P�[�h��������
	int count = 0;  // �o���P�[�h�̐�������p
	for (int row = 0; row<mapNS::MAP_HEIGHT; row++)       // �}�b�v�̊e�s������
	{
		mMap.setY((float)(row*mapNS::TEXTURE_SIZE));      // �^�C����Y��ݒ�
		for (int col = 0; col<mapNS::MAP_WIDTH; col++)    // �}�b�v�̊e�������
		{
			if (mMap.getMapData(row, col) >= 0)           // �^�C�������݂���ꍇ�i���̏����͂���Ȃ��Ǝv���邪�O�̂��߁j
			{
				mMap.setCurrentFrame(mMap.getMapData(row, col));                // �^�C���̃e�N�X�`����ݒ�
				mMap.setX((float)(col*mapNS::TEXTURE_SIZE));                    // �^�C����X��ݒ�
			}
			if (mMap.getMapObj(row, col) == mapNS::OBJ_BARRICADE)
			{
				mBarricades[count].setX((float)(col*mapNS::TEXTURE_SIZE));		// �I�u�W�F�N�g��X��ݒ�
				mBarricades[count].setY((float)(row*mapNS::TEXTURE_SIZE));		// �I�u�W�F�N�g��Y��ݒ�
				mBarricades[count].setTileX(col);								// �I�u�W�F�N�g��X��ݒ�
				mBarricades[count].setTileY(row);								// �I�u�W�F�N�g��Y��ݒ�
				if (mBarricades[count].getX() > -mapNS::TEXTURE_SIZE && mBarricades[count].getX() < GAME_WIDTH)	// �I�u�W�F�N�g����ʏ�ɂ��邩�ǂ���
				{
					count++;
				}
			}
		}
	}
	// �E�҂�������
	mBrave.reset();
	// �e�G��������
	for (int i = 0; i < mEnemyNum; i++)
	{
		mEnemy[i]->reset();
		mEnemy[i]->setX(mEnemyX[i]);
		mEnemy[i]->setY(mEnemyY[i]);
		mEnemy[i]->initTileXY();
	}
	// ���������
	mCastle.reset();
	// �c�莞�Ԃ�������
	mRemainingTime = 1500.0f;
	// �Q�[���I�[�o�[�̃t���O��������
	mRoundOver = false;
	mClearedStage = false;

	// �����̃V�[�h��ݒ�
	srand((unsigned int)time(NULL));
}

//==========================================================
// �l�H�m�\
//==========================================================
void Stage::ai(float frameTime)
{
	// �e�G���X�V
	for (int i = 0; i < mEnemyNum; i++)
		mEnemy[i]->ai(frameTime, mBrave);
}

//==========================================================
// �X�e�[�W��1�X�e�b�v���i�߂�
//==========================================================
void Stage::runStage(float frameTime)
{
	// ���ꂼ��̓G���X�V
	for (int i = 0; i < mEnemyNum; i++) {
		mEnemy[i]->update(frameTime);
	}
	// �E�҂��X�V
	mBrave.update(frameTime);
	// ����X�V
	mCastle.update(frameTime);
	// ���ꂼ��̃o���P�[�h���X�V
	for (int i = 0; i < mapNS::BARRICADE_NUM; i++)
	{
		mBarricades[i].update(frameTime);
	}
	// �G�̐����`�F�b�N���āA�}�b�v��ɑ��݂��Ȃ���ΐV���ɐ���
	checkCurrentEnemyNum();
	// �c�莞�Ԃ�0�Ȃ�Q�[���I�[�o�[
	mRemainingTime -= frameTime;
	
	if (mRemainingTime < 0)
	{
		mRoundOver = true;
	}

	// ���S�`�F�b�N
	if ((mCastle.isDeath() || mBrave.getActive() == false) && !mRoundOver)
	{
		mRoundOver = true;
	}
}

//==========================================================
// �Փ˂�����
// ��������Փ˂͈ȉ��̒ʂ�F
// �E�҂̍U���Ɗe�G�Ƃ̏Փ�
// �e�G�̍U���ƗE�҂��A�o���P�[�h�Ƃ̏Փ�
// �E�҂̕K�E�Z�i���j�Ɗe�G�̏Փ�
// �e�G�ƗE�҂̏Փ�
// �E�҂̍U���ƃo���P�[�h�Ƃ̏Փ�
// �e�G�Ƃق��̃G���e�B�e�B�Ƃ̏Փˏ󋵂ɂ���āA�G�̏�Ԃ𕪊�
// �@�U�����͂��̂܂܍U��
// �A�E�҂ƏՓ˂��Ă���ꍇ�͗E�҂ɑ΂��Ă̍U����ԂɑJ��
// �B��ƏՓ˂��Ă���ꍇ�͏�ɑ΂��Ă̍U����ԂɑJ��
// �C�ŋߐڂ̃o���P�[�h�ɏՓ˂��Ă���ꍇ�̓o���P�[�h�ɑ΂��Ă̍U����ԂɑJ��
// �D�����ꂩ�̃}�b�v�^�C���̒����܂ňړ������ꍇ�A
//   �ŋߐڂ̃o���P�[�h�Ɍ������Ĉړ���ԂɑJ�ځi1/3���炢�̊m���ŕ����������_���Ɍ��߂�悤�ɂ��Ă���j
//==========================================================
void Stage::collisions()
{
	VECTOR2 collisionVector;	// �Փ˔��肵���Ƃ��̏Փˑ���ւ̃x�N�g��
	bool braveAttackCollidesWithAnyEnemy = false;
	// �e�G�ɂ��ďՓ˔���
	for (int i = 0; i < mEnemyNum; i++) {
		// �v���C���[�̍U���R���W�����ƎG���G�̏Փ˂̏ꍇ
		if (mBrave.getBraveAttackCollision().collidesWith(*mEnemy[i], collisionVector))
		{
			// �G�Ƀ_���[�W��^����
			mEnemy[i]->damage(BRAVE_ATTACK);
			// �����ꂩ�̓G�ɍU�������������̂Ń��[�v�𔲂�����ɍU���R���W�������Ȃ���
			braveAttackCollidesWithAnyEnemy = true;
		}

		if (mEnemy[i]->getState() == characterNS::DEATH)
		{
			continue;
		}

		// �G���U�����́A��ƃv���C���[�ƃo���P�[�h�Ƃ̓����蔻������ꂼ��`�F�b�N
		if (mEnemy[i]->getState() == characterNS::ATTACK)
		{
			// �G�̍U���R���W�����ƃv���C���[�̏Փ˂̏ꍇ
			if (mEnemy[i]->getAttackCollision().collidesWith(mBrave, collisionVector))
			{
				// �v���C���[�Ƀ_���[�W��^����
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
				// �G�̍U���R���W�������Ȃ���
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
			}
			// �G�̍U���R���W�����Ə�̏Փ˂̏ꍇ
			if (mEnemy[i]->getAttackCollision().collidesWith(mCastle, collisionVector))
			{
				// ��Ƀ_���[�W��^����
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
				// �G�̍U���R���W�������Ȃ���
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
			}
			// �G�̍U���R���W�����ƃo���P�[�h�̏Փ˂̏ꍇ
			if (mEnemy[i]->getAttackCollision().collidesWith(mBarricades[mEnemy[i]->getNearBarricadeIndex()], collisionVector)) {
				// �o���P�[�h�Ƀ_���[�W��^����ibarricades[enemy[i].getNearBarricadeIndex()]���U���ΏۂɂȂ�͂��j
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
				// �G�̍U���R���W�������Ȃ���
				mEnemy[i]->getAttackCollision().setVisible(false);
				mEnemy[i]->getAttackCollision().setActive(false);
				// �U�����ꂽ�o���P�[�h���j�󂳂ꂽ�ꍇ�A
				if (!mBarricades[mEnemy[i]->getNearBarricadeIndex()].getActive())
				{
					// �}�b�v�f�[�^���X�V
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

		// ���ƎG���G�̏Փ˂̏ꍇ
		if (mBrave.getFire().collidesWith(*mEnemy[i], collisionVector))
		{
			mEnemy[i]->damage(FIRE);
			mBrave.getFire().setVisible(false);
			mBrave.getFire().setActive(false);
		}

		// �U�����Ȃ�΍s���I���͍s��Ȃ�
		if (mEnemy[i]->getState() == characterNS::ATTACK)
		{
		}
		else if (mEnemy[i]->collidesWith(mBrave, collisionVector))// �G���G�ƃv���C���[���Փ˂��Ă���ꍇ�A
		{
			// �E�҂���������ɉ����čU�����������ύX
			mEnemy[i]->changeAttack(collisionVector);
		}
		else if (mMap.getMapObj(mEnemy[i]->getTileY(), mEnemy[i]->getTileX()) == mapNS::OBJ_NEAR_CASTLE)	// �G���G�Ə邪�Փ˂��Ă�����U���A
		{
			// �邪��������ɉ����čU�����������ύX
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
		else if (mMap.getMapObj(mEnemy[i]->getTileY(), mEnemy[i]->getTileX()) == mapNS::OBJ_NEAR_BARRICADE)	// �ŋߐڂ̃o���P�[�h�ɏՓ˂��Ă�����U��
		{
			// �o���P�[�h����������ɉ����čU�����������ύX
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
		else if (mEnemy[i]->canMakeDecesionMoving())	// �G�������]���\�ȏ�Ԃɂ���Ƃ��A�ӎv������s���i�ޕ��������߂�
		{
			bool changeGoalDirectionFlag = false;	// �i�݂���������ς���K�v�����邩�ǂ���
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
				// �i�݂��������ɐi�߂Ȃ��ꍇ�A�����_���ɕ��������߂Ȃ���
				if (changeGoalDirectionFlag)
				{
					// �����_���ɐi�݂����������C��
					characterNS::DIRECTION newDirection = (characterNS::DIRECTION) (rand() % 4);
					// ����ł��i�߂Ȃ��ꍇ�A�G��ҋ@��Ԃɂ��ĐÎ~������
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
				// �ŋߐڂ̃o���P�[�h��T��
				mEnemy[i]->searchNearBarricadeIndex();
				if (!mEnemy[i]->checkBarricadeOnLine())	// ��܂ł̒�����Ƀo���P�[�h���Ȃ��ꍇ�A��ւƐi�s����
				{
					// �G�̏�Ԃ���ړ����ւƕύX
					mEnemy[i]->setState(characterNS::MOVE);
					// �i�݂��������ɐi�߂Ȃ��ꍇ�A������i�݂��������������_���ɕϊ�
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
				else // ��܂ł̒�����Ƀo���P�[�h�����݂���ꍇ�A
				{
					// �G�̏�Ԃ��o���P�[�h�ɐi�s���ւƕύX
					mEnemy[i]->setState(characterNS::MOVE);
					// �m��50%��x�����ɐi�ނ��ǂ������Ɍ��肵�A���̂��Ƃ�y�����ɐi�ނ��ǂ���������
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
				// �i�݂��������ɐi�߂Ȃ��ꍇ�A�����_���ɕ��������߂Ȃ���
				if (changeGoalDirectionFlag)
				{
					// �����_���ɐi�݂����������C��
					characterNS::DIRECTION newDirection = (characterNS::DIRECTION) (rand() % 4);
					// ����ł��i�߂Ȃ��ꍇ�A�G��ҋ@��Ԃɂ��ĐÎ~������
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
		// �v���C���[�̍U���R���W�����Ƃ̏Փ˂̏ꍇ
		if (mBrave.getBraveAttackCollision().collidesWith(mBarricades[i], collisionVector))
		{
			// �o���P�[�h�Ƀ_���[�W��^����
			mBarricades[i].damage(BRAVE_ATTACK);
			// �����ꂩ�̃o���P�[�h�ɍU�������������̂Ń��[�v�𔲂�����ɍU���R���W�������Ȃ���
			braveAttackCollidesWithAnyEnemy = true;
		}
	}

	// �E�҂̍U���������ꂩ�̓G�ɓ��������ꍇ�A�U���R���W�������Ȃ���
	if (braveAttackCollidesWithAnyEnemy)
	{
		mBrave.getBraveAttackCollision().setVisible(false);
		mBrave.getBraveAttackCollision().setActive(false);
	}
}

//==========================================================
// �G�̐����`�F�b�N���A�}�b�v��ɓG�����Ȃ���ΐV���ɐ���
//==========================================================
void Stage::checkCurrentEnemyNum()
{
	// �G���G�����݂�����
	for (int i = 0; i < mEnemyNum; i++)
	{
		if (mEnemy[i]->getActive())
			return;
	}

	// ��2�g
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
// �w�肳�ꂽ�X�e�[�W�̓G�f�[�^��ǂݍ���
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
	//csv�t�@�C����1�s���ǂݍ���
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
// �w�肳�ꂽ�X�e�[�W�A�h�̓G�f�[�^��ǂݍ��ݓG������������
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