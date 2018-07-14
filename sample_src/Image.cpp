//==========================================================
/// @file
/// @brief    image.hの実装
/// @author   阿部拳之
///
/// @attention  画像を描画するクラスです。
///				ゲーム内のアイテムはすべて画像があると考えられるので、
///				このクラスを継承することを勧めます

//==========================================================
#include "image.h"
//==========================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Image::Image()
{
	mInitialized = false;            // 初期化に成功した場合、trueとなる
	mSpriteData.width = 2;
	mSpriteData.height = 2;
	mSpriteData.x = 0.0;
	mSpriteData.y = 0.0;
	mSpriteData.scale = 1.0;
	mSpriteData.angle = 0.0;
	mSpriteData.rect.left = 0;       // 複数のフレームを持つ画像から１つのフレームを選択するために使用
	mSpriteData.rect.top = 0;
	mSpriteData.rect.right = mSpriteData.width;
	mSpriteData.rect.bottom = mSpriteData.height;
	mSpriteData.texture = NULL;      // スプライトテクスチャ（画像）
	mSpriteData.flipHorizontal = false;
	mSpriteData.flipVertical = false;
	mCols = 1;
	mTextureManager = NULL;
	mStartFrame = 0;
	mEndFrame = 0;
	mCurrentFrame = 0;
	mFrameDelay = 1.0;					// デフォルトでは1秒に1フレーム分アニメーションが進む
	mAnimTimer = 0.0;
	mVisible = true;					// 画像は表示状態がデフォルト
	mLoop = true;						// アニメーションをループさせるのがデフォルト
	mAnimComplete = false;
	mGraphics = NULL;					// グラフィックシステムへのリンク
	mColorFilter = graphicsNS::WHITE;	// カラーフィルターなしの場合、WHITE
	mMode = imageNS::MODE::HORIZONTAL;	// 通常は水平でアニメーションが進む
}

//==========================================================
// パラメータ初期化
//==========================================================
void Image::reset()
{
	mVisible = true;
	setCurrentFrame(mStartFrame);
	mMode = imageNS::HORIZONTAL;
	// アニメーションはループ状態からスタート
	mLoop = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
Image::~Image()
{}

//=============================================================================
// Imageを初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// Graphicsへのポインタ
// Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
// Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
// テクスチャ内の列数（1からnまで）（0は1と同じ）
// TextureManagerへのポインタ
//=============================================================================
bool Image::initialize(Graphics *g, int width, int height, int ncols,
	TextureManager *textureM)
{
	try {
		mGraphics = g;				// graphicsオブジェクト
		mTextureManager = textureM;	// テクスチャオブジェクトへのポインタ
		mSpriteData.texture = mTextureManager->getTexture();
		if (width == 0)
			width = mTextureManager->getWidth();	// 全体の幅を使用
		mSpriteData.width = width;
		if (height == 0)
			height = mTextureManager->getHeight();	// 全体の高さを使用
		mSpriteData.height = height;
		mCols = ncols;
		if (mCols == 0)
			mCols = 1;								// colsが0の場合は、1を使用
													// currentFrameを描画するspriteData.rectを構成
		mSpriteData.rect.left = (mCurrentFrame % mCols) * mSpriteData.width;
		// 右端+1
		mSpriteData.rect.right = mSpriteData.rect.left + mSpriteData.width;
		mSpriteData.rect.top = (mCurrentFrame / mCols) * mSpriteData.height;
		// 下端+1
		mSpriteData.rect.bottom = mSpriteData.rect.top + mSpriteData.height;
	}
	catch (...)
	{
		return false;
	}
	mInitialized = true;	// 正常に初期化された場合
	return true;
}

//=============================================================================
// colorをフィルタとして使って画像を描画
// colorパラメータはオプション
// （image.hで、デフォルトとして白が割り当てられている）
// 実行前：spriteBegin()を呼び出す
// 実行後：spriteEnd()を呼び出す
//=============================================================================
void Image::draw(const COLOR_ARGB color)
{
	if (!mVisible || mGraphics == NULL)
		return;
	// onReset()が呼び出されたときに新しいテクスチャを取得
	mSpriteData.texture = mTextureManager->getTexture();
	if (color == graphicsNS::FILTER)	// フィルタを使って描画する場合
										// colorFilterを使用
		mGraphics->drawSprite(mSpriteData, mColorFilter);
	else
		// フィルタとしてcolorを使用
		mGraphics->drawSprite(mSpriteData, color);
}

//=============================================================================
// 指定されたSpriteDataを使ってこの画像を描画
// 現在のSpriteData.rectを使って、テクスチャを選択
// 実行前：spriteBegin()を呼び出す
// 実行後：spriteEnd()を呼び出す
//=============================================================================
void Image::draw(SpriteData sd, const COLOR_ARGB color, const UINT textureN)
{
	if (!mVisible || mGraphics == NULL)
		return;
	// この画像の矩形を使ってテクスチャを選択
	sd.rect = mSpriteData.rect;
	// onReset()が呼び出されたときに新しいテクスチャを取得
	sd.texture = mTextureManager->getTexture(textureN);
	if (color == graphicsNS::FILTER)			// フィルタを使って描画する場合
		mGraphics->drawSprite(sd, mColorFilter);// colorFilterを使用
	else
		mGraphics->drawSprite(sd, color);
}

//=============================================================================
// update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Image::update(const float frameTime)
{
	switch (mMode)
	{
	case imageNS::HORIZONTAL:					// 読み込んだ画像ファイルに対して水平方向にアニメーションが進む場合、
		if (mEndFrame - mStartFrame > 0)        // アニメーション化するスプライトの場合
		{
			mAnimTimer += frameTime;            // 合計の経過時間
			if (mAnimTimer > mFrameDelay)
			{
				mAnimTimer -= mFrameDelay;
				mCurrentFrame++;
				if (mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame)
				{
					if (mLoop == true)          // ループするアニメーションの場合
						mCurrentFrame = mStartFrame;
					else                        // ループしないアニメーションの場合
					{
						mCurrentFrame = mEndFrame;
						mAnimComplete = true;   // アニメーションの完了
					}
				}
				setRect();                      // spriteData.rectを設定
			}
		}
		break;
	case imageNS::VERTICAL:						// 読み込んだ画像ファイルに対して水平方向にアニメーションが進む場合、
		if (mEndFrame - mStartFrame > 0)        // アニメーション化するスプライトの場合
		{
			mAnimTimer += frameTime;            // 合計の経過時間
			if (mAnimTimer > mFrameDelay)
			{
				mAnimTimer -= mFrameDelay;
				mCurrentFrame += mCols;
				if (mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame)
				{
					if (mLoop == true)          // ループするアニメーションの場合
						mCurrentFrame = mStartFrame;
					else                        // ループしないアニメーションの場合
					{
						mCurrentFrame = mEndFrame;
						mAnimComplete = true;   // アニメーションの完了
					}
				}
				setRect();                      // spriteData.rectを設定
			}
		}
		break;
	}
}

//=============================================================================
// 画像の現在のフレームを設定
//=============================================================================
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		mAnimTimer = 0.0f;
		mCurrentFrame = c;
		mAnimComplete = false;
		setRect();                          // spriteData.rectを設定
	}
}

//=============================================================================
//  spriteData.rectを現在のフレームを描画するために設定
//=============================================================================
inline void Image::setRect()
{
	// spriteData.rectを現在のフレームを描画するために再構成
	// configure spriteData.rect to draw currentFrame
	mSpriteData.rect.left = (mCurrentFrame % mCols) * mSpriteData.width;
	// right edge + 1
	mSpriteData.rect.right = mSpriteData.rect.left + mSpriteData.width;
	mSpriteData.rect.top = (mCurrentFrame / mCols) * mSpriteData.height;
	// bottom edge + 1
	mSpriteData.rect.bottom = mSpriteData.rect.top + mSpriteData.height;
}