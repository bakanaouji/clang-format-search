//==========================================================
/// @file
/// @brief    MpTextImageクラス
/// @author   阿部拳之
///
/// @attention  MPという文字の画像を表すクラスです。
///				システムグラフィックスとして使用できます。

//==========================================================
#ifndef _MP_TEXT_IMAGE_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _MP_TEXT_IMAGE_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"

//==========================================================
// MpTextImageクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace mpTextImageNS
{
	const int WIDTH = 40;		// 画像の幅（各フレーム）
	const int HEIGHT = 20;		// 画像の高さ
	const int X = 60;			// 初期位置
	const int Y = 30;
	const int TEXTURE_COLS = 5;	// テクスチャは5列
	const int START_FRAME = 20;	// アニメーションはフレーム15から開始
}

//==========================================================
// MPという文字の画像を表すクラスです。
// システムグラフィックスとして使用できます。
class MpTextImage : public Image
{
public:
	// コンストラクタ
	MpTextImage();
};
#endif // !_MP_TEXT_IMAGE_H
