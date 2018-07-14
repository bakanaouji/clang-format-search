//==========================================================
/// @file
/// @brief    entity.h�̎���
/// @author   �������V
///
/// @attention  �G���e�B�e�B��\���N���X�ł��B
///				���̃N���X���p�������N���X�̗͑͂������A
///				�Փ˔�����s�����Ƃ��ł��܂�

//==========================================================

#include "entity.h"

//==========================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Entity::Entity() : Image()
{
    mRadius = 1.0;						// �~�̏Փ˂̑傫��
	// BOX�̏Փ˔���̎l�p�`
    mEdge.left = -1;
    mEdge.top = -1;
    mEdge.right = 1;
    mEdge.bottom = 1;

    mMass = 1.0;						// ����
    mVelocity.x = 0.0;					// x���x
    mVelocity.y = 0.0;					// y���x
    mDeltaV.x = 0.0;					// x�����x
    mDeltaV.y = 0.0;					// y�����x
    mActive = true;						// �G���e�B�e�B�̓A�N�e�B�u
    mRotatedBoxReady = false;			// ��]�̏Փ˔���̃t���O
    mCollisionType = entityNS::CIRCLE;	// �Փ˔���̓f�t�H���g�ł͉~
    mHealth = 100;						// �̗́iMAX100�j
    mGravity = entityNS::GRAVITY;		// �d�͒萔
	mIsDamaged = false;					// �_���[�W�͎󂯂Ă��Ȃ���Ԃ���X�^�[�g
	mDrawFlag = true;					// �`��t���O�I��
	mDamagePer = 1.0f;					// �_���[�W���󂯂銄��
	// �_���[�W���Ɏg�p����^�C�}�[�����Z�b�g
	mDamageTimer = 0.0f;
	mTotalDamageTime = 0.0f;
}

//==========================================================
// �p�����[�^������
//==========================================================
void Entity::reset()
{
	mVelocity.x = 0.0;					// x���x
	mVelocity.y = 0.0;					// y���x
	mDeltaV.x = 0.0;					// x�����x
	mDeltaV.y = 0.0;					// y�����x
	mRotatedBoxReady = false;			// ��]�̏Փ˔���̃t���O
	mActive = true;						// �G���e�B�e�B�̓A�N�e�B�u
	mHealth = 100;						// �̗́iMAX100�j
	mIsDamaged = false;					// �_���[�W�͎󂯂Ă��Ȃ���Ԃ���X�^�[�g
	mDrawFlag = true;					// �`��t���O�I��
	mDamagePer = 1.0f;					// �_���[�W���󂯂銄��
	// �_���[�W���Ɏg�p����^�C�}�[�����Z�b�g
	mDamageTimer = 0.0f;
	mTotalDamageTime = 0.0f;
	// �C���[�W�����Z�b�g
	Image::reset();
}

//=============================================================================
// Entity��������
// ���s�O�F*gamePtr = Game�I�u�W�F�N�g�ւ̃|�C���^
//		   width = Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
//		   height = Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
//         ncols = �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
//         *textureM = Texturemanager�I�u�W�F�N�g�ւ̃|�C���^
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    mInput = gamePtr->getInput();                // ���̓V�X�e��
    mAudio = gamePtr->getAudio();                // �I�[�f�B�I�V�X�e��
    return(Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}

//==========================================================
// �L�����N�^�[��`��
// �`��t���O���I���̂Ƃ��̂ݕ`��
//==========================================================
void Entity::draw(COLOR_ARGB color)
{
	// �`��t���O���I���̂Ƃ��̂ݕ`��
	if (mDrawFlag)
		Image::draw(color);	// �G���G��`��
}

//=============================================================================
// �G���e�B�e�B���A�N�e�B�u��
//=============================================================================
void Entity::activate()
{
    mActive = true;
}

//=============================================================================
// update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Entity::update(const float frameTime)
{
    mVelocity += mDeltaV;
    mDeltaV.x = 0;
    mDeltaV.y = 0;
    Image::update(frameTime);
    mRotatedBoxReady = false;    // rotateBox�̏Փ˔���̂���
}

//==========================================================
// �G���e�B�e�B���X�V 
// �A�j���[�V�����𖈎��ԍX�V�������Ȃ��ꍇ�͂��̃��\�b�h���g�p����B
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//==========================================================
void Entity::updateWithoutImage(float frameTime)
{
	mVelocity += mDeltaV;
	mDeltaV.x = 0;
	mDeltaV.y = 0;
	mRotatedBoxReady = false;	// rotateBox�̏Փ˔���̂���
}

//==========================================================
// �A�j���[�V�����̂ݍX�V���郁�\�b�h
// �A�j���[�V�����𖈎��ԍX�V�������Ȃ��ꍇ�͂��̃��\�b�h���g�p����B
//==========================================================
void Entity::updateOnlyImage(float frameTime)
{
	Image::update(frameTime);
}

//=============================================================================
// AI�i�l�H�m�\�j
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// �l�H�m�\�v�Z�����s�Aent�͑��ݍ�p�̂��߂ɓn�����
//=============================================================================
void Entity::ai(const float frameTime, const Entity &ent)
{}

//=============================================================================
// ���̃G���e�B�e�B�Ƃ�������̃G���e�B�e�B�̏Փ˂𔻒�
// �e�G���e�B�e�B�͕K��1��ނ̏Փ˂��g�p���܂��B
// �Փ˂̎�ނ𕡐��K�v�Ƃ��镡�G�Ȑ}�`�́A
// �e������ʌ̃G���e�B�e�B�Ƃ��Ĉ������Ƃɂ�菈���ł��܂��B
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// �Փ˂̎�ށFCIRCLE�ABOX�AROTATED_BOX
//			   �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//=============================================================================
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{ 
	// �ǂ��炩�̃G���e�B�e�B���A�N�e�B�u�łȂ��ꍇ�A�Փ˂͋N����Ȃ�
    if (!mActive || !ent.getActive())    
        return false;

	// �����̃G���e�B�e�B��CIRCLE�Փ˂ł���ꍇ
    if (mCollisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
        return collideCircle(ent, collisionVector);
	// �����̃G���e�B�e�B��BOX�Փ˂ł���ꍇ
    if (mCollisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
        return collideBox(ent, collisionVector);
	// ���̂��ׂĂ̑g�ݍ��킹���g�p���镪�����e�X�g
	// �ǂ���̃G���e�B�e�B��CIRCLE�Փ˂��g�p���Ȃ��ꍇ
    if (mCollisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
        return collideRotatedBox(ent, collisionVector);
    else    // �ǂ��炩1�̃G���e�B�e�B���~�̏ꍇ
		// ���̃G���e�B�e�B��CIRCLE�Փ˂��g�p����ꍇ
        if (mCollisionType == entityNS::CIRCLE)
        {
			// ���̃{�b�N�X����̏Փ˂��`�F�b�N
            bool collide = ent.collideRotatedBoxCircle(*this, collisionVector); 
			// �K�؂ȕ����ɏՓ˃x�N�g�����Z�b�g
            collisionVector *= -1;              // �Փ˃x�N�g���𔽓]
            return collide;
        }
        else    // ��������̃G���e�B�e�B��CIRCLE�Փ˂��g�p����ꍇ
            return collideRotatedBoxCircle(ent, collisionVector);
    return false;
}

//=============================================================================
// �~�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//=============================================================================
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{
	// ���S�ƒ��S�̊Ԃ̍�
    mDistSquared = *getCenter() - *ent.getCenter();
    mDistSquared.x = mDistSquared.x * mDistSquared.x;     // ����2��
    mDistSquared.y = mDistSquared.y * mDistSquared.y;

	// ���a�̍��v���v�Z�i�g��k���̔{���𒲐��j
    mSumRadiiSquared = (mRadius*getScale()) + (ent.mRadius*ent.getScale());
    mSumRadiiSquared *= mSumRadiiSquared;                 // 2�悷��

	// �G���e�B�e�B���Փ˂��Ă���ꍇ
    if(mDistSquared.x + mDistSquared.y <= mSumRadiiSquared)
    {
		// �Փ˃x�N�g����ݒ�
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;   // �Փ˂��Ă��Ȃ��ꍇ
}

//=============================================================================
// �����s���E�{�b�N�X�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//=============================================================================
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
	// �ǂ��炩�̃G���e�B�e�B���A�N�e�B�u�łȂ��ꍇ�A�Փ˂͓{��Ȃ�
    if (!mActive || !ent.getActive())
        return false;

	// �����s���E�{�b�N�X���g���ďՓ˂��`�F�b�N
    if( (getCenterX() + mEdge.right*getScale() >= ent.getCenterX() + ent.getEdge().left*ent.getScale()) && 
        (getCenterX() + mEdge.left*getScale() <= ent.getCenterX() + ent.getEdge().right*ent.getScale()) &&
        (getCenterY() + mEdge.bottom*getScale() >= ent.getCenterY() + ent.getEdge().top*ent.getScale()) && 
        (getCenterY() + mEdge.top*getScale() <= ent.getCenterY() + ent.getEdge().bottom*ent.getScale()) )
    {
		// �Փ˃x�N�g����ݒ�
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// ��]����{�b�N�X�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// �������e�X�g���g�p���ďՓ˂𔻒�
// �������e�X�g�F
// 2�̃{�b�N�X�́A����̒����ւ̓��e���d�Ȃ��Ă��Ȃ���ΏՓ˂��Ă��Ȃ�
//=============================================================================
bool Entity::collideRotatedBox(Entity &entB, VECTOR2 &collisionVector)
{
    float overlap01, overlap03;
    computeRotatedBox();                    // ��]����{�b�N�X������
    entB.computeRotatedBox();               // ��]����{�b�N�X������
    if (projectionsOverlap(entB, collisionVector) && entB.projectionsOverlap(*this, collisionVector))
    {
		// �����ɓ��B�����ꍇ�A�G���e�B�e�B�͏Փ˂��Ă���B
		// �ŏ��̏d�������̃G�b�W���Փ˂��������Ă���G�b�W�B
		// �Փ˃x�N�g���͏Փ˂����������G�b�W�ɐ����ɐ��������B
		// ���e�G�b�W��01��03�B
        //
        //                    entA01min
        //                   /     entB01min
        //                  /     / entA01max 
        //                 /     / /  entB01max
        //                /     / /  /
        //            0--------------------1
        // entB03min..|          ____  
        // entA03min..|    _____|_ B | 
        //            |   | A   | |  |
        // entA03max..|   |_____|_|  |
        // entB03max..|         |____|
        //            | 
        //            |   
        //            3
        //            

        if (mEntA01min < mEntB01min)   // A�̂ق���B��荶
        {
            overlap01 = mEntA01max - mEntB01min;
            collisionVector = mCorners[1] - mCorners[0];
        }
        else    // A�̂ق���B���E
        {
            overlap01 = mEntB01max - mEntA01min;
            collisionVector = mCorners[0] - mCorners[1];
        }
        if (mEntA03min < mEntB03min)   // A�̂ق���B����
        {
            overlap03 = mEntA03max - mEntB03min;
            if (overlap03 < overlap01)
                collisionVector = mCorners[3] - mCorners[0];
        }
        else    // A�̂ق���B��艺
        {
            overlap03 = mEntB03max - mEntA03min;
            if (overlap03 < overlap01)
                collisionVector = mCorners[0] - mCorners[3];
        }
        return true;
    }
    return false;
}

//=============================================================================
// ����̃{�b�N�X���A���̃G���e�B�e�B��edge01�����edge03�ɓ��e
// collideRotateBos()�ɂ���ČĂяo�����
// ���s��F���e���d�Ȃ��Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
//=============================================================================
bool Entity::projectionsOverlap(Entity &entB, VECTOR2 &collisionVector)
{
	float projection;

	// ����̃{�b�N�X��edge01�ɓ��e
	projection = mGraphics->Vector2Dot(&mEdge01, entB.getCorner(0));
	// ���_0�𓊉e
	mEntB01min = projection;
	mEntB01max = projection;
	// �c��̒��_���ꂼ�������
	for (int c = 1; c < 4; c++)
	{
		// ���_��edge01�ɓ��e
		projection = mGraphics->Vector2Dot(&mEdge01, entB.getCorner(c));
		if (projection < mEntB01min)
			mEntB01min = projection;
		else if (projection > mEntB01max)
			mEntB01max = projection;
	}
	// ���e���d�Ȃ�Ȃ��ꍇ
	if (mEntB01min > mEntA01max || mEntB01max < mEntA01min)
		return false;                       // �Փ˂̉\���Ȃ�
	// ����̃{�b�N�X��edge03�ɓ��e
	projection = mGraphics->Vector2Dot(&mEdge03, entB.getCorner(0));
	// ���_0�𓊉e
	mEntB03min = projection;
	mEntB03max = projection;
	// �c��̒��_���ꂼ�������
	for (int c = 1; c < 4; c++)
	{
		// ���_��edge03�ɓ��e
		projection = mGraphics->Vector2Dot(&mEdge03, entB.getCorner(c));
		if (projection < mEntB03min)
			mEntB03min = projection;
		else if (projection > mEntB03max)
			mEntB03max = projection;
	}
	// ���e���d�Ȃ�Ȃ��ꍇ
	if (mEntB03min > mEntA03max || mEntB03max < mEntA03min)
		return false;                       // �Փ˂̉\���Ȃ�
	return true;                            // ���e���d�Ȃ��Ă���ꍇ
}

//=============================================================================
// ��]����{�b�N�X�Ɖ~�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
// �{�b�N�X�̕ӂƉ~�̔��a�ŕ������e�X�g���g�p
// �~�̒��S���Փ˃{�b�N�X�̊e�ӂ�L�΂��������̊O��
// �i�{���m�C�̈�j�ɂ���ꍇ�A�{�b�N�X�̍ł��߂����_��
// �Փ˂��Ȃ����������̃`�F�b�N�Ŕ��肵�܂��B
// �ł��߂����_�́A�d�Ȃ�̃e�X�g���画�f�ł��܂��B
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//
//   Voronoi0 |   | Voronoi1
//         ---0---1---
//            |   |
//         ---3---2---
//   Voronoi3 |   | Voronoi2
//
// ���s�O�F���̃G���e�B�e�B�͕K���{�b�N�X�A����̃G���e�B�e�B�ient�j�͕K���~
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//=============================================================================
bool Entity::collideRotatedBoxCircle(Entity &entB, VECTOR2 &collisionVector)
{
    float center01, center03, overlap01, overlap03;

    computeRotatedBox();                    // ��]����{�b�N�X������

	// �~�̒��S��edge01�ɓ��e
    center01 = mGraphics->Vector2Dot(&mEdge01, entB.getCenter());
	// �ŏ��l�ƍő�l�͒��S����̔��a
    mEntB01min = center01 - entB.getRadius()*entB.getScale();
    mEntB01max = center01 + entB.getRadius()*entB.getScale();
    if (mEntB01min > mEntA01max || mEntB01max < mEntA01min) // ���e���d�Ȃ�Ȃ��ꍇ
        return false;                       // �Փ˂̉\���Ȃ�
        
	// �~�̒��S��edge03�ɓ��e
    center03 = mGraphics->Vector2Dot(&mEdge03, entB.getCenter());
	// �ŏ��l�ƍő�l�͒��S����̔��a
    mEntB03min = center03 - entB.getRadius()*entB.getScale();
    mEntB03max = center03 + entB.getRadius()*entB.getScale();
    if (mEntB03min > mEntA03max || mEntB03max < mEntA03min) // ���e���d�Ȃ�Ȃ��ꍇ
        return false;                       // �Փ˂̉\���Ȃ�

	// �~�̓��e���{�b�N�X�̓��e�ɏd�Ȃ�ꍇ
	// �~���Փ˃{�b�N�X�̃{���m�C�̈�ɂ��邩�ǂ������`�F�b�N

	// ���S��Voronoi0�ɂ���ꍇ
    if(center01 < mEntA01min && center03 < mEntA03min)
        return collideCornerCircle(mCorners[0], entB, collisionVector);
	// ���S��Voronoi1�ɂ���ꍇ
    if(center01 > mEntA01max && center03 < mEntA03min)
        return collideCornerCircle(mCorners[1], entB, collisionVector);
	// ���S��Voronoi2�ɂ���ꍇ
    if(center01 > mEntA01max && center03 > mEntA03max)
        return collideCornerCircle(mCorners[2], entB, collisionVector);
	// ���S��Voronoi3�ɂ���ꍇ
    if(center01 < mEntA01min && center03 > mEntA03max)
        return collideCornerCircle(mCorners[3], entB, collisionVector);

	// �~���Փ˃{�b�N�X�̃{���m�C�̈�ɂȂ��̂ŁA�{�b�N�X�̃G�b�W�ƏՓˁB
	// �ŏ��̏d�������̃G�b�W���Փ˂��������Ă���G�b�W�B
	// �Փ˃x�N�g���͏Փ˂����������G�b�W�ɐ����ɐ��������B
	// ���e�G�b�W��01��03�B
    //
    //                    entA01min
    //                   /   entB01min
    //                  /   /    entB01max 
    //                 /   /    /  entA01max
    //                /   /    /  /
    //            0--------------------1
    // entB03min..|        ___  
    // entA03min..|    ___/ B \__  
    // entB03max..|   |   \___/  |
    //            |   | A        |
    // entA03max..|   |__________|
    //            |         
    //            | 
    //            |   
    //            3
    //            
    if (mEntA01min < mEntB01min)   // A�̂ق���B��荶
    {
        overlap01 = mEntA01max - mEntB01min;
        collisionVector = mCorners[1] - mCorners[0];
    }
    else    // A�̂ق���B���E
    {
        overlap01 = mEntB01max - mEntA01min;
        collisionVector = mCorners[0] - mCorners[1];
    }
    if (mEntA03min < mEntB03min)   // A�̂ق���B����
    {
        overlap03 = mEntA03max - mEntB03min;
        if (overlap03 < overlap01)
            collisionVector = mCorners[3] - mCorners[0];
    }
    else    // A�̂ق���B��艺
    {
        overlap03 = mEntB03max - mEntA03min;
        if (overlap03 < overlap01)
            collisionVector = mCorners[0] - mCorners[3];
    }
    return true;
}

//=============================================================================
// �{�b�N�X�̒��_���~�ƏՓ˂��Ă��Ȃ������A�����̃`�F�b�N���g�p���Ĕ���
// collideRotateBosCircle()�ɂ���ČĂяo�����
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
//=============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
    mDistSquared = corner - *ent.getCenter();            // ���_ - �~
    mDistSquared.x = mDistSquared.x * mDistSquared.x;      // ����2��
    mDistSquared.y = mDistSquared.y * mDistSquared.y;

    // ���a�̍��v���v�Z���Ă���A�����2��
	mSumRadiiSquared = ent.getRadius()*ent.getScale();   // (0 + �~�̔��a)
    mSumRadiiSquared *= mSumRadiiSquared;                 // 2�悷��

	// ���_�Ɖ~���Փ˂��Ă���ꍇ
    if(mDistSquared.x + mDistSquared.y <= mSumRadiiSquared)
    {
		// �Փ˃x�N�g����ݒ�
        collisionVector = *ent.getCenter() - corner;
        return true;
    }
    return false;
}

//=============================================================================
// ��]����{�b�N�X�̒��_�A���e���A���e�̍ŏ��l�ƍő�l���v�Z
// 0---1  ���_�̔ԍ�
// |   |
// 3---2
//=============================================================================
void Entity::computeRotatedBox()
{
    if(mRotatedBoxReady)
        return;
    float projection;

    VECTOR2 rotatedX(cos(mSpriteData.angle), sin(mSpriteData.angle));
    VECTOR2 rotatedY(-sin(mSpriteData.angle), cos(mSpriteData.angle));

    const VECTOR2 *center = getCenter();
    mCorners[0] = *center + rotatedX * ((float)mEdge.left*getScale())  +
                           rotatedY * ((float)mEdge.top*getScale());
    mCorners[1] = *center + rotatedX * ((float)mEdge.right*getScale()) + 
                           rotatedY * ((float)mEdge.top*getScale());
    mCorners[2] = *center + rotatedX * ((float)mEdge.right*getScale()) + 
                           rotatedY * ((float)mEdge.bottom*getScale());
    mCorners[3] = *center + rotatedX * ((float)mEdge.left*getScale())  +
                           rotatedY * ((float)mEdge.bottom*getScale());

    // corners[0]����_�Ƃ��Ďg�p
	// corners[0]�ɐڂ���2�ӂ𓊉e���Ƃ��Ďg�p
    mEdge01 = VECTOR2(mCorners[1].x - mCorners[0].x, mCorners[1].y - mCorners[0].y);
    mGraphics->Vector2Normalize(&mEdge01);
    mEdge03 = VECTOR2(mCorners[3].x - mCorners[0].x, mCorners[3].y - mCorners[0].y);
    mGraphics->Vector2Normalize(&mEdge03);

	// ���̃G���e�B�e�B�𓊉e����ɓ��e�����Ƃ��̍ŏ��l�ƍő�l
    projection = mGraphics->Vector2Dot(&mEdge01, &mCorners[0]);
    mEntA01min = projection;
    mEntA01max = projection;
	// edge01�ւ̓��e
    projection = mGraphics->Vector2Dot(&mEdge01, &mCorners[1]);
    if (projection < mEntA01min)
        mEntA01min = projection;
    else if (projection > mEntA01max)
        mEntA01max = projection;
	// edge03�ւ̓��e
    projection = mGraphics->Vector2Dot(&mEdge03, &mCorners[0]);
    mEntA03min = projection;
    mEntA03max = projection;
    projection = mGraphics->Vector2Dot(&mEdge03, &mCorners[3]);
    if (projection < mEntA03min)
        mEntA03min = projection;
    else if (projection > mEntA03max)
        mEntA03max = projection;

    mRotatedBoxReady = true;
}

//=============================================================================
// ���̃G���e�B�e�B���w�肳�ꂽ��`�̊O���ɂ��邩
// ���s��F��`�̊O���ɂ���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
//=============================================================================
bool Entity::outsideRect(RECT rect)
{
    if( mSpriteData.x + mSpriteData.width*getScale() < rect.left || 
        mSpriteData.x > rect.right ||
        mSpriteData.y + mSpriteData.height*getScale() < rect.top || 
        mSpriteData.y > rect.bottom)
        return true;
    return false;
}

//=============================================================================
// damage
// ���̃G���e�B�e�B���A����ɂ���ă_���[�W���󂯂�
// �p�����鑤�̃N���X�ł��̊֐����I�[�o�[���C�h
//=============================================================================
void Entity::damage(const int weapon)
{}

//=============================================================================
// ���̃G���e�B�e�B�Ƃ̏Փˌ�̒��˕Ԃ�
// �K�v�ɉ����Ďg�p����
//=============================================================================
void Entity::bounce(VECTOR2 &collisionVector, Entity &ent)
{
    float cUVdotVdiff;
    VECTOR2 Vdiff = ent.getVelocity() - mVelocity;	// ���x�x�N�g���̍��x�N�g��
    VECTOR2 cUV = collisionVector;					// �Փ˒P�ʃx�N�g��
    Graphics::Vector2Normalize(&cUV);
    if(mCollisionType == entityNS::ROTATED_BOX)		// ���̃G���e�B�e�B��ROTATED_BOX�Փ˂̏ꍇ
        cUVdotVdiff = 1.0f;							// ���x�͕ω������Ȃ�
    else
        cUVdotVdiff = Graphics::Vector2Dot(&cUV, &Vdiff);   // ���x�̍���p����
    float massRatio = 2.0f;
    if (getMass() != 0)
        massRatio *= (ent.getMass() / (getMass() + ent.getMass()));
    if(massRatio < 0.1f)
        massRatio = 0.1f;

	// �G���e�B�e�B��collisionVector�ɉ����ė��������Ɉړ�
    VECTOR2 cv;
    int count=10;   // �ő僋�[�v��
    do
    {
        setX(getX() - cUV.x);
        setY(getY() - cUV.y);
        mRotatedBoxReady = false;
        count--;
    } while( this->collidesWith(ent, cv) && count);

    // ���˕Ԃ�
    mDeltaV += ((massRatio * cUVdotVdiff) * cUV);
}

//=============================================================================
// ����̃G���e�B�e�B���炱�̃G���e�B�e�B�ւ̏d��
// �d�͂����̃G���e�B�e�B�̑��x�x�N�g���ɉ��Z
// force = GRAVITY * m1 * m2 / r*r
//                    2              2
//  r*r  =   (Ax - Bx)   +  (Ay - By)
//=============================================================================
void Entity::gravityForce(Entity *ent, float frameTime)
{
	// �ǂ��炩�̃G���e�B�e�B���A�N�e�B�u�łȂ��ꍇ�A�d�͂̉e���͂Ȃ�
    if (!mActive || !ent->getActive())
        return ;

    mRr = pow((ent->getCenterX() - getCenterX()),2) + 
            pow((ent->getCenterY() - getCenterY()),2);
    mForce = mGravity * ent->getMass() * mMass/mRr;

    // --- �x�N�g���v�Z���g���ďd�̓x�N�g�����쐬 ---
    // �G���e�B�e�B�Ԃ̃x�N�g�����쐬
    VECTOR2 gravityV(ent->getCenterX() - getCenterX(),
                        ent->getCenterY() - getCenterY());
    // �x�N�g���𐳋K��
    Graphics::Vector2Normalize(&gravityV);
	// �d�͂ŏ�Z���āA�d�̓x�N�g�����쐬
    gravityV *= mForce * frameTime;
	// �d�̓x�N�g�����A�ړ����̑��x�x�N�g���ɉ��Z���āA������ς���
    mVelocity += gravityV;
}

//==========================================================
// �^�C����ł̈ʒu������������֐�
//==========================================================
void Entity::initTileXY()
{
	mTileX = (int)mSpriteData.x / 32;
	mTileY = (int)mSpriteData.y / 32;
}