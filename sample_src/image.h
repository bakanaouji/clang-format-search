//==========================================================
/// @file
/// @brief    Imageクラス
/// @author   阿部拳之
///
/// @attention  画像を描画するクラスです。
///				ゲーム内のアイテムはすべて画像があると考えられるので、
///				このクラスを継承することを勧めます

//==========================================================
#ifndef _IMAGE_H	//このファイルが複数の箇所でインクルードされる場合に、
#define _IMAGE_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "textureManager.h"
#include "constants.h"

//==========================================================

// Imageクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace imageNS
{
	enum MODE { HORIZONTAL, VERTICAL };	// 読み込んだ画像に対して横にアニメーションが流れる場合はHORIZONTAL、縦の場合はVERTICAL
}

//==========================================================
// 画像を描画するクラスです。
// ゲーム内のアイテムはすべて画像があると考えられるので、
// このクラスを継承することを勧めます
class Image
{
protected:
	// Imageプロパティ
	Graphics *mGraphics;					// graphicsへのポインタ
	TextureManager *mTextureManager;		// texture managerへのポインタ
	// spriteDataはGraphics::drawSprite()による画像の描画に必要なデータを保持する
	SpriteData mSpriteData;					// "graphics.h"で定義されているSpriteData
	COLOR_ARGB mColorFilter;				// color filterとして適用（白を使用）
	int mCols;								// 複数のフレームを持つスプライト内の列数（1からnまで）
	int mStartFrame;						// 複数のフレームを持つアニメーションシーケンスの最初のフレーム番号
	int mEndFrame;							// 複数のフレームを持つアニメーションシーケンスの最後のフレーム番号
	int mCurrentFrame;						// アニメーションの現在のフレーム
	double mFrameDelay;						// アニメーションの各フレームを表示する間隔（秒単位）
	double mAnimTimer;						// アニメーションの時間を測るために使用する変数
	HRESULT mHr;							// 標準の戻り値
	bool mLoop;								// アニメーションを繰り返す場合、trueに設定（デフォルト）
	bool mVisible;							// 画像が可視な場合、true
	bool mInitialized;						// 初期化が成功した場合は、true
	bool mAnimComplete;						// ループしないアニメーションシーケンスが完了したときにtrueとなる
	imageNS::MODE mMode;					// 読み込んだ画像に対して横にアニメーションが流れる場合はHORIZONTAL、縦の場合はVERTICAL

public:
	// コンストラクタ
	Image();

	// デストラクタ
	virtual ~Image();

	// SpriteData構造体への参照を戻す
	const virtual SpriteData& getSpriteInfo()
	{
		return mSpriteData;
	}
	const virtual SpriteData& getSpriteData() { return mSpriteData; }


	// visibleパラメータをｓ戻す
	virtual bool getVisible()
	{
		return mVisible;
	}

	// X位置を戻す
	virtual float getX()
	{
		return mSpriteData.x;
	}

	// Y位置を戻す
	virtual float getY()
	{
		return mSpriteData.y;
	}

	// 倍率を戻す
	virtual float getScale()
	{
		return mSpriteData.scale;
	}

	// 幅を戻す
	virtual int   getWidth()
	{
		return mSpriteData.width;
	}

	// 高さを戻す
	virtual int   getHeight()
	{
		return mSpriteData.height;
	}

	// 中心点のXを戻す
	virtual float getCenterX()
	{
		return mSpriteData.x + mSpriteData.width / 2 * getScale();
	}

	// 中心点のYを戻す
	virtual float getCenterY()
	{
		return mSpriteData.y + mSpriteData.height / 2 * getScale();
	}

	// 回転角度（度単位）を戻す
	virtual float getDegrees()
	{
		return mSpriteData.angle*(180.0f / (float)PI);
	}

	// 回転角度（ラジアン）を戻す
	virtual float getRadians()
	{
		return mSpriteData.angle;
	}

	// アニメーションのフレーム間の間隔を戻す
	virtual float getFrameDelay() {
		return (float)mFrameDelay;
	}

	// 開始フレームの番号を戻す
	virtual int   getStartFrame()
	{
		return mStartFrame;
	}

	// 終了フレームの番号を戻す
	virtual int   getEndFrame()
	{
		return mEndFrame;
	}

	// 現在のフレームの番号を戻す
	virtual int   getCurrentFrame()
	{
		return mCurrentFrame;
	}

	// ImageのRECT構造体を戻す
	virtual RECT  getSpriteDataRect()
	{
		return mSpriteData.rect;
	}

	// アニメーション完了の状態を戻す
	virtual bool  getAnimationComplete()
	{
		return mAnimComplete;
	}

	// colorFilterを戻す
	virtual COLOR_ARGB getColorFilter() { return mColorFilter; }

	// X位置を設定
	virtual void setX(float newX)
	{
		mSpriteData.x = newX;
	}

	// Y位置を設定
	virtual void setY(float newY)
	{
		mSpriteData.y = newY;
	}

	// 拡大縮小を設定
	virtual void setScale(float s)
	{
		mSpriteData.scale = s;
	}

	// 回転角度を設定（度単位）
	// 0度は上向きです。角度は時計回りに進みます。
	virtual void setDegrees(float deg)
	{
		mSpriteData.angle = deg*((float)PI / 180.0f);
	}

	// 回転角度を設定（ラジアン単位）
	// 0度は上向きです。角度は時計回りに進みます。
	virtual void setRadians(float rad)
	{
		mSpriteData.angle = rad;
	}

	// 可視性を設定
	virtual void setVisible(bool v)
	{
		mVisible = v;
	}

	// アニメーションのフレーム間の間隔を設定
	virtual void setFrameDelay(float d)
	{
		mFrameDelay = d;
	}

	// アニメーションの開始フレームと終了フレームを設定
	virtual void setFrames(int s, int e)
	{
		mStartFrame = s; mEndFrame = e;
	}

	// アニメーションの現在のフレームを設定
	virtual void setCurrentFrame(int c);

	// CurrentFrameを描画するspriteData.rectを設定
	virtual void setRect();

	// spriteData.rectをrに設定
	virtual void setSpriteDataRect(RECT r)
	{
		mSpriteData.rect = r;
	}

	// アニメーションループを設定（lp = trueの場合にループする）
	virtual void setLoop(bool lp)
	{
		mLoop = lp;
	}

	// アニメーション完了ブール値を設定
	virtual void setAnimationComplete(bool a) { mAnimComplete = a; };

	// 色フィルタを設定（WHITEを使用すると、変化しない）
	virtual void setColorFilter(COLOR_ARGB color)
	{
		mColorFilter = color;
	}

	// TextureManagerを設定
	virtual void setTextureManager(TextureManager *textureM)
	{
		mTextureManager = textureM;
	}

	// MODEを設定
	virtual void setMode(imageNS::MODE m)
	{
		mMode = m;
	}


	// 画像を水平方向に反転（鏡）
	virtual void flipHorizontal(bool flip)
	{
		mSpriteData.flipHorizontal = flip;
	}

	// 画像を垂直方向に反転
	virtual void flipVertical(bool flip)
	{
		mSpriteData.flipVertical = flip;
	}

	// Imageを初期化
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	// Graphicsへのポインタ
	// Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
	// Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
	// テクスチャ内の列数（1からnまで）（0は1と同じ）
	// TextureManagerへのポインタ
	virtual bool initialize(Graphics *g, int width, int height, int nclos,
		TextureManager *textureM);

	// colorをフィルタとして使って画像を描画。colorのデフォルトは白。
	// colorパラメータはオプション
	// （image.hで、デフォルトとして白が割り当てられている）
	// 実行前：spriteBegin()を呼び出す
	// 実行後：spriteEnd()を呼び出す
	virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE);

	// 指定されたSpriteDataを使ってこの画像を描画
	// 現在のSpriteData.rectを使って、テクスチャを選択
	// 実行前：spriteBegin()を呼び出す
	// 実行後：spriteEnd()を呼び出す
	virtual void draw(SpriteData sd, const COLOR_ARGB color = graphicsNS::WHITE, const UINT textureN = 0);

	virtual void update(const float frameTime);

	// パラメータリセット
	virtual void reset();
};
#endif