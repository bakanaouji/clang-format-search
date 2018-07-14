//==========================================================
/// @file
/// @brief    textureManager.hの実装
/// @author   阿部拳之
///
/// @attention  テクスチャを扱うクラスです。
///				画像ファイルの読み込みなどを行います。
#include "textureManager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
TextureManager::TextureManager()
{
	mGraphics = NULL;
	mInitialized = false;            // 初期化に成功した場合は、true
}

//=============================================================================
// デストラクタ
//=============================================================================
TextureManager::~TextureManager()
{
	for (UINT i = 0; i < mTexture.size(); i++)
		safeReleaseTexture(mTexture[i]);
}

//=============================================================================
// テクスチャファイルをディスクから読み込む
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool TextureManager::initialize(Graphics *g, std::string file)
{
	bool success = true;
	try {
		mGraphics = g;								// graphicsオブジェクト
		for (UINT i = 0; i < file.size(); i++)		// 小文字に変換
			file.at(i) = tolower(file.at(i));
		if (file.rfind(".txt") == file.size() - 4)	// .txt拡張子の場合
		{
			// それぞれのテクスチャ名を含むファイルを開く
			std::ifstream infile(file.c_str());
			if (!infile)                     // 開くのに失敗した場合
				return false;
			std::string name;
			while (getline(infile, name))
			{
				mFileNames.push_back(name);  // ファイル名を追加
				mWidth.push_back(0);         // テクスチャの幅に余白を作成
				mHeight.push_back(0);        // テクスチャの高さに余白を作成
				mTexture.push_back(NULL);    // テクスチャに余白を作成
			}
			infile.close();
		}
		else    // .txtファイルでなく、１つのテクスチャ名がファイルに含まれている
		{
			mFileNames.push_back(file);  //ファイル名を追加
			mWidth.push_back(0);         // テクスチャの幅に余白を作成
			mHeight.push_back(0);        // テクスチャの高さに余白を作成
			mTexture.push_back(NULL);    // テクスチャに余白を作成
		}

		// テクスチャファイルの読み込み
		for (UINT i = 0; i < mFileNames.size(); i++)
		{
			mHr = mGraphics->loadTexture(mFileNames[i].c_str(),
				graphicsNS::TRANSCOLOR, mWidth[i], mHeight[i], mTexture[i]);
			if (FAILED(mHr))
				success = false;    // 最低でも１つのテクスチャの読み込みに失敗
		}
	}
	catch (...) { return false; }
	mInitialized = true;                    // 正常に初期化された場合、trueを設定
	return success;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void TextureManager::onLostDevice()
{
	try
	{
		if (!mInitialized)
			return;
		for (UINT i = 0; i<mTexture.size(); i++)
			safeReleaseTexture(mTexture[i]);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::WARNING,
			"Warning, TextureManager onLostDevice attempted to access an invalid texture."));
	}
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出される
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!mInitialized)
		return;
	// load texture files
	for (UINT i = 0; i<mFileNames.size(); i++)
	{
		mHr = mGraphics->loadTexture(mFileNames[i].c_str(),
			graphicsNS::TRANSCOLOR, mWidth[i], mHeight[i], mTexture[i]);
		if (FAILED(mHr))
			safeReleaseTexture(mTexture[i]);
	}
}