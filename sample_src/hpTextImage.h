//==========================================================
/// @file
/// @brief    HpTextImageクラス
/// @author   阿部拳之
///
/// @attention  HPという文字の画像を表すクラスです。
///				システムグラフィックスとして使用できます。

//==========================================================
#ifndef _HP_TEXT_IMAGE_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _HP_TEXT_IMAGE_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "image.h"
#include "constants.h"

//==========================================================
// HpTextImageクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace hpTextImageNS
{
	const int WIDTH = 40;		// 画像の幅（各フレーム）
	const int HEIGHT = 20;		// 画像の高さ
	const int X = 60;			// 初期位置
	const int Y = 5;
	const int TEXTURE_COLS = 5;	// テクスチャは5列
	const int START_FRAME = 15;	// アニメーションはフレーム15から開始
}

//==========================================================
// HPという文字の画像を表すクラスです。
// システムグラフィックスとして使用できます。
class HpTextImage : public Image
{
public:
	// コンストラクタ
	HpTextImage();
};
#endif // !_HP_TEXT_IMAGE_H
