//==========================================================
/// @file
/// @brief    graphics.hの実装
/// @author   阿部拳之
///
/// @attention  スプライトなどを描画するための情報を扱ったり、描画処理を行うクラスです。


#include "graphics.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Graphics::Graphics()
{
	mDirect3d = NULL;
	mDevice3d = NULL;
	mSprite = NULL;
	mFullscreen = false;
	mWidth = GAME_WIDTH;    // 幅と高さはinitialize()で置き換えられる
	mHeight = GAME_HEIGHT;
	mBackColor = graphicsNS::BACK_COLOR;
}

//=============================================================================
// デストラクタ
//=============================================================================
Graphics::~Graphics()
{
	releaseAll();
}

//=============================================================================
// すべてを解放
//=============================================================================
void Graphics::releaseAll()
{
	safeRelease(mSprite);
	safeRelease(mDevice3d);
	safeRelease(mDirect3d);
}

//=============================================================================
// DirextXグラフィックスを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	mHwnd = hw;
	mWidth = w;
	mHeight = h;
	mFullscreen = full;

	// Direct3Dデバイスを作成
	mDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (mDirect3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp();        // D3Dプレゼンテーションパラメータを初期化
	if (mFullscreen)    // 全画面表示モードの場合
	{
		if (isAdapterCompatible())   // アダプターが対応しているか
			// 対応しているリフレッシュレートを設定
			mD3dpp.FullScreen_RefreshRateInHz = mPMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the specified resolution and/or format."));
	}

	// グラフィックスカードがハードウェアテクスチャリングとライティング、
	// 頂点シェーダーをサポートしているかどうかを判定
	D3DCAPS9 caps;
	DWORD behavior;
	mResult = mDirect3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// デバイスがHWテクスチャリングとライティングをサポートしていない場合、
	// または、1.1頂点シェーダーをハードウェアでサポートしていない場合、
	// ソフトウェア頂点処理に切り替えます。
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// ソフトウェアのみの処理を使用
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// ハードウェアのみの処理の使用
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// Direct3Dデバイスを作成
	mResult = mDirect3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		mHwnd,
		behavior,
		&mD3dpp,
		&mDevice3d);

	if (FAILED(mResult))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	mResult = D3DXCreateSprite(mDevice3d, &mSprite);
	if (FAILED(mResult))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

	// プリミティブのアルファブレンド用の構成
	mDevice3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	mDevice3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mDevice3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// D3Dプレゼンテーションパラメータを初期化
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&mD3dpp, sizeof(mD3dpp));            // 構造体に0を設定
		// 必要なパラメータを設定
		mD3dpp.BackBufferWidth = mWidth;
		mD3dpp.BackBufferHeight = mHeight;
		if (mFullscreen)                                // 全画面表示の場合
			mD3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// 24ビットカラー
		else
			mD3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // デスクトップ設定を使用
		mD3dpp.BackBufferCount = 1;
		mD3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		mD3dpp.hDeviceWindow = mHwnd;
		mD3dpp.Windowed = (!mFullscreen);
		mD3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		mD3dpp.EnableAutoDepthStencil = true;
		mD3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;   // Depth 24, Stencil 8
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));
	}
}

//=============================================================================
// テクスチャをデフォルトのD3Dメモリに読み込む（通常のテクスチャで使用）
// エンジンの使用時に内部的にのみ使用します。
// ゲームテクスチャの読み込みには、TextureManagerクラスを使用します。
// 実行前：filenameは、テクスチャファイルの名前
//		　 transcolorは、透明として扱う色
// 実行後：widthとheight = テクスチャの寸法
//         textureは、テクスチャを指す
// HRESULTを戻す
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// ファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	mResult = E_FAIL;

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// 幅と高さをファイルから取得
		mResult = D3DXGetImageInfoFromFile(filename, &info);
		if (mResult != D3D_OK)
			return mResult;
		width = info.Width;
		height = info.Height;

		// ファイルを読み込んで、新しいテクスチャを作成
		mResult = D3DXCreateTextureFromFileEx(
			mDevice3d,          // 3Dデバイス
			filename,           // 画像ファイルの名前
			info.Width,         // テクスチャの幅
			info.Height,        // テクスチャの高さ
			1,                  // ミップマップのレベル（チェーンなしの場合は1）
			0,                  // 使用方法（usage）
			D3DFMT_UNKNOWN,     // サーフェイスフォーマット（デフォルト）
			D3DPOOL_DEFAULT,    // テクスチャ用のメモリの種類
			D3DX_DEFAULT,       // 画像フィルタ
			D3DX_DEFAULT,       // ミップフィルタ
			transcolor,         // 透明用の色キー
			&info,              // ピットマップファイル情報（読み込んだファイルから）
			NULL,               // カラーパレット
			&texture);          // 目的のテクスチャ

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return mResult;
}

//=============================================================================
// テクスチャをシステムメモリに読み込む（システムメモリはロック可能）
// ピクセルデータへの直接アクセスを可能にします。
// TextureManagerクラスを使って、表示するテクスチャを読み込みます。
// 実行前：filenameは、テクスチャファイルの名前
//		   transcolorは、透明として扱う色
// 実行後：widthとheight = テクスチャの寸法
//         textureは、テクスチャを指す
// HRESULTを戻し、TextureData構造体にデータを格納する
//=============================================================================
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// ビットマップファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	mResult = E_FAIL;        // 標準のWindows戻り値

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// 幅と高さをビットマップファイルから取得
		mResult = D3DXGetImageInfoFromFile(filename, &info);
		if (mResult != D3D_OK)
			return mResult;
		width = info.Width;
		height = info.Height;

		// ビットマップ画像ファイルを読み込んで、新しいテクスチャを作成
		mResult = D3DXCreateTextureFromFileEx(
			mDevice3d,          // 3Dデバイス
			filename,           // ビットマップファイルの名前
			info.Width,         // ビットマップ画像の幅
			info.Height,        // ビットマップ画像の高さ
			1,                  // ミップマップのレベル（チェーンなしの場合は1）
			0,                  // 使用方法（usage）
			D3DFMT_UNKNOWN,     // サーフェイスフォーマット（デフォルト）
			D3DPOOL_SYSTEMMEM,  // システムメモリはロック可能
			D3DX_DEFAULT,       // 画像フィルタ
			D3DX_DEFAULT,       // ミップフィルタ
			transcolor,         // 透明用の色キー
			&info,              // ビットマップファイル情報（読み込んだファイルから）
			NULL,               // カラーパレット
			&texture);          // 目的のテクスチャ

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return mResult;
}

//=============================================================================
// 頂点バッファを作成
// 実行前：verts[]に頂点データが格納されている
//		   size = verts[]のサイズ
// 実行後：成功した場合、&vertexBufferがバッファを指す
//=============================================================================
HRESULT Graphics::createVertexBuffer(VertexC verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
	// 標準のWindows戻り値
	HRESULT result = E_FAIL;

	// 頂点バッファを作成
	result = mDevice3d->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL);
	if (FAILED(result))
		return result;

	void *ptr;
	// データが転送されてくる前に、バッファをロックする必要がある
	result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
	if (FAILED(result))
		return result;
	memcpy(ptr, verts, size);   // 頂点データをバッファにコピー
	vertexBuffer->Unlock();     // バッファのロックを解除

	return result;
}

//=============================================================================
// 三角形ファンを使って、アルファ透過性を持つ四角形を表示
// 実行前：createVertexBufferを使ってvertexBufferを作成し、
//		   四角形を時計回りの順序で定義する4つの頂点を格納しておく
//		   g3ddev->BeginSceneを呼び出す
// 実行後：四角形が描画される
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{
	HRESULT result = E_FAIL;    // 標準のWindows戻り値

	if (vertexBuffer == NULL)
		return false;

	// アルファブレンドを有効化
	mDevice3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	mDevice3d->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
	mDevice3d->SetFVF(D3DFVF_VERTEX);
	result = mDevice3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// アルファブレンドを無効化
	mDevice3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (FAILED(result))
		return false;

	return true;
}

//=============================================================================
// バックバッファを表示
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	// バックバッファを画面に表示
	mResult = mDevice3d->Present(NULL, NULL, NULL, NULL);
	return mResult;
}

//=============================================================================
// アダプターをチェックして、d3dppで指定されたBackBufferの高さ、幅、
// リフレッシュレートに対応しているかどうかを確認します。
// 対応するものが見つかった場合は、pMode構造体に、
// 対応しているモードのフォーマットを設定します。
// 実行前：d3dppが初期化されている
// 実行後：対応しているモードが見つかり、pMode構造体にデータが
// 設定された場合、trueを戻します。
// 対応しているモードが見つからなかった場合、falseを戻します。
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = mDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		mD3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		mResult = mDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			mD3dpp.BackBufferFormat,
			i, &mPMode);
		if (mPMode.Height == mD3dpp.BackBufferHeight &&
			mPMode.Width == mD3dpp.BackBufferWidth &&
			mPMode.RefreshRate >= mD3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

//=============================================================================
// SpriteData構造体に記述されたスプライトを描画
// color（オプション）は、フィルタのように適用される
// デフォルトは白（変化なし）
// 実行前：sprite->Begin()を呼び出す
// 実行後：sprite->End()を呼び出す
//		   spriteDate.rectは、描画するspriteDate.textureの部分を定義
//		   spriteData.rect.rightは、右端+1にする必要がある
//		   spriteDate.rect.bottomは、下端+1にする必要がある
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)      // テクスチャがない場合
		return;

	// スプライトの中心を特定
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale));
	// スプライトの画面位置
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	// X方向とY方向の拡大縮小
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	if (spriteData.flipHorizontal)  // 水平方向に反転する場合
	{
		scaling.x *= -1;            // Xスケールを負にして反転
		// 反転された画像の中心を取得
		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
		// 左端を中心に反転が起きるので、反転された画像を
		// 元と同じ位置に配置するために、右へ移動します。
		translate.x += (float)(spriteData.width*spriteData.scale);
	}
	if (spriteData.flipVertical)    // 垂直方向に反転する場合
	{
		scaling.y *= -1;            // Yスケールを負にして反転
		// 反転された画像の中心を取得
		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
		// 上端を中心に反転が起きるので、反転された画像を
		// 元と同じ位置に配置するために、下へ移動します。
		translate.y += (float)(spriteData.height*spriteData.scale);
	}
	// スプライトの回転、拡大縮小、配置を行うための行列を作成
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,                // 行列
		NULL,                   // 拡大縮小を行うとき、原点は左上のまま
		0.0f,                   // 拡大縮小時の回転なし
		&scaling,               // 拡大縮小の倍率
		&spriteCenter,          // 回転の中心
		(float)(spriteData.angle),  // 回転の角度
		&translate);            // X、Y位置

	// 行列をスプライトに適用
	mSprite->SetTransform(&matrix);

	// スプライトを描画
	mSprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//=============================================================================
// ウィンドウモードやフルスクリーンモードに設定
// 実行前：すべてのD3DPOOL_DEFAULTサーフェイスを解放する
// 実行後：すべてのサーフェイスを再作成
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (mFullscreen)			// すでにフルスクリーンモードだった場合は何もしない
				return;
			mFullscreen = true; break;
		case graphicsNS::WINDOW:
			if (mFullscreen == false)	// すでにウィンドウモードだった場合は何もしない
				return;
			mFullscreen = false; break;
		default:			// デフォルトではフルスクリーンモードとウィンドウモードを切り替える
			mFullscreen = !mFullscreen;
		}
		reset();
		if (mFullscreen)	// フルスクリーンモード
		{
			SetWindowLong(mHwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else				// ウィンドウモード
		{
			SetWindowLong(mHwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(mHwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// クライアント領域がGAME_WIDTH×GAME_HEIGHTとなるようにウィンドウサイズを調整
			RECT clientRect;
			GetClientRect(mHwnd, &clientRect);   // ウィンドウのクライアント領域のサイズを確保
			MoveWindow(mHwnd,
				0,                                           // 左
				0,                                           // 上
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // 右
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // 下
				TRUE);                                       // ウィンドウの再描画
		}

	}
	catch (...)
	{
		// エラーが発生した場合、ウィンドウモードに切り替えようとする
		SetWindowLong(mHwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(mHwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// クライアント領域がGAME_WIDTH×GAME_HEIGHTとなるようにウィンドウサイズを調整
		RECT clientRect;
		GetClientRect(mHwnd, &clientRect);   // ウィンドウのクライアント領域のサイズを確保
		MoveWindow(mHwnd,
			0,                                           // 左
			0,                                           // 上
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // 右
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // 下
			TRUE);                                       // ウィンドウの再描画
	}
}

//=============================================================================
// デバイスが消失していないかをチェック
//=============================================================================
HRESULT Graphics::getDeviceState()
{
	mResult = E_FAIL;    // 消失をデフォルトとし、成功時に置き換え
	if (mDevice3d == NULL)
		return  mResult;
	mResult = mDevice3d->TestCooperativeLevel();
	return mResult;
}

//=============================================================================
// グラフィックスデバイスをリセット
//=============================================================================
HRESULT Graphics::reset()
{
	initD3Dpp();						 // D3Dプレゼンテーションパラメータを初期化
	mSprite->OnLostDevice();             // スプライトを解放
	// グラフィックスデバイスのリセットを試みる
	mResult = mDevice3d->Reset(&mD3dpp);

	// プリミティブのアルファブレンド用の構成
	mDevice3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	mDevice3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mDevice3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	mSprite->OnResetDevice();            // スプライトを再確保
	return mResult;
}
