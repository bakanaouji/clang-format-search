//==========================================================
/// @file
/// @brief    GameErrorクラス
/// @author   阿部拳之
///
/// @attention  ゲームエンジンによってスローされるErrorクラス

//==========================================================
#ifndef _GAMEERROR_H            // このファイルが複数の箇所でインクルードされる場合に、
#define _GAMEERROR_H            // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <exception>

// GameErrorクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace gameErrorNS
{
	// エラーコード
	// 負の数は、ゲームをシャットダウンする必要のある致命的なエラーを表します。
	// 正の数は、ゲームをシャットダウンする必要のない警告を表します。
    const int FATAL_ERROR = -1;
    const int WARNING = 1;
}

//==========================================================
// GameErrorクラス。ゲームエンジンによってエラーが検知されたときにスローされます。
// std::exceptionを継承
class GameError : public std::exception
{
private:
    int     mErrorCode;		// エラーコード
    std::string mMessage;	// エラーメッセージ
public:
	// デフォルトコンストラクタ
    GameError() throw() :mErrorCode(gameErrorNS::FATAL_ERROR), mMessage("Undefined Error in game.") {}
    // コピーコンストラクタ
    GameError(const GameError& e) throw(): std::exception(e), mErrorCode(e.mErrorCode), mMessage(e.mMessage) {}
    // 引数ありのコンストラクタ
    GameError(int code, const std::string &s) throw() :mErrorCode(code), mMessage(s) {}
    // 代入演算子
    GameError& operator= (const GameError& rhs) throw() 
    {
        std::exception::operator=(rhs);
        this->mErrorCode = rhs.mErrorCode;
        this->mMessage = rhs.mMessage;
    }
	// デストラクタ
    virtual ~GameError() throw() {};

	// 基本クラスに対するオーバーライド
    virtual const char* what() const throw() { return this->getMessage(); }

    const char* getMessage() const throw() { return mMessage.c_str(); }
    int getErrorCode() const throw() { return mErrorCode; }
};

#endif
