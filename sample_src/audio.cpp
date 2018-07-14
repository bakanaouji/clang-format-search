//==========================================================
/// @file
/// @brief    audio.h�̎���
/// @author   �������V
///
/// @attention  �I�[�f�B�I���Đ��A��~����N���X�ł��B

//==========================================================

#include "audio.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Audio::Audio()
{
    mXactEngine = NULL;
    mWaveBank = NULL;
    mSoundBank = NULL;
    mCueI = 0;
    mMapWaveBank = NULL;         // UnmapViewOfFile()���Ăяo���ăt�@�C�������
    mSoundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        mCoInitialized = true;
    else
        mCoInitialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Audio::~Audio()
{
    // XACT���V���b�g�_�E��
    if( mXactEngine )
    {
		// XACT�G���W�����V���b�g�_�E�����A���\�[�X�����
        mXactEngine->ShutDown();
        mXactEngine->Release();
    }

    if ( mSoundBankData )
        delete[] mSoundBankData;
    mSoundBankData = NULL;

	// xactEngine()->ShutDown()����߂�����A�������}�b�v�g�t�@�C�������
    if( mMapWaveBank )
        UnmapViewOfFile( mMapWaveBank );
    mMapWaveBank = NULL;

    if( mCoInitialized )        // CoInitializeEx�����������ꍇ
        CoUninitialize();
}

//=============================================================================
// ������
// ���̃��\�b�h�͎��ɏ]���Ďg�p����K�v������܂��B
//		1. xactEngine->Initialize���Ăяo�����Ƃɂ����XACT��������
//		2. �g�p������XACT wave bank���쐬
//      3. �g�p������XACT sound bank���쐬
//		4. �g�p����XACT cue�ւ̃C���f�b�N�X���i�[
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

    // XACT�����^�C���̏������ƍ쐬
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    result = mXactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // �������}�b�v�ƃt�@�C��IO���g�p����u�C���������v��XACT�E�F�C�u�o���N�t�@�C�����쐬
    result = E_FAIL; // ���s���f�t�H���g�Ƃ��āA�������ɒu������
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

				// mapWaveBank���t�@�C���ւ̃n���h����ێ�
				// �s�v�ȃn���h���͕���
                CloseHandle( hMapFile );
            }
        }
		// mapWaveBank���t�@�C���ւ̃n���h����ێ����Ă���̂ŁA
		// �s�v�ȃn���h���͕���
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

	// XACT�ō쐬�����T�E���h�o���N�t�@�C����ǂݎ��A�o�^
    result = E_FAIL;    // ���s���f�t�H���g�Ƃ��āA�������ɒu������
    hFile = CreateFile( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
			// �T�E���h�o���N�p�̃�������\��
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
// �T�E���h�G���W���̎����I�^�X�N�����s
//=============================================================================
void Audio::run()
{
    if (mXactEngine == NULL)
        return;
    mXactEngine->DoWork();
}

//=============================================================================
// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N����Đ�
// �L���[�����݂��Ȃ��ꍇ�́A�T�E���h���Đ�����Ȃ������ŁA�G���[�͔������Ȃ�
//=============================================================================
void Audio::playCue(const char cue[])
{
    if (mSoundBank == NULL)
        return;
	// �T�E���h�o���N����L���[�C���f�b�N�X���擾
    mCueI = mSoundBank->GetCueIndex( cue );
    mSoundBank->Play( mCueI, 0, 0, NULL );
}

//=============================================================================
// �L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N�Œ�~
// �L���[�����ݏꍇ�A�G���[�͔������Ȃ�
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (mSoundBank == NULL)
        return;
	// �T�E���h�o���N����L���[�C���f�b�N�X���擾
    mCueI = mSoundBank->GetCueIndex( cue );
    mSoundBank->Stop( mCueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
