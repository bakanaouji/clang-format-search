//==========================================================
/// @file
/// @brief    entity.hの実装
/// @author   阿部拳之
///
/// @attention  エンティティを表すクラスです。
///				このクラスを継承したクラスは体力を持ち、
///				衝突判定を行うことができます

//==========================================================

#include "entity.h"

//==========================================================

//=============================================================================
// コンストラクタ
//=============================================================================
Entity::Entity() : Image()
{
    mRadius = 1.0;						// 円の衝突の大きさ
	// BOXの衝突判定の四角形
    mEdge.left = -1;
    mEdge.top = -1;
    mEdge.right = 1;
    mEdge.bottom = 1;

    mMass = 1.0;						// 質量
    mVelocity.x = 0.0;					// x速度
    mVelocity.y = 0.0;					// y速度
    mDeltaV.x = 0.0;					// x加速度
    mDeltaV.y = 0.0;					// y加速度
    mActive = true;						// エンティティはアクティブ
    mRotatedBoxReady = false;			// 回転の衝突判定のフラグ
    mCollisionType = entityNS::CIRCLE;	// 衝突判定はデフォルトでは円
    mHealth = 100;						// 体力（MAX100）
    mGravity = entityNS::GRAVITY;		// 重力定数
	mIsDamaged = false;					// ダメージは受けていない状態からスタート
	mDrawFlag = true;					// 描画フラグオン
	mDamagePer = 1.0f;					// ダメージを受ける割合
	// ダメージ時に使用するタイマーをリセット
	mDamageTimer = 0.0f;
	mTotalDamageTime = 0.0f;
}

//==========================================================
// パラメータ初期化
//==========================================================
void Entity::reset()
{
	mVelocity.x = 0.0;					// x速度
	mVelocity.y = 0.0;					// y速度
	mDeltaV.x = 0.0;					// x加速度
	mDeltaV.y = 0.0;					// y加速度
	mRotatedBoxReady = false;			// 回転の衝突判定のフラグ
	mActive = true;						// エンティティはアクティブ
	mHealth = 100;						// 体力（MAX100）
	mIsDamaged = false;					// ダメージは受けていない状態からスタート
	mDrawFlag = true;					// 描画フラグオン
	mDamagePer = 1.0f;					// ダメージを受ける割合
	// ダメージ時に使用するタイマーをリセット
	mDamageTimer = 0.0f;
	mTotalDamageTime = 0.0f;
	// イメージをリセット
	Image::reset();
}

//=============================================================================
// Entityを初期化
// 実行前：*gamePtr = Gameオブジェクトへのポインタ
//		   width = Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
//		   height = Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
//         ncols = テクスチャ内の列数（1からnまで）（0は1と同じ）
//         *textureM = Texturemanagerオブジェクトへのポインタ
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    mInput = gamePtr->getInput();                // 入力システム
    mAudio = gamePtr->getAudio();                // オーディオシステム
    return(Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}

//==========================================================
// キャラクターを描画
// 描画フラグがオンのときのみ描画
//==========================================================
void Entity::draw(COLOR_ARGB color)
{
	// 描画フラグがオンのときのみ描画
	if (mDrawFlag)
		Image::draw(color);	// 雑魚敵を描画
}

//=============================================================================
// エンティティをアクティブ化
//=============================================================================
void Entity::activate()
{
    mActive = true;
}

//=============================================================================
// update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Entity::update(const float frameTime)
{
    mVelocity += mDeltaV;
    mDeltaV.x = 0;
    mDeltaV.y = 0;
    Image::update(frameTime);
    mRotatedBoxReady = false;    // rotateBoxの衝突判定のため
}

//==========================================================
// エンティティを更新 
// アニメーションを毎時間更新したくない場合はこのメソッドを使用する。
// frameTimeは、移動とアニメーションの速さを制御するために使用
//==========================================================
void Entity::updateWithoutImage(float frameTime)
{
	mVelocity += mDeltaV;
	mDeltaV.x = 0;
	mDeltaV.y = 0;
	mRotatedBoxReady = false;	// rotateBoxの衝突判定のため
}

//==========================================================
// アニメーションのみ更新するメソッド
// アニメーションを毎時間更新したくない場合はこのメソッドを使用する。
//==========================================================
void Entity::updateOnlyImage(float frameTime)
{
	Image::update(frameTime);
}

//=============================================================================
// AI（人工知能）
// 通常、フレームごとに1回呼び出す
// 人工知能計算を実行、entは相互作用のために渡される
//=============================================================================
void Entity::ai(const float frameTime, const Entity &ent)
{}

//=============================================================================
// このエンティティともう一方のエンティティの衝突を判定
// 各エンティティは必ず1種類の衝突を使用します。
// 衝突の種類を複数必要とする複雑な図形は、
// 各部分を別個のエンティティとして扱うことにより処理できます。
// 通常、フレームごとに1回呼び出す
// 衝突の種類：CIRCLE、BOX、ROTATED_BOX
//			   衝突している場合は、collisionVectorを設定
//=============================================================================
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{ 
	// どちらかのエンティティがアクティブでない場合、衝突は起こらない
    if (!mActive || !ent.getActive())    
        return false;

	// 両方のエンティティがCIRCLE衝突である場合
    if (mCollisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
        return collideCircle(ent, collisionVector);
	// 両方のエンティティがBOX衝突である場合
    if (mCollisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
        return collideBox(ent, collisionVector);
	// 他のすべての組み合わせが使用する分離軸テスト
	// どちらのエンティティもCIRCLE衝突を使用しない場合
    if (mCollisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
        return collideRotatedBox(ent, collisionVector);
    else    // どちらか1つのエンティティが円の場合
		// このエンティティがCIRCLE衝突を使用する場合
        if (mCollisionType == entityNS::CIRCLE)
        {
			// 他のボックスからの衝突をチェック
            bool collide = ent.collideRotatedBoxCircle(*this, collisionVector); 
			// 適切な方向に衝突ベクトルをセット
            collisionVector *= -1;              // 衝突ベクトルを反転
            return collide;
        }
        else    // もう一方のエンティティがCIRCLE衝突を使用する場合
            return collideRotatedBoxCircle(ent, collisionVector);
    return false;
}

//=============================================================================
// 円の衝突判定メソッド
// collision()によって呼び出される、デフォルトの衝突判定メソッド
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
//=============================================================================
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{
	// 中心と中心の間の差
    mDistSquared = *getCenter() - *ent.getCenter();
    mDistSquared.x = mDistSquared.x * mDistSquared.x;     // 差を2乗
    mDistSquared.y = mDistSquared.y * mDistSquared.y;

	// 半径の合計を計算（拡大縮小の倍率を調整）
    mSumRadiiSquared = (mRadius*getScale()) + (ent.mRadius*ent.getScale());
    mSumRadiiSquared *= mSumRadiiSquared;                 // 2乗する

	// エンティティが衝突している場合
    if(mDistSquared.x + mDistSquared.y <= mSumRadiiSquared)
    {
		// 衝突ベクトルを設定
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;   // 衝突していない場合
}

//=============================================================================
// 軸平行境界ボックスの衝突判定メソッド
// collision()によって呼び出される、デフォルトの衝突判定メソッド
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
//=============================================================================
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
	// どちらかのエンティティがアクティブでない場合、衝突は怒らない
    if (!mActive || !ent.getActive())
        return false;

	// 軸平行境界ボックスを使って衝突をチェック
    if( (getCenterX() + mEdge.right*getScale() >= ent.getCenterX() + ent.getEdge().left*ent.getScale()) && 
        (getCenterX() + mEdge.left*getScale() <= ent.getCenterX() + ent.getEdge().right*ent.getScale()) &&
        (getCenterY() + mEdge.bottom*getScale() >= ent.getCenterY() + ent.getEdge().top*ent.getScale()) && 
        (getCenterY() + mEdge.top*getScale() <= ent.getCenterY() + ent.getEdge().bottom*ent.getScale()) )
    {
		// 衝突ベクトルを設定
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// 回転するボックスの衝突判定メソッド
// collision()によって呼び出される、デフォルトの衝突判定メソッド
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
// 分離軸テストを使用して衝突を判定
// 分離軸テスト：
// 2つのボックスは、特定の直線への投影が重なっていなければ衝突していない
//=============================================================================
bool Entity::collideRotatedBox(Entity &entB, VECTOR2 &collisionVector)
{
    float overlap01, overlap03;
    computeRotatedBox();                    // 回転するボックスを準備
    entB.computeRotatedBox();               // 回転するボックスを準備
    if (projectionsOverlap(entB, collisionVector) && entB.projectionsOverlap(*this, collisionVector))
    {
		// ここに到達した場合、エンティティは衝突している。
		// 最小の重複部分のエッジが衝突が発生しているエッジ。
		// 衝突ベクトルは衝突が発生したエッジに垂直に生成される。
		// 投影エッジは01と03。
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

        if (mEntA01min < mEntB01min)   // AのほうがBより左
        {
            overlap01 = mEntA01max - mEntB01min;
            collisionVector = mCorners[1] - mCorners[0];
        }
        else    // AのほうがBより右
        {
            overlap01 = mEntB01max - mEntA01min;
            collisionVector = mCorners[0] - mCorners[1];
        }
        if (mEntA03min < mEntB03min)   // AのほうがBより上
        {
            overlap03 = mEntA03max - mEntB03min;
            if (overlap03 < overlap01)
                collisionVector = mCorners[3] - mCorners[0];
        }
        else    // AのほうがBより下
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
// 相手のボックスを、このエンティティのedge01およびedge03に投影
// collideRotateBos()によって呼び出される
// 実行後：投影が重なっている場合はtrue、それ以外の場合はfalseを戻す
//=============================================================================
bool Entity::projectionsOverlap(Entity &entB, VECTOR2 &collisionVector)
{
	float projection;

	// 相手のボックスをedge01に投影
	projection = mGraphics->Vector2Dot(&mEdge01, entB.getCorner(0));
	// 頂点0を投影
	mEntB01min = projection;
	mEntB01max = projection;
	// 残りの頂点それぞれを処理
	for (int c = 1; c < 4; c++)
	{
		// 頂点をedge01に投影
		projection = mGraphics->Vector2Dot(&mEdge01, entB.getCorner(c));
		if (projection < mEntB01min)
			mEntB01min = projection;
		else if (projection > mEntB01max)
			mEntB01max = projection;
	}
	// 投影が重ならない場合
	if (mEntB01min > mEntA01max || mEntB01max < mEntA01min)
		return false;                       // 衝突の可能性なし
	// 相手のボックスをedge03に投影
	projection = mGraphics->Vector2Dot(&mEdge03, entB.getCorner(0));
	// 頂点0を投影
	mEntB03min = projection;
	mEntB03max = projection;
	// 残りの頂点それぞれを処理
	for (int c = 1; c < 4; c++)
	{
		// 頂点をedge03に投影
		projection = mGraphics->Vector2Dot(&mEdge03, entB.getCorner(c));
		if (projection < mEntB03min)
			mEntB03min = projection;
		else if (projection > mEntB03max)
			mEntB03max = projection;
	}
	// 投影が重ならない場合
	if (mEntB03min > mEntA03max || mEntB03max < mEntA03min)
		return false;                       // 衝突の可能性なし
	return true;                            // 投影が重なっている場合
}

//=============================================================================
// 回転するボックスと円の衝突判定メソッド
// collision()によって呼び出される、デフォルトの衝突判定メソッド
// ボックスの辺と円の半径で分離軸テストを使用
// 円の中心が衝突ボックスの各辺を伸ばした直線の外側
// （ボロノイ領域）にある場合、ボックスの最も近い頂点と
// 衝突がないかを距離のチェックで判定します。
// 最も近い頂点は、重なりのテストから判断できます。
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
//
//   Voronoi0 |   | Voronoi1
//         ---0---1---
//            |   |
//         ---3---2---
//   Voronoi3 |   | Voronoi2
//
// 実行前：このエンティティは必ずボックス、相手のエンティティ（ent）は必ず円
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
//=============================================================================
bool Entity::collideRotatedBoxCircle(Entity &entB, VECTOR2 &collisionVector)
{
    float center01, center03, overlap01, overlap03;

    computeRotatedBox();                    // 回転するボックスを準備

	// 円の中心をedge01に投影
    center01 = mGraphics->Vector2Dot(&mEdge01, entB.getCenter());
	// 最小値と最大値は中心からの半径
    mEntB01min = center01 - entB.getRadius()*entB.getScale();
    mEntB01max = center01 + entB.getRadius()*entB.getScale();
    if (mEntB01min > mEntA01max || mEntB01max < mEntA01min) // 投影が重ならない場合
        return false;                       // 衝突の可能性なし
        
	// 円の中心をedge03に投影
    center03 = mGraphics->Vector2Dot(&mEdge03, entB.getCenter());
	// 最小値と最大値は中心からの半径
    mEntB03min = center03 - entB.getRadius()*entB.getScale();
    mEntB03max = center03 + entB.getRadius()*entB.getScale();
    if (mEntB03min > mEntA03max || mEntB03max < mEntA03min) // 投影が重ならない場合
        return false;                       // 衝突の可能性なし

	// 円の投影がボックスの投影に重なる場合
	// 円が衝突ボックスのボロノイ領域にあるかどうかをチェック

	// 中心がVoronoi0にある場合
    if(center01 < mEntA01min && center03 < mEntA03min)
        return collideCornerCircle(mCorners[0], entB, collisionVector);
	// 中心がVoronoi1にある場合
    if(center01 > mEntA01max && center03 < mEntA03min)
        return collideCornerCircle(mCorners[1], entB, collisionVector);
	// 中心がVoronoi2にある場合
    if(center01 > mEntA01max && center03 > mEntA03max)
        return collideCornerCircle(mCorners[2], entB, collisionVector);
	// 中心がVoronoi3にある場合
    if(center01 < mEntA01min && center03 > mEntA03max)
        return collideCornerCircle(mCorners[3], entB, collisionVector);

	// 円が衝突ボックスのボロノイ領域にないので、ボックスのエッジと衝突。
	// 最小の重複部分のエッジが衝突が発生しているエッジ。
	// 衝突ベクトルは衝突が発生したエッジに垂直に生成される。
	// 投影エッジは01と03。
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
    if (mEntA01min < mEntB01min)   // AのほうがBより左
    {
        overlap01 = mEntA01max - mEntB01min;
        collisionVector = mCorners[1] - mCorners[0];
    }
    else    // AのほうがBより右
    {
        overlap01 = mEntB01max - mEntA01min;
        collisionVector = mCorners[0] - mCorners[1];
    }
    if (mEntA03min < mEntB03min)   // AのほうがBより上
    {
        overlap03 = mEntA03max - mEntB03min;
        if (overlap03 < overlap01)
            collisionVector = mCorners[3] - mCorners[0];
    }
    else    // AのほうがBより下
    {
        overlap03 = mEntB03max - mEntA03min;
        if (overlap03 < overlap01)
            collisionVector = mCorners[0] - mCorners[3];
    }
    return true;
}

//=============================================================================
// ボックスの頂点が円と衝突していないかを、距離のチェックを使用して判定
// collideRotateBosCircle()によって呼び出される
// 実行後：衝突している場合はtrue、していない場合はfalseを戻す
// 衝突している場合は、collisionVectorを設定
//=============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
    mDistSquared = corner - *ent.getCenter();            // 頂点 - 円
    mDistSquared.x = mDistSquared.x * mDistSquared.x;      // 差を2乗
    mDistSquared.y = mDistSquared.y * mDistSquared.y;

    // 半径の合計を計算してから、それを2乗
	mSumRadiiSquared = ent.getRadius()*ent.getScale();   // (0 + 円の半径)
    mSumRadiiSquared *= mSumRadiiSquared;                 // 2乗する

	// 頂点と円が衝突している場合
    if(mDistSquared.x + mDistSquared.y <= mSumRadiiSquared)
    {
		// 衝突ベクトルを設定
        collisionVector = *ent.getCenter() - corner;
        return true;
    }
    return false;
}

//=============================================================================
// 回転するボックスの頂点、投影線、投影の最小値と最大値を計算
// 0---1  頂点の番号
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

    // corners[0]を基点として使用
	// corners[0]に接する2辺を投影線として使用
    mEdge01 = VECTOR2(mCorners[1].x - mCorners[0].x, mCorners[1].y - mCorners[0].y);
    mGraphics->Vector2Normalize(&mEdge01);
    mEdge03 = VECTOR2(mCorners[3].x - mCorners[0].x, mCorners[3].y - mCorners[0].y);
    mGraphics->Vector2Normalize(&mEdge03);

	// このエンティティを投影線上に投影したときの最小値と最大値
    projection = mGraphics->Vector2Dot(&mEdge01, &mCorners[0]);
    mEntA01min = projection;
    mEntA01max = projection;
	// edge01への投影
    projection = mGraphics->Vector2Dot(&mEdge01, &mCorners[1]);
    if (projection < mEntA01min)
        mEntA01min = projection;
    else if (projection > mEntA01max)
        mEntA01max = projection;
	// edge03への投影
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
// このエンティティが指定された矩形の外側にあるか
// 実行後：矩形の外側にある場合はtrue、それ以外の場合はfalseを戻す
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
// このエンティティが、武器によってダメージを受ける
// 継承する側のクラスでこの関数をオーバーライド
//=============================================================================
void Entity::damage(const int weapon)
{}

//=============================================================================
// 他のエンティティとの衝突後の跳ね返り
// 必要に応じて使用する
//=============================================================================
void Entity::bounce(VECTOR2 &collisionVector, Entity &ent)
{
    float cUVdotVdiff;
    VECTOR2 Vdiff = ent.getVelocity() - mVelocity;	// 速度ベクトルの差ベクトル
    VECTOR2 cUV = collisionVector;					// 衝突単位ベクトル
    Graphics::Vector2Normalize(&cUV);
    if(mCollisionType == entityNS::ROTATED_BOX)		// このエンティティがROTATED_BOX衝突の場合
        cUVdotVdiff = 1.0f;							// 速度は変化させない
    else
        cUVdotVdiff = Graphics::Vector2Dot(&cUV, &Vdiff);   // 速度の差を用いる
    float massRatio = 2.0f;
    if (getMass() != 0)
        massRatio *= (ent.getMass() / (getMass() + ent.getMass()));
    if(massRatio < 0.1f)
        massRatio = 0.1f;

	// エンティティをcollisionVectorに沿って離れる方向に移動
    VECTOR2 cv;
    int count=10;   // 最大ループ回数
    do
    {
        setX(getX() - cUV.x);
        setY(getY() - cUV.y);
        mRotatedBoxReady = false;
        count--;
    } while( this->collidesWith(ent, cv) && count);

    // 跳ね返り
    mDeltaV += ((massRatio * cUVdotVdiff) * cUV);
}

//=============================================================================
// 相手のエンティティからこのエンティティへの重力
// 重力をこのエンティティの速度ベクトルに加算
// force = GRAVITY * m1 * m2 / r*r
//                    2              2
//  r*r  =   (Ax - Bx)   +  (Ay - By)
//=============================================================================
void Entity::gravityForce(Entity *ent, float frameTime)
{
	// どちらかのエンティティがアクティブでない場合、重力の影響はない
    if (!mActive || !ent->getActive())
        return ;

    mRr = pow((ent->getCenterX() - getCenterX()),2) + 
            pow((ent->getCenterY() - getCenterY()),2);
    mForce = mGravity * ent->getMass() * mMass/mRr;

    // --- ベクトル計算を使って重力ベクトルを作成 ---
    // エンティティ間のベクトルを作成
    VECTOR2 gravityV(ent->getCenterX() - getCenterX(),
                        ent->getCenterY() - getCenterY());
    // ベクトルを正規化
    Graphics::Vector2Normalize(&gravityV);
	// 重力で乗算して、重力ベクトルを作成
    gravityV *= mForce * frameTime;
	// 重力ベクトルを、移動中の速度ベクトルに加算して、方向を変える
    mVelocity += gravityV;
}

//==========================================================
// タイル上での位置を初期化する関数
//==========================================================
void Entity::initTileXY()
{
	mTileX = (int)mSpriteData.x / 32;
	mTileY = (int)mSpriteData.y / 32;
}