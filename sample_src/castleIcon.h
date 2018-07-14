//==========================================================
/// @file
/// @brief    CastleIconクラス
/// @author   阿部拳之
///
/// @attention  城のアイコン画像を表すクラスです。

//==========================================================
#ifndef _CASTLE_ICON_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _CASTLE_ICON_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"

//==========================================================

// CastleIconクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace castleIconNS
{
	const int WIDTH = 48;			// 画像の幅（各フレーム）
	const int HEIGHT = 48;			// 画像の高さ
	const int X = 780;				// 初期位置
	const int Y = 0;
	const int TEXTURE_COLS = 1;		// テクスチャは1列
	const int START_FRAME = 1;		// アニメーションはフレーム1から開始
}

// 勇者のアイコン画像を表すクラスです。
class CastleIcon : public Image
{
public:
	// コンストラクタ
	CastleIcon();
};
#endif // !_BRAVE_ICON_H
