//==========================================================
/// @file
/// @brief    audio.hの実装
/// @author   阿部拳之
///
/// @attention  オーディオを再生、停止するクラスです。

//==========================================================

#include "audio.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Audio::Audio()
{
    mXactEngine = NULL;
    mWaveBank = NULL;
    mSoundBank = NULL;
    mCueI = 0;
    mMapWaveBank = NULL;         // UnmapViewOfFile()を呼び出してファイルを解放
    mSoundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        mCoInitialized = true;
    else
        mCoInitialized = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Audio::~Audio()
{
    // XACTをシャットダウン
    if( mXactEngine )
    {
		// XACTエンジンをシャットダウンし、リソースを解放
        mXactEngine->ShutDown();
        mXactEngine->Release();
    }

    if ( mSoundBankData )
        delete[] mSoundBankData;
    mSoundBankData = NULL;

	// xactEngine()->ShutDown()から戻った後、メモリマップトファイルを解放
    if( mMapWaveBank )
        UnmapViewOfFile( mMapWaveBank );
    mMapWaveBank = NULL;

    if( mCoInitialized )        // CoInitializeExが成功した場合
        CoUninitialize();
}

//=============================================================================
// 初期化
// このメソッドは次に従って使用する必要があります。
//		1. xactEngine->Initializeを呼び出すことによってXACTを初期化
//		2. 使用したいXACT wave bankを作成
//      3. 使用したいXACT sound bankを作成
//		4. 使用するXACT cueへのインデックスを格納
//=============================================================================
HRESULT Audio::initialize()
{
    HRESULT result = E_FAIL;
    HANDLE hFile;
    DWORD fileSize;
    DWORD bytesRead;
    HANDLE hMapFile;

    if( mCoInitialized == false)
        return E_FAIL;

    result = XACT3CreateEngine( 0, &mXactEngine );
    if( FAILED( result ) || mXactEngine == NULL )
        return E_FAIL;

    // XACTランタイムの初期化と作成
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    result = mXactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // メモリマップとファイルIOを使用する「インメモリ」のXACTウェイブバンクファイルを作成
    result = E_FAIL; // 失敗をデフォルトとして、成功時に置き換え
    hFile = CreateFile( WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, fileSize, NULL );
            if( hMapFile )
            {
                mMapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( mMapWaveBank )
                    result = mXactEngine->CreateInMemoryWaveBank( mMapWaveBank, fileSize, 0, 0, &mWaveBank );

				// mapWaveBankがファイルへのハンドルを保持
				// 不要なハンドルは閉じる
                CloseHandle( hMapFile );
            }
        }
		// mapWaveBankがファイルへのハンドルを保持しているので、
		// 不要なハンドルは閉じる
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

	// XACTで作成したサウンドバンクファイルを読み取り、登録
    result = E_FAIL;    // 失敗をデフォルトとして、成功時に置き換え
    hFile = CreateFile( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
			// サウンドバンク用のメモリを予約
            mSoundBankData = new BYTE[fileSize];
            if( mSoundBankData )
            {
                if( 0 != ReadFile( hFile, mSoundBankData, fileSize, &bytesRead, NULL ) )
                    result = mXactEngine->CreateSoundBank( mSoundBankData, fileSize, 0, 0, &mSoundBank );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    return S_OK;
}

//=============================================================================
// サウンドエンジンの周期的タスクを実行
//=============================================================================
void Audio::run()
{
    if (mXactEngine == NULL)
        return;
    mXactEngine->DoWork();
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクから再生
// キューが存在しない場合は、サウンドが再生されないだけで、エラーは発生しない
//=============================================================================
void Audio::playCue(const char cue[])
{
    if (mSoundBank == NULL)
        return;
	// サウンドバンクからキューインデックスを取得
    mCueI = mSoundBank->GetCueIndex( cue );
    mSoundBank->Play( mCueI, 0, 0, NULL );
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクで停止
// キューが存在場合、エラーは発生しない
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (mSoundBank == NULL)
        return;
	// サウンドバンクからキューインデックスを取得
    mCueI = mSoundBank->GetCueIndex( cue );
    mSoundBank->Stop( mCueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
