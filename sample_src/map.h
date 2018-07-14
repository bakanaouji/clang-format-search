//==========================================================
/// @file
/// @brief    Mapクラス
/// @author   阿部拳之
///
/// @attention  ステージ内のマップ情報を保持するクラスです。

//==========================================================
#ifndef _MAP_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _MAP_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "entity.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <sstream> //文字ストリーム
#include "gameError.h"

using namespace std;

//==========================================================
// Mapクラスの定数
// ゲーム内でのステータス以外をここに記述
namespace mapNS
{
	const int TEXTURE_SIZE = 32;		// 画像サイズ（1タイル分）
	const int TEXTURE_COLS = 16;		// 行数
	const int MAP_HEIGHT = 23;			// マップの高さ
	const int MAP_WIDTH = 40;			// マップの幅
	const int BARRICADE_NUM = 8;		// 初期バリケードの数
	const int MAP_ROCK = 9;				// マップデータでの、岩のマップチップの割り当て（プログラム内では使用しないが、備忘録として）
	const int COL_ROCK = 1;				// 当たり判定データでの、岩の割り当て
	const int COL_CASTLE = 2;			// 当たり判定データでの、城の割り当て
	const int OBJ_BARRICADE = 0;		// オブジェクトデータでの、バリケードの割り当て
	const int OBJ_NEAR_BARRICADE = -2;	// オブジェクトデータでの、バリケードに隣接しているタイルの割り当て
	const int OBJ_NEAR_CASTLE = 1;		// オブジェクトデータでの、城に隣接しているタイルの割り当て
	const int OBJ_NONE = -1;			// オブジェクトデータでの、何もないタイルの割り当て
	const int COL_ENEMY_INCLEMENT = 3;	// 当たり判定データでの、敵の割り当て（この値を元の値に足す）
}

//==========================================================
// ステージ内のマップ情報を保持するクラスです。
// タイル座標は(画面上の座標 / 32)で与えられます。
class Map : public Entity
{
private:
	// マップデータ
	// どのタイルにどのマップチップを描画するかを決めるデータ
	// 城の周りを囲う岩は9に設定し、それ以外は自由にマップチップを選んでよい
	int mMapData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// 当たり判定のデータ
	// ゲームを通して移動できない箇所は1か2に設定する、岩は1に、城は2にする
	// 敵がいるタイルには元の値+3を設定する、この値は敵のタイルがなるべくかぶらないようにすることに利用
	int mColData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// 当たり判定のデータの初期値
	// 初期値をとることで当たり判定データをリセットしやすくする
	int mColDataInit[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
	// マップ上のオブジェクトデータ
	// 見えないオブジェクト（バリケードや城に隣接していると敵に知らせるためのオブジェクト）も管理する
	// バリケードは0、バリケードに隣接しているタイルの一部には-2を、城に隣接しているタイルには1を割り当てる
	// このデータは敵の意思決定にのみしようするため、プレイヤーは利用しない
	int mObjData[mapNS::MAP_HEIGHT][mapNS::MAP_WIDTH];
public:
	// コンストラクタ
	Map();

	//==========================================================
	// 継承されたメンバ関数
	//==========================================================
	
	// マップを描画
	virtual void draw();

	//==========================================================
	// 新しく追加するメンバ関数
	//==========================================================

	//==========================================================
	// getter
	//==========================================================

	// マップデータの取得　
	// 引数：x	取得したいタイル座標X
	// 引数：y	取得したいタイル座標Y
	int getMapData(const int x, const int y)
	{
		return mMapData[x][y];
	}
	// マップ当たり判定データの取得
	// 引数：x	取得したいタイル座標X
	// 引数：y	取得したいタイル座標Y
	int getMapCol(const int x, const int y)
	{
		return mColData[x][y];
	}
	// マップ上のオブジェクトデータ更新
	// 引数：x	取得したいタイル座標X
	// 引数：y	取得したいタイル座標Y
	int getMapObj(const int x, const int y)
	{
		return mObjData[x][y];
	}

	// マップデータの更新
	// 引数：x		更新したい座標X
	// 引数：y		更新したい座標Y
	// 引数：val	マップデータ
	void updateMapData(const float x, const float y, const int val);
	// マップ当たり判定の更新 引数は更新したい座標値と更新する値
	// val -> 0 : 判定なし　val -> 1 : 判定あり
	// 引数：x		更新したい座標X
	// 引数：y		更新したい座標Y
	// 引数：val	当たり判定のデータ
	void updateMapCol(const float x, const float y, const int val);
	// マップ上のオブジェクトデータ更新
	// 引数：x		更新したい座標X
	// 引数：y		更新したい座標Y
	// 引数：val	オブジェクトのデータ
	void updateMapObj(const float x, const float y, const int val);
	// マップ上のオブジェクトデータ更新
	// 引数：x		更新したいタイル座標X
	// 引数：y		更新したいタイル座標Y
	// 引数：val	マップデータ
	void updateMapObjInt(const int y, const int x, const int val);
	// マップ上の当たり判定データすべてをリセットする関数
	void resetMapCol() {
		for (int i = 0; i < mapNS::MAP_HEIGHT; ++i)
		{
			for (int j = 0; j < mapNS::MAP_WIDTH; ++j)
			{
				mColData[i][j] = mColDataInit[i][j];
			}
		}
	}
	// 指定されたマップの当たり判定データをリセットする
	// 引数：x		リセットしたいタイル座標X
	// 引数：y		リセットしたいタイル座標Y
	void resetMapCol(const int y, const int x) {
		if (x < 0 || x >= mapNS::MAP_WIDTH || y < 0 || y >= mapNS::MAP_HEIGHT) { return; }
		mColData[y][x] = mColDataInit[y][x];
	}
	// 指定されたステージのマップデータを読み込む
	// 引数：stageNum	ステージ番号
	void readMapFile(const int stageNum);
};

#endif